
/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "InterruptRoutines.h"
#include "ConfigurationMenu.h"
#include "LIS3DH_Registers.h"
#include "I2C_Interface.h"
#include "25LC256.h"
#include "MemoryCells.h"
#include "EEPROMCommunication.h"
#include "DataProcessing.h"
 void Pointer_resetter(void);


void blue_led_PWM_behaviour(uint16_t);



void Begin_Acquisition(void);
void Stop_Acquisition(void);
void Clear_Fifo(void);

uint8_t BeginFlag;
/* array used to change the period of the timer when the user changes the sampling frequency] */
uint16 timer_periods[4] = { 1000, 100, 40, 20 }; 

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    char message[100];
    
    uint8_t PacketReadyFlag=0;
    
    uint8_t sending_data=0;
    
    uint16_t PWM_period = 0;
    
    uint8_t i;
    
    
     /* default temperature format to send data is Celsius */
    m_temp_conversion= M_CELSIUS;
    q_temp_conversion= Q_CELSIUS;
    
    /* header and tail needed for UART communication */
    Packet_To_Send[0] = 0xA0;
    Packet_To_Send[9]= 0xC0;
    
    /* flags for temeperature and accelerometer new data initialized to zero */
    FIFODataReadyFlag = 0;
    TempDataReadyFlag = 0;
    
    /* index of the temperature data array initialized to 0 */
    temp_counter = 0;
    
    
    /* structs needed to create the tables about full scale range, sampling frequency, temperature format settings */
    options_to_display FSR = {"Character", "Full scale range",'1', "+/- 2g", '2', "+/- 4g",'3', "+/- 8g", '4', "+/- 16g"};
    options_to_display SampFreq = {"Character", "Sampling Frequency",'1', "1 Hz", '2', "10 Hz",'3', "25 Hz", '4', "50 Hz"};
    options_to_display TempFormat = {"Character","Temperature format",'c', "Celsius", 'f', "Fahrenheit",' ' , " ",' ' , " "};
    
    /* structs put in set_of_tables array */ 
    set_of_tables[0]= FSR;
    set_of_tables[1]= SampFreq;
    set_of_tables[2]= TempFormat;
  
    
    /*Starting I2C*/
    I2C_Peripheral_Start();
    
   /*Starting UART*/
    UART_Start();
  
    /*Starting SPI*/
    SPIM_Start();
    

    /* wait for components to start */
    CyDelay(10);
    

    
    Flag_Cell = EEPROM_readByte(FLAG_ADDRESS);
    UART_ClearTxBuffer();
    /*sprintf(message,"Flag_Cell_delfino == %d\r\n",Flag_Cell);
    UART_PutString(message);*/
    
    if (Flag_Cell == 0) {
        
        EEPROM_Initialization();
        Accelerometer_Configuration();
    
    }
    else {
        
        UART_PutString("EEPROM already initialized");
        start = EEPROM_readByte(BEGIN_STOP_ADDRESS);
        BeginFlag = 1;
        begin_pressed = start;
        Pointer = (uint16_t)(EEPROM_readByte(POINTER_ADDRESS_L) | (EEPROM_readByte(POINTER_ADDRESS_H)<<8));
 
    }
    
    isr_UART_StartEx(Custom_isr_UART);
    isr_FIFO_StartEx(Custom_isr_FIFO);
    isr_TIMER_StartEx(Custom_isr_TIMER);
    
    button_pressed = BUTTON_PRESSED;
    isr_BUTTON_StartEx(Custom_isr_BUTTON);
    
    
    change_settings_flag = 1;
    option_table = DONT_SHOW_TABLE;
    feature_selected = 0;
    keys_menu = 0;
    display_error = DONT_SHOW_ERROR;
    show_menu_flag = SHOW_MENU;
    while_working_menu_flag = DONT_SHOW_MENU;
    EEPROM_Full = 0;
    time_counter = 0;
    temp_counter = 0;
    /* flag that is set high when the user want to visualize the data */
    display_data=ACTIONS_DONE;

   
