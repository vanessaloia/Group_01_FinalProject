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

#include "EEPROMCommunication.h"
#include "25LC256.h"
#include "MemoryCells.h"
#include "project.h"

//uint8_t Flag_Cell;
//uint16_t Pointer;
//uint8_t EEPROM_Data[EEPROM_PACKET_BYTES * (WATERMARK_LEVEL + 1)];


void EEPROM_Data_Write(void) {
    
    uint8_t i;
    char message[100];
    if(!EEPROM_Full){
        if ( Pointer < POINTER_LIMIT ) 
        {
        
            for(i = 0; i < 192; i++) {
    //            sprintf(message,"pointer = %d\r\n",Pointer);
    //            UART_PutString(message);
              /*  if(i == 0){
                    EEPROM_writePage(Pointer, &EEPROM_Data[i * SPI_EEPROM_PAGE_SIZE], 57);
                    EEPROM_waitForWriteComplete();
                    Pointer += 57;
                }else if(i == 3){
                    EEPROM_writePage(Pointer, &EEPROM_Data[185], 8);
                    EEPROM_waitForWriteComplete();
                    Pointer += 7;
                }else{
                    EEPROM_writePage(Pointer, &EEPROM_Data[57+(i-1) * SPI_EEPROM_PAGE_SIZE], SPI_EEPROM_PAGE_SIZE);
                    EEPROM_waitForWriteComplete();
                    Pointer += 64;
                }*/
               
                    EEPROM_writeByte(Pointer, EEPROM_Data[i]);
                    EEPROM_waitForWriteComplete();
                    Pointer += 1;
                }
                
                //CyDelay(10);
                //Pointer += SPI_EEPROM_PAGE_SIZE;
        }
        
    
        else 
        {
            for(i = 0; i < 60; i++){
                EEPROM_writeByte(Pointer, EEPROM_Data[i]);
                EEPROM_waitForWriteComplete();
                Pointer++;
            }
            EEPROM_Full = 1;
        }
    }
}

void EEPROM_Data_Read (void) {
    //UART_PutString("SONO IN DATA READ\r\n");
    uint8_t i;
    char message[100];
    
    
    if ( Read_Pointer < POINTER_LIMIT ) 
    {
        for (i=0; i < 192 ; i++) {
//            sprintf(message,"read pointer = %d\r\n",Read_Pointer);
//            UART_PutString(message);
            EEPROM_Data_read[i] = EEPROM_readByte(Read_Pointer);
            Read_Pointer++;
        }
    }
    
    
    else 
    {
        for(i=0;i<60;i++){
            EEPROM_Data_read[i] = EEPROM_readByte(Read_Pointer);
            Read_Pointer++;
        }
//        sprintf(message,"Read_Pointer = %d" ,Read_Pointer);
//        UART_PutString(message);
    }
   // UART_PutString("Read completed\r\n");
}

       
        
        
    
    

void EEPROM_Initialization(void) {
    char message[50];
    UART_PutString("EEPROM_init\r\n");
    Pointer = FIRST_FREE_CELL;
    EEPROM_writeByte(POINTER_ADDRESS_H,(Pointer&0xFF00)>>8);
    EEPROM_waitForWriteComplete();
    EEPROM_writeByte(POINTER_ADDRESS_L,(Pointer&0xff));
    EEPROM_waitForWriteComplete();
    EEPROM_writeByte(BEGIN_STOP_ADDRESS,0);
    EEPROM_waitForWriteComplete();
    EEPROM_writeByte(FULL_SCALE_RANGE_ADDRESS,1);
    EEPROM_waitForWriteComplete();
    EEPROM_writeByte(SAMPLING_FREQUENCY_ADDRESS,1);
    EEPROM_waitForWriteComplete();
    EEPROM_writeByte(TEMPERATURE_UNIT_ADDRESS,1);
    EEPROM_waitForWriteComplete();
    /*Writing 1 on the EEPROM first cell */
    Flag_Cell = 1;
    EEPROM_writeByte(FLAG_ADDRESS,Flag_Cell);
    EEPROM_waitForWriteComplete();
    sprintf(message,"Flag_Cell = %d\r\n",Flag_Cell);
    UART_PutString(message);
    UART_PutString("Device started up for the first time. You can change default settings through the menu \r\n");

}

void EEPROM_Store_FSR(void) {
    
    EEPROM_writeByte(FULL_SCALE_RANGE_ADDRESS, feature_selected);
    EEPROM_waitForWriteComplete();
    
}

void EEPROM_Store_Freq(void) {
    
    EEPROM_writeByte(SAMPLING_FREQUENCY_ADDRESS, feature_selected);
    EEPROM_waitForWriteComplete();
}
void EEPROM_Store_Temp(void) {
    
    EEPROM_writeByte(TEMPERATURE_UNIT_ADDRESS, feature_selected);
    EEPROM_waitForWriteComplete();
    
}



 






/* [] END OF FILE */
