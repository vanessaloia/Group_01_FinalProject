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


void EEPROM_Data_Write(void) {
    
    uint8_t i;
    
    if (Pointer < POINTER_LIMIT) {
        
        for(i = 0; i < 3; i++) { 
            
            EEPROM_writePage(Pointer, &EEPROM_Data[i * SPI_EEPROM_PAGE_SIZE], SPI_EEPROM_PAGE_SIZE);
            EEPROM_waitForWriteComplete();
            CyDelay(10);
            Pointer += SPI_EEPROM_PAGE_SIZE;
   
        }  
    }
    else {
        
        EEPROM_writePage(Pointer, EEPROM_Data , SPI_EEPROM_PAGE_SIZE);
        EEPROM_waitForWriteComplete();
        Pointer+= 64;
        EEPROM_writePage( Pointer, &EEPROM_Data[SPI_EEPROM_PAGE_SIZE], 56);
        EEPROM_waitForWriteComplete();
        Pointer += 56;
        EEPROM_Full = 1;
    }
    
    EEPROM_writeByte(POINTER_ADDRESS_H,(Pointer&0xFF00)>>8);
    EEPROM_waitForWriteComplete();
    EEPROM_writeByte(POINTER_ADDRESS_L,(Pointer&0xff));
    EEPROM_waitForWriteComplete(); 
}

void EEPROM_Data_Read (void) {
    
    uint8_t i;
   
    if (Read_Pointer < POINTER_LIMIT) {
   
        for (i=0; i < 3 ; i++) {
            EEPROM_readPage (Read_Pointer, &EEPROM_Data[i*SPI_EEPROM_PAGE_SIZE], SPI_EEPROM_PAGE_SIZE);
            Read_Pointer+= SPI_EEPROM_PAGE_SIZE;
        }
    }
    
    else {
        EEPROM_readPage(Read_Pointer, EEPROM_Data , SPI_EEPROM_PAGE_SIZE);
        Read_Pointer+= 64;
        EEPROM_readPage( Read_Pointer, &EEPROM_Data[SPI_EEPROM_PAGE_SIZE], 56);
        Pointer += 56;
    }
}


void EEPROM_Initialization(void) {
    char message[50];
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
