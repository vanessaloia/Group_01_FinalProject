
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
/* Data from the temperature sensors have to be converted in Celsius as:
* /             (value_mv - 500)* 0.1 
* /macros OFFSET_MV, M_CELSIUS, Q_CELSIUS defined to do the conversion
*/
#define M_CELSIUS 0.1
#define Q_CELSIUS 0
#define OFFSET_mV 500
/* Conversion from Celsius to Fahrenheit is (°C * 1.8) + 32
* \So the conversion from the value in mv from the temperature sensor is
* \     (value_mv -500) *0.18 +32
* \ macro defined to do the conversion
*/
#define M_FAHRENEIT 0.18
#define Q_FAHRENHEIT 32

#define START 1
#define STOP 2
#define BYTE_SAVED 0

float m_temp_conversion;
float q_temp_conversion;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    char message[100];
    /****INITIAL EEPROM CONFIGURATION****/
    
    
    
    /*Starting I2C*/
    I2C_Peripheral_Start();
    
   /*Starting UART*/
    UART_Start();
    UART_PutString("\nUART Started\r\n");
    /*SPI start*/
    SPIM_Start();
    
    Timer_Start();
    
    ADC_DelSig_Start();
    
    isr_UART_StartEx(Custom_isr_UART);
    isr_FIFO_StartEx(Custom_isr_FIFO);
    isr_TIMER_StartEx(Custom_isr_TIMER);
    
    CyDelay(10);
    
    ADC_DelSig_StartConvert();
    
    Accelerometer_Configuration();
    
    Flag_Cell = EEPROM_readByte(FLAG_ADDRESS);
    sprintf(message,"Flag_Cell = %d\r\n",Flag_Cell);
    UART_PutString(message);
    
    if (Flag_Cell == 0){
        EEPROM_Initialization();
    }
    else{
        UART_PutString("EEPROM already initialized");
    }
    /* array used to change the period of the timer when the user changes the sampling frequency] */
    uint16 timer_periods[4] = { 1000, 100, 40, 20 }; 
    
    //FlagReady = 0;
    start = BYTE_SAVED;
    //stop = 0;
    change_settings_flag = 1;
    option_table = DONT_SHOW_TABLE;
    feature_selected = 0;
    KeysMenu = 0;
    display_error = DONT_SHOW_ERROR;
    ShowMenuFlag = SHOW_MENU;
    while_working_menu_flag = DONT_SHOW_MENU;
    EEPROM_Full = 0;
    struct_initialized = 0;
    
    uint8_t i;
    
     /* default temperature format to send data is Celsius */
    m_temp_conversion= M_CELSIUS;
    q_temp_conversion= Q_CELSIUS;
    
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
                default:
                break;
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
                    /* change full scale range */
                    EEPROM_Store_FSR();
                    Change_Accelerometer_FSR();
                   break;
                case SAMP_FREQ:
                    /* change sampling freqeuncy */
                    EEPROM_Store_Freq();
                    Change_Accelerometer_SampFreq();
                    /* change timer frequency in order to change the fequency of the isr */
                    Timer_WritePeriod(timer_periods[feature_selected-1]);
                    break;
                case TEMP:
                    /* to do */
                    EEPROM_Store_Temp();
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
            start = 0;
//        if(start == START){
//            /* save the value  in the EEPROM */    
//            EEPROM_writeByte(BEGIN_STOP_ADDRESS, 1);
//            EEPROM_waitForWriteComplete();
//            start = 0;
//        }if(stop){
//            EEPROM_writeByte(BEGIN_STOP_ADDRESS, 0);
//            EEPROM_waitForWriteComplete();
//            stop = 0;
//        }
        
        switch(start){
            case (START):
                EEPROM_writeByte(BEGIN_STOP_ADDRESS, 1);
                EEPROM_waitForWriteComplete();
                start = BYTE_SAVED;
            break;
            case (STOP):
                EEPROM_writeByte(BEGIN_STOP_ADDRESS, 1);
                EEPROM_waitForWriteComplete();
                start = BYTE_SAVED;
            break;
        }
        
        
        if(EEPROM_Full){
            Red_LED_Write(1);
        }else{
            Red_LED_Write(0);
        }
        
        if(display_error){
            Display_error();
            display_error = 0;
        }
        
        
        
    }/*END FOR*/
    
}/*END MAIN*/
        
    
void Display_error(){
    UART_PutString("\nSelection invalid. Please choose one of the available characters\r\n");
}


    




    
/* [] END OF FILE */