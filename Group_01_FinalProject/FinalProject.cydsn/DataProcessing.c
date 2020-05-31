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
#include "DataProcessing.h"
#include "EEPROMCommunication.h"
#include "MemoryCells.h"
#include "InterruptRoutines.h"

void Digit_To_EEPROM_Conversion(void)
{
    uint8_t i;
    
    for(i = 0; i < (WATERMARK_LEVEL+1); i++) {
                EEPROM_Data[i*EEPROM_PACKET_BYTES] = Accelerations_digit[i*3]>>4;
                EEPROM_Data[i*EEPROM_PACKET_BYTES+1] = (Accelerations_digit[i*3] << 4) | (Accelerations_digit[i*3+1] >>6);
                EEPROM_Data[i*EEPROM_PACKET_BYTES+2] = (Accelerations_digit[i*3+1] << 2) | (Accelerations_digit[i*3+2] >> 8);
                EEPROM_Data[i*EEPROM_PACKET_BYTES+3] = Accelerations_digit[i*3+2];
                if (Temp_Counter > WATERMARK_LEVEL) {
                    EEPROM_Data[i*EEPROM_PACKET_BYTES+4] = Temperature_Data[i]>>8;
                    EEPROM_Data[i*EEPROM_PACKET_BYTES+5] = Temperature_Data[i];  
                }
                else {
                    EEPROM_Data[i*EEPROM_PACKET_BYTES+4] = Temperature_Data[i+WATERMARK_LEVEL]>>8;
                    EEPROM_Data[i*EEPROM_PACKET_BYTES+5] = Temperature_Data[i+WATERMARK_LEVEL];  
                }
            } 
}


void EEPROM_To_Digit_Conversion (void) 
{
    uint8_t i;
    
    for( i=0 ; i< (number_of_packets); i++ ) 
    {
        EEPROM_Data_digit[i*PACKET_DATA]= (int16_t) ((EEPROM_Data[i*EEPROM_PACKET_BYTES]<<4) | (EEPROM_Data[i*EEPROM_PACKET_BYTES +1] >>4)) ;
        EEPROM_Data_digit[i*PACKET_DATA+1] = (int16_t) ((EEPROM_Data[i*EEPROM_PACKET_BYTES+1] & 0x0f)<< 6) | (EEPROM_Data[i*EEPROM_PACKET_BYTES+2]>>2);
        EEPROM_Data_digit[i*PACKET_DATA+2] = (int16_t) ((EEPROM_Data[i*EEPROM_PACKET_BYTES+2] & 0x03)<<8) | (EEPROM_Data[i*EEPROM_PACKET_BYTES+3]);
        EEPROM_Data_digit[i*PACKET_DATA+3] = (int16_t) ((EEPROM_Data[i*EEPROM_PACKET_BYTES+4] <<8) | (EEPROM_Data[i*EEPROM_PACKET_BYTES+5]));
    }
       
}

void Digit_To_UOM_Conversion (void) 
{
    char mess[50];
    uint8_t i;
    uint8_t fsr = EEPROM_readByte(FULL_SCALE_RANGE_ADDRESS);
    uint8_t sensitivity = ((2*(1<<fsr)*1000)/ (MAX_VALUE_DIGIT)+1);
    
    for(i=0; i< (number_of_packets); i++) 
    {
        Data_UOM[i*PACKET_DATA] = EEPROM_Data_digit[i*PACKET_DATA]* sensitivity * MG_TO_MS2;
        sprintf(
        Data_UOM[i*PACKET_DATA+1] = EEPROM_Data_digit[i*PACKET_DATA+1]* sensitivity * MG_TO_MS2;
        Data_UOM[i*PACKET_DATA+2] = EEPROM_Data_digit[i*PACKET_DATA+2]* sensitivity * MG_TO_MS2;
        EEPROM_Data_digit[i*PACKET_DATA+3]= ADC_DelSig_CountsTo_mVolts(EEPROM_Data_digit[i*PACKET_DATA+3]);
        Data_UOM[i*PACKET_DATA+3] = m_temp_conversion* (EEPROM_Data_digit[i*PACKET_DATA+3]-OFFSET_mV) +q_temp_conversion;
    }
    
}
void Buffer_Creation(void)
{
    uint8_t i;
    int16_t data_int;
    
    for (i=0; i< (number_of_packets); i++)
    {
        data_int= (int)(Data_UOM[i*PACKET_DATA]*100);
        Data_Buffer[i*PACKET_BYTES] =  data_int & 0xff;
        
        data_int= (int)(Data_UOM[i*PACKET_DATA]*100);
        Data_Buffer[i*PACKET_BYTES+1] =  (data_int>>8);
        
        data_int= (int)(Data_UOM[i*PACKET_DATA+1]*100);
        Data_Buffer[i*PACKET_BYTES+2] =  data_int &0xff;
        
        data_int= (int)(Data_UOM[i*PACKET_DATA+1]*100);
        Data_Buffer[i*PACKET_BYTES+3] =  data_int>>8;
        
        data_int= (int)(Data_UOM[i*PACKET_DATA+2]*100);
        Data_Buffer[i*PACKET_BYTES+4] =  data_int & 0xff;
        
        data_int= (int)(Data_UOM[i*PACKET_DATA+2]*100);
        Data_Buffer[i*PACKET_BYTES+5] =  data_int >>8;
        
        data_int= (int)(Data_UOM[i*PACKET_DATA+3]*100);
        Data_Buffer[i*PACKET_BYTES+ 6] =  data_int & 0xff;
        
        data_int= (int)(Data_UOM[i*PACKET_DATA+3]*100);
        Data_Buffer[i*PACKET_BYTES+7] =  data_int >>8;
    }
}

void Packets_To_Send_Creation(void)
{
    uint8_t i;
    for (i=0; i< (number_of_packets); i++) 
            {
                Packet_To_Send[1] = Data_Buffer[i*PACKET_BYTES];
                
                Packet_To_Send[2] = Data_Buffer[i*PACKET_BYTES+1];
                
                Packet_To_Send[3] = Data_Buffer[i*PACKET_BYTES+2];
                
                Packet_To_Send[4] = Data_Buffer[i*PACKET_BYTES+3];
                
                Packet_To_Send[5] = Data_Buffer[i*PACKET_BYTES+4];
                
                Packet_To_Send[6] = Data_Buffer[i*PACKET_BYTES+5];
                
                Packet_To_Send[7] = Data_Buffer[i*PACKET_BYTES+ 6 ];
                
                Packet_To_Send[8] = Data_Buffer[i*PACKET_BYTES+7];
            
                UART_PutArray(Packet_To_Send, 10);
            }
            
}
   




/* [] END OF FILE */
