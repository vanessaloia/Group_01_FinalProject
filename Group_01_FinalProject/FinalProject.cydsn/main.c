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



int main(void)
{
    
    /****INITIAL EEPROM CONFIGURATION****/

    
    /*Starting I2C*/
    I2C_Master_Start();
    /*SPI start*/
    SPIM_Start();
   /*Starting UART*/
    UART_Start();
    
    isr_UART_StartEx(Custom_isr_UART);
    
    UART_PutString("\nUART Started\r\n");
    
    isr_TIMER_StartEx(Custom_isr_TIMER);
    ADC_DelSig_StartConvert();
        
    
    uint8 Flag_cell;
    uint16 pointer;
    
    Flag_cell = EEPROM_readByte(FLAG_ADDRESS);
    
        UART_PutString("Sampling frequency set at 1Hz\r\n");
        EEPROM_writeByte(TEMPERATURE_UNIT_ADDRESS,'c');
        UART_PutString("Temperature unit set at celius\r\n");
        /*Writing 1 on the EEPROM first cell */
        Flag_cell = 1;
        EEPROM_writeByte(FLAG_ADDRESS,Flag_cell);
    }
    CyGlobalIntEnable; /* Enable global interrupts. */
    if(Flag_Cell == 0) EEPROM_Initialization();
        UART_PutString("Full scale range set at +/-2g\r\n");
        EEPROM_writeByte(SAMPLING_FREQUENCY_ADDRESS,1);
        EEPROM_writeByte(FULL_SCALE_RANGE_ADDRESS,1);
        UART_PutString("Begin/stop cell set in 'stop' condition\r\n");
        EEPROM_writeByte(BEGIN_STOP_ADDRESS,0);
        UART_PutString("Pointer second registers set\r\n");
        EEPROM_writeByte(POINTER_ADDRESS_L,(pointer&0xff));
        UART_PutString("Pointer first register set\r\n");
        EEPROM_writeByte(POINTER_ADDRESS_H,(pointer&0xFF00)>>8);
        pointer = FIRST_FREE_CELL;
        /*Setting EEPROM registers at default value*/
        UART_PutString("Setting EEPROM registers at default value\r\n");    
    if(Flag_cell == 0){
 
    UART_Start();
    isr_UART_StartEx(Custom_isr_UART);
        
    change_settings_flag=0;
    option_table= DONT_SHOW_TABLE;
    initialized=0;
    feature_selected = 0;
    /* array used to change the period of the timer when the user changes the sampling frequency] */
    uint16 timer_periods[4] = { 1000, 100, 40, 20 }; 
    
    /* After tables displaying the possible setting have shown and the user 
    * / has inseerted a valid input, the accelerometer setting or temperature data 
    * / format are changed (da cambiare fa schifo)
    */
    if (option_table != DONT_SHOW_TABLE && feature_selected !=0) 
    {  
        /* data need to be deleted: the timer is stopped to not generate new data */
        Timer_Stop();

    UART_Start();
    ADC_DelSig_Start();
    ADC_DelSig_StartConvert();
    FlagReady = 0;
    start = 0;
    stop = 0;
    change_settings_flag = 1;
    option_table = DONT_SHOW_TABLE;
    initialized = 0;
    feature_selected = 0;
    KeysMenu = 0;
    display_error = 0;
    
    char message[40];
    //*/
    //DataBuffer2[0] = 0xA0;
    //DataBuffer2[7] = 0xC0;
    //isr_FIFO_StartEx(Custom_isr_FIFO);
   // Keys_menu();
    
    ShowMenuFlag = 1;
    
    *//*
    I2C_Master_Start();
    

    Timer_Start();
    isr_TIMER_StartEx(Custom_isr_TIMER);
    isr_FIFO_StartEx(Custom_isr_FIFO);
    
    uint8_t i;
    for(;;)
    {
        if (FIFODataReadyFlag && TempDataReadyFlag) {
            
            for(i = 0; i < (WATERMARK_LEVEL+1); i++) {
                EEPROM_Data[i*6] = Accelerations_digit[i*3]>>4;
                EEPROM_Data[i*6+1] = (Accelerations_digit[i*3] << 4) | (Accelerations_digit[i*3+1] >> 6);
                EEPROM_Data[i*6+2] = (Accelerations_digit[i*3+1] << 2) | (Accelerations_digit[i*3+2] >> 8);
                EEPROM_Data[i*6+3] = Accelerations_digit[i*3+2];
                if (Temp_Counter > WATERMARK_LEVEL) {
                    EEPROM_Data[i*6+4] = Temperature_Data[i]>>8;
                    EEPROM_Data[i*6+5] = Temperature_Data[i];  
                }
                else {
                    EEPROM_Data[i*6+4] = Temperature_Data[i+WATERMARK_LEVEL]>>8;
                    EEPROM_Data[i*6+5] = Temperature_Data[i+WATERMARK_LEVEL];  
                }
            }  
           
            FIFODataReadyFlag = 0;
            TempDataReadyFlag = 0;
            
            EEPROM_Data_Write();   
    
        }
        
        if(ShowMenuFlag){
            Keys_menu();
            ShowMenuFlag = 0;
            KeysMenu = 1;
        }
        /* Value of option table defines which settings have to be modified:
        * option table= FSR -> change the full scale range of the accelerometer
        * option table = SAMP_FREQ -> change the sampling frequency of the acceleromter
        * option table = TEMP -> change temprature data format
        * Depending on option table the value of feature_selected variable is used 
        * to operate the correct change on the acquisition settings */
        
        if(KeysMenu == 1){
            switch (option_table){
                case F_S_R:
                    Show_table(0);
                    KeysMenu = 0;
                break;
                case SAMP_FREQ:
                    Show_table(1);
                    KeysMenu = 0;
                break;
                case TEMP:
                    Show_table(2);
                    KeysMenu = 0;
                break;
            }
        }
        
        if(feature_selected && option_table == F_S_R){
            if(feature_selected == 1){
                //SET FSR +-2g
                UART_PutString("FSR = +/-2g\r\n");
                feature_selected = 0;
            }else if(feature_selected == 2){
                //SET FSR +-4g
                UART_PutString("FSR = +/-4g\r\n");
                feature_selected = 0;
            }else if(feature_selected == 3){
                //SET FSR +-8g
                UART_PutString("FSR = +/-8g\r\n");
                feature_selected = 0;
            }else if(feature_selected == 4){
                //SET FSR +-16g
                UART_PutString("FSR = +/-16g\r\n");
                feature_selected = 0;
            }
            ShowMenuFlag = 1; //Dysplay Keys_menu
            KeysMenu = 0;
            option_table = DONT_SHOW_TABLE;
        }else if(feature_selected && option_table == SAMP_FREQ){
            if(feature_selected == 1){
                //SET SAMPLING FREQUENCY 1Hz
                UART_PutString("Freq = 1Hz\r\n");
                feature_selected = 0;
            }else if(feature_selected == 2){
                //SET SAMPLING FREQUENCY 10Hz
                UART_PutString("Freq = 10Hz\r\n");
                feature_selected = 0;
            }else if(feature_selected == 3){
                //SET SAMPLING FREQUENCY 25Hz
                UART_PutString("Freq = 25Hz\r\n");
                feature_selected = 0;
            }else if(feature_selected == 4){
                //SET SAMPLING FREQUENCY 50Hz
                UART_PutString("Freq = 50Hz\r\n");
                feature_selected = 0;
            }
            ShowMenuFlag = 1; //Dysplay Keys_menu
            KeysMenu = 0;
            option_table = DONT_SHOW_TABLE;
        }else if(feature_selected && option_table == TEMP){
            if(feature_selected == 'c'){
                //SET TEMPERATURE UNIT AT CELSIUS
                UART_PutString("Temp in celsius\r\n");
                feature_selected = 0;
            }else if(feature_selected == 'f'){
                //SET TEMPERATURE UNIT AT FAHRENHEIT
                UART_PutString("Temp in Fahrenheit\r\n");
                feature_selected = 0;
            }
            ShowMenuFlag = 1; //Dysplay Keys_menu
            KeysMenu = 0;
            option_table = DONT_SHOW_TABLE;
        }
        
        if(start){
            /* save the value  in the EEPROM */    
            EEPROM_writeByte(BEGIN_STOP_ADDRESS, 1);
            start = 0;
        }
        
        if(stop){
            EEPROM_writeByte(BEGIN_STOP_ADDRESS, 0);
            stop = 0;
        }
        
        if(display_error){
            Display_error();
            display_error = 0;
        }
  }
//}
/*/
    While_Working_Menu();
    char v=0;
    for (;;) 
    {   *
        if (change_settings_flag) 
            v=1;
        if (v) 
        switch (option_table) 
        {
            case F_S_R:
                /* change full scale range */
                Change_Accelerometer_FSR();
            case SAMP_FREQ:
                /* change sampling freqeuncy */
                Change_Accelerometer_SampFreq();
                /* change timer frequency in order to change the fequency of the isr */
                Timer_WritePeriod(timer_periods[feature_selected-1]);
            case TEMP:
                /* to do */
        }
        option_table= DONT_SHOW_TABLE;
        feature_selected = 0;
    }
        
    
void Display_error(){
    UART_PutString("\nSelection invalid. Please choose one of the available characters\r\n");
}
}

    



    
/* [] END OF FILE */