//     uint8_t buffer[64];
//    uint8_t buffer1[64];
//    uint8_t buffer2[64];
//    memset(&buffer,0x01,64);
//    
//    EEPROM_writePage(0x0020,buffer,64);
//    EEPROM_waitForWriteComplete();
////    EEPROM_writePage(0x0020,buffer,64);
////    EEPROM_waitForWriteComplete();
//    
//    EEPROM_readPage(0x0000,buffer1,64);
//    EEPROM_readPage(0x0040,buffer2,64);
//    
//    uint8_t i;
//    for(i= 0;i<64;i++){
//        sprintf(message,"buffer1 [%d] = %d\r\n",i,buffer1[i]);
//        UART_PutString(message);
//    }
//    for(i= 0;i<64;i++){
//        sprintf(message,"buffer2 [%d] = %d\r\n",i,buffer2[i]);
//        UART_PutString(message);
//    }
    
    for(;;)
    {
        
        switch(start){
            case (START):
                if (BeginFlag == 0) {
                    
                    uint8_t InterruptStatus;
                    InterruptStatus=CyEnterCriticalSection();
                    EEPROM_writeByte(BEGIN_STOP_ADDRESS, START);
                    EEPROM_waitForWriteComplete();
                    CyExitCriticalSection(InterruptStatus);
                }
                else BeginFlag = 0;
                
                Begin_Acquisition();
                
            break;
            case (STOP):
                if (BeginFlag == 0) {
                    Stop_Acquisition();
                    Clear_Fifo();
                }
                else BeginFlag = 0;

            break;
            default:
                break;
        }
        
        
        
        if (FIFODataReadyFlag && TempDataReadyFlag) {

            Digit_To_EEPROM_Conversion();
            FIFODataReadyFlag = 0;
            TempDataReadyFlag = 0;
            EEPROM_Data_Write();
        }
        
        
        /* if the user presses 'v' display_data flag is set to START.
        * \a message is displayed to warn him to switch to the bridge control panel.
        * \data are read from the EEPROM and packets to send thorugh UART are prepared.
        * \data are sent until the user press 'u'
        */
        
        switch (display_data) 
        {
            case START :
                /* function that display a message waring to switch in the bridge control panel */
                Switch_to_BridgeControlPanel();
                if(begin_pressed == START)
                    Stop_Acquisition();
                sending_data = START;
                Read_Pointer = FIRST_FREE_CELL;
                display_data = ACTIONS_DONE;
                break;
                
            case STOP :
                /* stop sending data throygh UART to the Brisdge Control Panel */
            
                /* display data set to ACTIONS_DONE */
                display_data = ACTIONS_DONE;
                sending_data = STOP;
                if(begin_pressed == START)
                    Begin_Acquisition();
                
                UART_PutString("Visualization data stopped\r\n");
                while_working_menu_flag = SHOW_MENU;
                change_settings_flag = 0;
                break;
            default :
                break;
        }
        
        
        
        
        if (sending_data == START) 
        {   
//            sprintf(message,"Pointer=%u,readPointer=%u\r\n",Pointer,Read_Pointer);
//            UART_PutString(message);
            if (Read_Pointer < Pointer) 
            {
                if (Read_Pointer <POINTER_LIMIT)
                    number_of_packets = WATERMARK_LEVEL + 1;
                else 
                    number_of_packets = END_EEPROM_PACKETS;
                
                EEPROM_Data_Read();
                EEPROM_To_Digit_Conversion();
                Digit_To_UOM_Conversion();
                Buffer_Creation();
                PacketReadyFlag = 1;
            }
            else Read_Pointer = FIRST_FREE_CELL;   
        }
        
        if( PacketReadyFlag)
        {   
            //UART_PutString("Packet ready\r\n");
            Packets_To_Send_Creation();
            
            PacketReadyFlag=0;    
            
        }
        
        if(while_working_menu_flag){
            While_Working_Menu();
            while_working_menu_flag = DONT_SHOW_MENU;
        }
        if(show_menu_flag){
            Keys_Menu();
            show_menu_flag = DONT_SHOW_MENU;
            keys_menu = 1;
        }
        
        if(keys_menu == 1){
            if(option_table != DONT_SHOW_TABLE){
                Show_Table(option_table);
                keys_menu = 0;
           }
        }

        
     
        /* Value of option table defines which settings have to be modified:
        * option table= FSR -> change the full scale range of the accelerometer
        * option table = SAMP_FREQ -> change the sampling frequency of the acceleromter
        * option table = TEMP -> change temprature data format
        * Depending on option table the value of feature_selected variable is used 
        * to operate the correct change on the acquisition settings */
        if (option_table!= DONT_SHOW_TABLE && feature_selected) {
            switch (option_table) 
           {
                
                case F_S_R:
                    /* change full scale range and store it in EEPROM*/
                    EEPROM_Store_FSR();
                    Change_Accelerometer_FSR(feature_selected);
                    /* Pointer resetted at the first available cell (0x0007)*/
                    Pointer_resetter();
                    Clear_Fifo();
                   break;
                case SAMP_FREQ:
                    /* data need to be deleted: the timer is stopped to not generate new data */
                    Timer_Stop();
                    /* change sampling freqeuncy */
                    EEPROM_Store_Freq();
                    if (begin_pressed) {
                    Change_Accelerometer_SampFreq(feature_selected);
                    }
                    /* change timer frequency in order to change the fequency of the isr */
                    Timer_WritePeriod(timer_periods[feature_selected-1]);
                    /* Pointer resetted at the first available cell (0x0007)*/
                    Timer_Stop();
                    Pointer_resetter();
                    Clear_Fifo();
                    break;
                case TEMP:
                    /* to do */
                    EEPROM_Store_Temp();
                    /* change the coeffients for the temperature sensor data conversion depending
                    * \to the user input: 
                    * \feature_selected = 1 -> Celsius coefficients
                    * \feature_selected =2 -> Fahrenheit coefficients
                    */
                    if (feature_selected == 1) 
                    {
                        m_temp_conversion= M_CELSIUS;
                        q_temp_conversion= Q_CELSIUS;
                        UART_PutString("Temperature data format: Celsius\r\n\n");
                    }
                    else
                    {
                        m_temp_conversion= M_FAHRENEIT;
                        q_temp_conversion= Q_FAHRENHEIT;
                        UART_PutString("Temperature data format: Fahrenheit\r\n\n");
                    }
                    /* Pointer resetted at the first available cell (0x0007)*/
                    Pointer_resetter();
                    break;
                default:
                    break;
            }
                option_table= DONT_SHOW_TABLE;
                feature_selected = 0;
                //KeysMenu=0;
                while_working_menu_flag = SHOW_MENU;
                change_settings_flag = 0;
        }
            
        
        if(EEPROM_Full){
            /*set PWM period to 250 ms*/
            PWM_period = 14999;
            blue_led_PWM_behaviour(PWM_period);
        }else{
            /*set PWM period to 1 s*/
            PWM_period = 60000;
            blue_led_PWM_behaviour(PWM_period);
        }
        
        if(display_error){
            Display_Error();
            display_error = DONT_SHOW_ERROR;
        }
    


        
        if(time_counter == 5000 / (1 + Timer_ReadPeriod())){
            /*Reset the pointer if the button has been pressed for 5 seconds*/
            Pointer_resetter();
            time_counter = 0;
        }
        
    }//END FOR CYCLE
}//END MAIN


