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

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
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
    
    uint8_t i;
    for(;;)
    {
        /* Place your application code here. */
        if (FIFODataReadyFlag && TempDataReadyFlag) {
            
            for(i = 0; i < (WATERMARK_LEVEL+1); i++) {
                EEPROM_Data[i*6] = Accelerations_digit[i*3]>>4;
                EEPROM_Data[i*6+1] = (Accelerations_digit[i*3] << 4) | (Accelerations_digit[i*3+1] >> 6);
                EEPROM_Data[i*6+2] = (Accelerations_digit[i*3+1] << 2) | (Accelerations_digit[i*3+2] >> 8);
                EEPROM_Data[i*6+3] = Accelerations_digit[i*3+2];
                EEPROM_Data[i*6+4] = Temperature_Data[i]>>8;
                EEPROM_Data[i*6+5] = Temperature_Data[i];                    
            }  
            
            //function to send data to EEPROM to be put here
            FIFODataReadyFlag = 0;
            TempDataReadyFlag = 0;
        
        if(ShowMenuFlag){
            Keys_menu();
            ShowMenuFlag = 0;
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
        {
            Keys_menu();
            //change_settings_flag=0;
            v=0;
        }
        if (option_table != DONT_SHOW_TABLE)
        {
            char message[20];
            sprintf(message,"option table:%d \n", option_table);
            UART_PutString(message);
            Show_table(option_table);
            option_table=DONT_SHOW_TABLE;
        }
        */
        
        
        
        
        
    }
//}
    
void Display_error(){
    UART_PutString("\nSelection invalid. Please choose one of the available characters\r\n");
}
/* [] END OF FILE */
