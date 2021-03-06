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
char message[100];

/*
\brief function to convert 32 data in digit acquired from the sensors to the packet of bytes to be stored in EEPROM
*/
void Digit_To_EEPROM_Conversion(void)
{
    uint8_t i;
    
    for(i = 0; i < (WATERMARK_LEVEL+1); i++) {
        
                EEPROM_Data[i*EEPROM_PACKET_BYTES] = (Accelerations_digit[i*3]>>4)&0x3f;
                EEPROM_Data[i*EEPROM_PACKET_BYTES+1] = (Accelerations_digit[i*3] << 4) | (Accelerations_digit[i*3+1] >>6);
                EEPROM_Data[i*EEPROM_PACKET_BYTES+2] = (Accelerations_digit[i*3+1] << 2) | (Accelerations_digit[i*3+2] >> 8); 
                EEPROM_Data[i*EEPROM_PACKET_BYTES+3] = Accelerations_digit[i*3+2];
                
                if (temp_counter > WATERMARK_LEVEL) {
                    EEPROM_Data[i*EEPROM_PACKET_BYTES+4] = Temperature_Data[i]>>8;
                    EEPROM_Data[i*EEPROM_PACKET_BYTES+5] = Temperature_Data[i]; 
                }
                else {
                    EEPROM_Data[i*EEPROM_PACKET_BYTES+4] = Temperature_Data[i+WATERMARK_LEVEL]>>8;
                    EEPROM_Data[i*EEPROM_PACKET_BYTES+5] = Temperature_Data[i+WATERMARK_LEVEL]; 
                }
    } 
}
/*
\brief function to convert 192 bytes read from the eeprom to 32 packets in digit 
*/
void EEPROM_To_Digit_Conversion (void) 
{
    uint8_t i;
    
    for( i=0 ; i<(number_of_packets); i++ ) 
    {   
        x_l=(uint8_t) (EEPROM_Data_read[i*EEPROM_PACKET_BYTES+1]&0x30)<<2;
        x_h=(uint8_t) (((EEPROM_Data_read[i*EEPROM_PACKET_BYTES+1]&0xC0)>>6) |  ((EEPROM_Data_read[i*EEPROM_PACKET_BYTES]&0x3f)<<2));  
        y_l=(uint8_t) ((EEPROM_Data_read[i*EEPROM_PACKET_BYTES+2]&0x0C)<<4);  
        y_h=(uint8_t) (((EEPROM_Data_read[i*EEPROM_PACKET_BYTES+2]&0xF0)>>4) |  ((EEPROM_Data_read[i*EEPROM_PACKET_BYTES+1]&0x0F)<<4));  
        z_l=(uint8_t) ((EEPROM_Data_read[i*EEPROM_PACKET_BYTES+3]&0x03)<<6);
        z_h=(uint8_t) (((EEPROM_Data_read[i*EEPROM_PACKET_BYTES+3]&0xFC)>>2) |  ((EEPROM_Data_read[i*EEPROM_PACKET_BYTES+2]&0x03)<<6));   
        t_l=(uint8_t) ((EEPROM_Data_read[i*EEPROM_PACKET_BYTES+5]&0x03)<<6);
        t_h=(uint8_t) (((EEPROM_Data_read[i*EEPROM_PACKET_BYTES+5]&0xFC)>>2) |  ((EEPROM_Data_read[i*EEPROM_PACKET_BYTES+4]&0x03)<<6));   

        EEPROM_Data_Digit[i*PACKET_DATA]=(int16) ((x_h<<8)|(x_l))>>6;
        EEPROM_Data_Digit[i*PACKET_DATA+1]=(int16) ((y_h<<8)|(y_l))>>6;
        EEPROM_Data_Digit[i*PACKET_DATA+2]=(int16) ((z_h<<8)|(z_l))>>6;
        EEPROM_Data_Digit[i*PACKET_DATA+3]=(int16) ((t_h<<8)|(t_l))>>6;

    }
    
}

/*
\brief function to convert 32 packets in digit in 32 packets in the correct unit of measurement 
\(m/s^2 for the acceleration, celsius or farhenheit for the temperature)
*/
void Digit_To_UOM_Conversion (void) 
{
    uint8_t i;
    
    uint8_t InterruptStatus;
    InterruptStatus=CyEnterCriticalSection();
    uint8_t fsr = EEPROM_readByte(FULL_SCALE_RANGE_ADDRESS);
    CyExitCriticalSection(InterruptStatus);
    
    uint8_t sensitivity = ((2*(1<<fsr)*1000)/(MAX_VALUE_DIGIT)+1);
    
    for(i=0; i< (number_of_packets); i++) 
    {
        Data_UOM[i*PACKET_DATA] = EEPROM_Data_Digit[i*PACKET_DATA]* sensitivity * MG_TO_MS2;
        Data_UOM[i*PACKET_DATA+1] = EEPROM_Data_Digit[i*PACKET_DATA+1]* sensitivity * MG_TO_MS2;
        Data_UOM[i*PACKET_DATA+2] = EEPROM_Data_Digit[i*PACKET_DATA+2]* sensitivity * MG_TO_MS2;
        EEPROM_Data_Digit[i*PACKET_DATA+3]= ADC_DelSig_CountsTo_mVolts(EEPROM_Data_Digit[i*PACKET_DATA+3]);
        Data_UOM[i*PACKET_DATA+3] = m_temp_conversion*(EEPROM_Data_Digit[i*PACKET_DATA+3]-OFFSET_mV) +q_temp_conversion;
    }
      
}
/*
\brief function to create a buffer of 256 bytes to be sent to the bridge starting from the 32 packets 
*/
void Buffer_Creation(void)
{
    uint8_t i;
    int16_t data_int;
    
    for (i=0; i< (number_of_packets); i++)
    {
        data_int= (int)(Data_UOM[i*PACKET_DATA]*100);
        Data_Buffer[i*PACKET_BYTES] =  data_int & 0xff;
        Data_Buffer[i*PACKET_BYTES+1] =  (data_int>>8);
        
        data_int= (int)(Data_UOM[i*PACKET_DATA+1]*100);
        Data_Buffer[i*PACKET_BYTES+2] =  data_int &0xff;
        Data_Buffer[i*PACKET_BYTES+3] =  data_int>>8;
        
        data_int= (int)(Data_UOM[i*PACKET_DATA+2]*100);
        Data_Buffer[i*PACKET_BYTES+4] =  data_int & 0xff;
        Data_Buffer[i*PACKET_BYTES+5] =  data_int >>8;
        
        data_int= (int)(Data_UOM[i*PACKET_DATA+3]*100);
        Data_Buffer[i*PACKET_BYTES+6] =  data_int & 0xff;
        Data_Buffer[i*PACKET_BYTES+7] =  data_int >>8;
    }
}

/*
\brief function to create little buffer to be sent to the bridge,each containing one packet, starting from the buffer of 256 bytes
*/
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
                
                Packet_To_Send[7] = Data_Buffer[i*PACKET_BYTES+6];
                
                Packet_To_Send[8] = Data_Buffer[i*PACKET_BYTES+7];
            
                UART_PutArray(Packet_To_Send, 10);
            }
            
}
   




/* [] END OF FILE */