void blue_led_PWM_behaviour(uint16_t period){    
    Blue_LED_PWM_WritePeriod(period);
    /*Set duty cycle to 50%*/
    Blue_LED_PWM_WriteCompare(period/2);    
}

void Begin_Acquisition(void) {
    
    if (BeginFlag == 0) {
        Change_Accelerometer_SampFreq(EEPROM_readByte(SAMPLING_FREQUENCY_ADDRESS));
    }
    Timer_WritePeriod(timer_periods[EEPROM_readByte(SAMPLING_FREQUENCY_ADDRESS)-1]);   
    /*Starting timer*/
    Timer_Start();
    /*Starting ADC*/
    ADC_DelSig_Start();
    /*ADC start conversion*/
    ADC_DelSig_StartConvert();
    Blue_LED_PWM_Start();
    UART_PutString("Data acquisition ON... \r\n");
    start = ACTIONS_DONE;
    
}
void Stop_Acquisition(void) {
    
    Change_Accelerometer_SampFreq(0);
    /*Stopping timer*/
    Timer_Stop();
    /*Stopping ADC*/
    ADC_DelSig_Stop();
    Blue_LED_PWM_Stop();
    uint8_t InterruptStatus;
    InterruptStatus=CyEnterCriticalSection();
    EEPROM_writeByte(BEGIN_STOP_ADDRESS, STOP);
    EEPROM_waitForWriteComplete();
    CyExitCriticalSection(InterruptStatus);
    UART_PutString("Data acquisition OFF \r\n");
    start = ACTIONS_DONE;
    
}
     

void Pointer_resetter(){
        char message[100];
        Pointer = FIRST_FREE_CELL;
        
        uint8_t InterruptStatus;
        InterruptStatus=CyEnterCriticalSection();
        EEPROM_writeByte(POINTER_ADDRESS_H,(Pointer & 0xFF00) >> 8);
        EEPROM_waitForWriteComplete();
        EEPROM_writeByte(POINTER_ADDRESS_L,(Pointer & 0xff));
        EEPROM_waitForWriteComplete();
        CyExitCriticalSection(InterruptStatus);
        sprintf(message,"pointer resetted at %x\r\n",EEPROM_readByte(POINTER_ADDRESS_L));
        UART_PutString(message);
}

void Clear_Fifo(void) {
    
    uint8_t fifo_ctrl_reg;
    ErrorCode error;
    
    fifo_ctrl_reg = FIFO_CTRL_REG_CONTENT & 0x7f;

    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         FIFO_CTRL_REG_ADDR,
                                         fifo_ctrl_reg);

    if (error == ERROR)

    {
        UART_PutString("Error occurred during I2C comm to clear fifo \r\n");   
    }
 
    fifo_ctrl_reg = FIFO_CTRL_REG_CONTENT;

    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         FIFO_CTRL_REG_ADDR,
                                         fifo_ctrl_reg);

    if (error == ERROR)
    {
        UART_PutString("Error occurred during I2C comm to set stream mode \r\n");   
    }
}

    
/* [] END OF FILE */