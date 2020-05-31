
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


void blue_led_PWM_behaviour(uint16_t);



void Begin_Acquisition(void);
void Stop_Acquisition(void);

uint8_t BeginFlag;
/* array used to change the period of the timer when the user changes the sampling frequency] */
uint16 timer_periods[4] = { 1000, 100, 40, 20 }; 

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    char message[100];
    /****INITIAL EEPROM CONFIGURATION****/
    
    uint8_t PacketReadyFlag=0;
    
    
    
    uint8_t sending_data=0;
    
    uint16_t PWM_period = 0;
    
    
     /* default temperature format to send data is Celsius */
    m_temp_conversion= M_CELSIUS;
    q_temp_conversion= Q_CELSIUS;
    
   
    Packet_To_Send[0] = 0xA0;
    Packet_To_Send[9]= 0xC0;
    
    
    
    /*Starting I2C*/
    I2C_Peripheral_Start();
    
   /*Starting UART*/
    UART_Start();
    UART_PutString("\nUART Started\r\n");
    /*SPI start*/
    SPIM_Start();
    
    isr_UART_StartEx(Custom_isr_UART);
    isr_FIFO_StartEx(Custom_isr_FIFO);
    isr_TIMER_StartEx(Custom_isr_TIMER);
    
    button_pressed = BUTTON_PRESSED;
    isr_BUTTON_StartEx(Custom_isr_BUTTON);
    
    CyDelay(10);
    

    
    Flag_Cell = EEPROM_readByte(FLAG_ADDRESS);
    sprintf(message,"Flag_Cell = %d\r\n",Flag_Cell);
    UART_PutString(message);
    
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
    
    change_settings_flag = 1;
    option_table = DONT_SHOW_TABLE;
    struct_initialized = 0;
    feature_selected = 0;
    KeysMenu = 0;
    display_error = DONT_SHOW_ERROR;
    ShowMenuFlag = SHOW_MENU;
    while_working_menu_flag = DONT_SHOW_MENU;
    EEPROM_Full = 0;
    time_counter = 0;
    
    Read_Pointer = FIRST_FREE_CELL;
    
    /* flag that is set high when the user want to visualize the data */
    display_data=DONT_DISPLAY;

   
   
    
    for(;;)
    {
        
        switch(start){
            case (START):
                if (BeginFlag == 0) {
                    EEPROM_writeByte(BEGIN_STOP_ADDRESS, START);
                    EEPROM_waitForWriteComplete();
                }
                else BeginFlag = 0;
                
                Begin_Acquisition();
            break;
            case (STOP):
                if (BeginFlag == 0) {
                    Stop_Acquisition();
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
                display_data = DONT_DISPLAY;
                break;
                
            case STOP :
                /* stop sending data throygh UART to the Brisdge Control Panel */
            
                /* display data set to DONT_DISPLAY */
                display_data = DONT_DISPLAY;
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
            while (Read_Pointer < Pointer ) 
            {
                if (Read_Pointer <POINTER_LIMIT)
                    number_of_packets = WATERMARK_LEVEL + 1;
                else 
                    number_of_packets = END_EEPROM_PACKETS;
                
                EEPROM_Data_Read();
                EEPROM_To_Digit_Conversion();
                Digit_To_UOM_Conversion ();
                Buffer_Creation();
                PacketReadyFlag=1;
            }
            
        }
        
        if( PacketReadyFlag)
        {
            Packets_To_Send_Creation();
            
            PacketReadyFlag=0;    
            
        }
        
        if(while_working_menu_flag){
            While_Working_Menu();
            while_working_menu_flag = DONT_SHOW_MENU;
        }
        if(ShowMenuFlag){
            Keys_menu();
            ShowMenuFlag = DONT_SHOW_MENU;
            KeysMenu = 1;
        }
        
        if(KeysMenu == 1){
            if(option_table != DONT_SHOW_TABLE){
                Show_table(option_table);
                KeysMenu = 0;
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
                /* data need to be deleted: the timer is stopped to not generate new data */
                Timer_Stop();
                
                case F_S_R:
                    /* change full scale range and store it in EEPROM*/
                    EEPROM_Store_FSR();
                    Change_Accelerometer_FSR(feature_selected);
                    /* Pointer resetted at the first available cell (0x0007)*/
                    Pointer_resetter();
                   break;
                case SAMP_FREQ:
                    /* change sampling freqeuncy */
                    EEPROM_Store_Freq();
                    Change_Accelerometer_SampFreq(feature_selected);
                    /* change timer frequency in order to change the fequency of the isr */
                    Timer_WritePeriod(timer_periods[feature_selected-1]);
                    /* Pointer resetted at the first available cell (0x0007)*/
                    Pointer_resetter();
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
            Display_error();
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
    start = BYTE_SAVED;
    
}
void Stop_Acquisition(void) {
    
    Change_Accelerometer_SampFreq(0);
    /*Stopping timer*/
    Timer_Stop();
    /*Stopping ADC*/
    ADC_DelSig_Stop();
    Blue_LED_PWM_Stop();
    EEPROM_writeByte(BEGIN_STOP_ADDRESS, STOP);
    EEPROM_waitForWriteComplete();
    start = BYTE_SAVED;
    
}
     

void Pointer_resetter(){
        char message[100];
        Pointer = FIRST_FREE_CELL;
        EEPROM_writeByte(POINTER_ADDRESS_H,(Pointer & 0xFF00) >> 8);
        EEPROM_waitForWriteComplete();
        EEPROM_writeByte(POINTER_ADDRESS_L,(Pointer & 0xff));
        EEPROM_waitForWriteComplete();
        sprintf(message,"pointer resetted at %x\r\n",EEPROM_readByte(POINTER_ADDRESS_L));
        UART_PutString(message);
}

    
/* [] END OF FILE */