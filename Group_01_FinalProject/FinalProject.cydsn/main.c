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
    
    uint8 Flag_cell;
    uint16 pointer;
    
    if(Flag_cell == 0){
        /*Setting registers at default value*/
        pointer = FIRST_FREE_CELL;
        EEPROM_writeByte(POINTER_ADDRESS_H,(pointer&0xFF00)>>8);
        EEPROM_writeByte(POINTER_ADDRESS_L,(pointer&0xff));
        EEPROM_writeByte(BEGIN_STOP_ADDRESS,0);
        EEPROM_writeByte(FULL_SCALE_RANGE_ADDRESS,1);
        EEPROM_writeByte(SAMPLING_FREQUENCY_ADDRESS,1);
        EEPROM_writeByte(TEMPERATURE_UNIT_ADDRESS,'c');
        Flag_cell = 1;
    }
 
    UART_Start();
    isr_UART_StartEx(Custom_isr_UART);
        
    change_settings_flag=0;
    option_table= DONT_SHOW_TABLE;
    initialized=0;
    feature_selected = 0;

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    Timer_Start();
    UART_Start();
    ADC_DelSig_Start();
    
    isr_TIMER_StartEx(Custom_isr_TIMER);
    ADC_DelSig_StartConvert();
    FlagReady = 0;
    
    *//*
    I2C_Master_Start();
    

    
    */
    //DataBuffer2[0] = 0xA0;
    //DataBuffer2[7] = 0xC0;
    //isr_FIFO_StartEx(Custom_isr_FIFO);
    
    
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
           
//            if (PacketReadyFlag) {
//                for (i=0; i<WATERMARK_LEVEL+1; i++) {
//                  DataBuffer2[1]=DataBuffer[6*i];  
//                  DataBuffer2[2]=DataBuffer[6*i+1]; 
//                  DataBuffer2[3]=DataBuffer[6*i+2];  
//                  DataBuffer2[4]=DataBuffer[6*i+3];  
//                  DataBuffer2[5]=DataBuffer[6*i+4];  
//                  DataBuffer2[6]=DataBuffer[6*i+5];    
//                  UART_PutArray(DataBuffer2, 8); /*API to transmit an array of bytes */
//        }
//        PacketReadyFlag = 0;
//        
//    }
        }
        
        if(ShowMenuFlag){
            While_Working_Menu();
            ShowMenuFlag = 0;
        }
        
        if(feature_selected && option_table == F_S_R){
            if(feature_selected == 1){
                //SET FSR +-2g
                feature_selected = 0;
            }else if(feature_selected == 2){
                //SET FSR +-4g
                feature_selected = 0;
            }else if(feature_selected == 3){
                //SET FSR +-8g
                feature_selected = 0;
            }else if(feature_selected == 4){
                //SET FSR +-16g
                feature_selected = 0;
            }
        }else if(feature_selected && option_table == SAMP_FREQ){
            if(feature_selected == 1){
                //SET SAMPLING FREQUENCY 1Hz
                feature_selected = 0;
            }else if(feature_selected == 2){
                //SET SAMPLING FREQUENCY 10Hz
                feature_selected = 0;
            }else if(feature_selected == 3){
                //SET SAMPLING FREQUENCY 25Hz
                feature_selected = 0;
            }else if(feature_selected == 4){
                //SET SAMPLING FREQUENCY 50Hz
                feature_selected = 0;
            }
        }else if(feature_selected && option_table == TEMP){
            if(feature_selected == 'c'){
                //SET TEMPERATURE UNIT AT CELSIUS
                feature_selected = 0;
            }else if(feature_selected == 'f'){
                //SET TEMPERATURE UNIT AT FAHRENHEIT
                feature_selected = 0;
            }
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
    
/* [] END OF FILE */
