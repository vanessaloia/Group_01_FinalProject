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

float m_temp_conversion;
float q_temp_conversion;


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
    
    if(Flag_cell == 0){
        UART_PutString("Setting EEPROM registers at default value\r\n");    
        /*Setting EEPROM registers at default value*/
        pointer = FIRST_FREE_CELL;
        EEPROM_writeByte(POINTER_ADDRESS_H,(pointer&0xFF00)>>8);
        UART_PutString("Pointer first register set\r\n");
        EEPROM_writeByte(POINTER_ADDRESS_L,(pointer&0xff));
        UART_PutString("Pointer second registers set\r\n");
        EEPROM_writeByte(BEGIN_STOP_ADDRESS,0);
        UART_PutString("Begin/stop cell set in 'stop' condition\r\n");
        EEPROM_writeByte(FULL_SCALE_RANGE_ADDRESS,1);
        UART_PutString("Full scale range set at +/-2g\r\n");
        EEPROM_writeByte(SAMPLING_FREQUENCY_ADDRESS,1);
        UART_PutString("Sampling frequency set at 1Hz\r\n");
        EEPROM_writeByte(TEMPERATURE_UNIT_ADDRESS,'c');
        UART_PutString("Temperature unit set at celius\r\n");
        /*Writing 1 on the EEPROM first cell */
        Flag_cell = 1;
        EEPROM_writeByte(FLAG_ADDRESS,Flag_cell);
    }
    CyGlobalIntEnable; /* Enable global interrupts. */
 
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

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    Timer_Start();
    UART_Start();
    ADC_DelSig_Start();
    
    isr_TIMER_StartEx(Custom_isr_TIMER);
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
    
    *//*
    I2C_Master_Start();
    


    uint8_t EEPROM_Data[EEPROM_PACKET_BYTES * (WATERMARK_LEVEL + 1)];
    isr_TIMER_StartEx(Custom_isr_TIMER);
    isr_FIFO_StartEx(Custom_isr_FIFO);
    
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
            
            //function to send data to EEPROM to be put here
            FIFODataReadyFlag = 0;
            TempDataReadyFlag = 0;
    
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
        if (option_table!= DONT_SHOW_TABLE && feature_selected) {
            switch (option_table) 
            {
                case F_S_R:
                    /* change full scale range */
                    Change_Accelerometer_FSR();
                    break;
                case SAMP_FREQ:
                    /* change sampling freqeuncy */
                    Change_Accelerometer_SampFreq();
                    /* change timer frequency in order to change the fequency of the isr */
                    Timer_WritePeriod(timer_periods[feature_selected-1]);
                    break;
                case TEMP:
                    /* to do */
                    break;
                default:
                    break;
                    
            }
            option_table= DONT_SHOW_TABLE;
            feature_selected = 0;
            KeysMenu=0;
            ShowMenuFlag=1;
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
            }
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
        
    
void Display_error(){
    UART_PutString("\nSelection invalid. Please choose one of the available characters\r\n");
}
}

    



    
/* [] END OF FILE */
