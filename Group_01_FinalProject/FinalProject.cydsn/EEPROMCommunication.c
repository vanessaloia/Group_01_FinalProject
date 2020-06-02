/* file EEPROMCommunication.c */

/* storage and recovery of data in memory is done 32 packets at a time, corresponding to 192 bytes (3 pages)
\ the first 6 cells are busy to store configuration informations. 
\ To write in memory always an integer multiple of one packet (made of 6 bytes) and to avoid to lose the first packets, 
\ data are written starting from the cell 0x40 (value defined in FIRST_FREE_CELL), so 192 bytes are written 170 times through 3 write pages. 
\ after these 170 write (when the Pointer reach the value defined in POINTER_LIMIT), only one page remains available, so only 10 packets (60 bytes) are stored. 
*/
#include "EEPROMCommunication.h"
#include "25LC256.h"
#include "MemoryCells.h"
#include "project.h"

/*
\ brief function to write in the eeprom
\ this function write 192 bytes (or 60 if the end of the eeprom is reached) every time 32 new packets are acquired 
\ when the end of the memory is reached the flag EEPROMFull is raised
*/
void EEPROM_Data_Write(void) {
    
    uint8_t i;
    uint8_t InterruptStatus;
    InterruptStatus=CyEnterCriticalSection();
    
    if (!EEPROM_Full) {
        
        if (Pointer  < POINTER_LIMIT) {
            
            for(i = 0; i < 192; i++) { 
                
                EEPROM_writeByte(Pointer, EEPROM_Data[i]);
                EEPROM_waitForWriteComplete();
                Pointer++;
       
            }  
        }
        else {
            for(i = 0; i < 60; i++) {
                EEPROM_writeByte(Pointer, EEPROM_Data[i]);                
                EEPROM_waitForWriteComplete();
                Pointer++;
            }
            EEPROM_Full=1;
        }
    
        EEPROM_writeByte(POINTER_ADDRESS_H,(Pointer&0xFF00)>>8);
        EEPROM_waitForWriteComplete();
        EEPROM_writeByte(POINTER_ADDRESS_L,(Pointer&0xff));
        EEPROM_waitForWriteComplete(); 
    }
    
    CyExitCriticalSection(InterruptStatus);
}

/*
\ brief function to read from the eeprom
\ this function read 192 bytes (or 60 if the end of the eeprom is reached) 
*/
void EEPROM_Data_Read (void) {

    uint8_t i;
    uint8_t InterruptStatus;
    InterruptStatus=CyEnterCriticalSection();
   
    if (Read_Pointer < POINTER_LIMIT) {
   
        for (i=0; i < 192 ; i++) {
            EEPROM_Data_read[i]=EEPROM_readByte(Read_Pointer);
            Read_Pointer++;
        }
    }
    
    else {
        
        for(i = 0; i < 60; i++) {
        EEPROM_Data_read[i] = EEPROM_readByte(Read_Pointer);
        Read_Pointer++;
        }
    }
    
    CyExitCriticalSection(InterruptStatus); 
}

/*
\brief function to initialize the memory cells to store settings
\this function is called only once at the first device start up
*/
void EEPROM_Initialization(void) {
    
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
    UART_PutString("Device started up for the first time. You can change default settings through the menu \r\n");

}

/*
\brief function to store new FSR setting in the eeprom when the user change it
*/
void EEPROM_Store_FSR(void) {
    
    uint8_t InterruptStatus;
    InterruptStatus=CyEnterCriticalSection();
    
    EEPROM_writeByte(FULL_SCALE_RANGE_ADDRESS, feature_selected);
    EEPROM_waitForWriteComplete();
    
    CyExitCriticalSection(InterruptStatus);
}
/*
\brief function to store new frequency setting in the eeprom when the user change it
*/
void EEPROM_Store_Freq(void) {
    
    uint8_t InterruptStatus;
    InterruptStatus=CyEnterCriticalSection();
    
    EEPROM_writeByte(SAMPLING_FREQUENCY_ADDRESS, feature_selected);
    
    EEPROM_waitForWriteComplete();
    
    CyExitCriticalSection(InterruptStatus);
}
/*
\brief function to store new temperature unit setting in the eeprom when the user change it
*/
void EEPROM_Store_Temp(void) {
    
    uint8_t InterruptStatus;
    InterruptStatus=CyEnterCriticalSection();
    
    EEPROM_writeByte(TEMPERATURE_UNIT_ADDRESS, feature_selected);
    EEPROM_waitForWriteComplete();
    
    CyExitCriticalSection(InterruptStatus);
    
}



 






/* [] END OF FILE */
