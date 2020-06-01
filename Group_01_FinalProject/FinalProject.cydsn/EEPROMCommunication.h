
/*file EEPROMCommunication.h*/

#ifndef _EEPROMCOMMUNICATION_H_
    
    #define _EEPROMCOMMUNICATION_H_
    
    #include "cytypes.h"
    #include "25LC256.h"
    #include "InterruptRoutines.h"
    /* Value of the pointer at which only 64 memory cells are still available */
    #define POINTER_LIMIT 32704
    /* Number of bytes read or written in the eeprom at once */
    #define BYTES_RW_EEPROM (EEPROM_PACKET_BYTES)*(WATERMARK_LEVEL + 1)
    
    /* Variable to track the first free cell in the eeprom available to be written */
    uint16_t Pointer;
    /* Variable to track if the device is started up for the the first time or not */
    uint8_t Flag_Cell;
    /* Array of bytes written in the the eeprom */
    uint8_t EEPROM_Data[BYTES_RW_EEPROM];
    /* Array of bytes read from the eeprom */
    uint8_t EEPROM_Data_read[BYTES_RW_EEPROM];
    /* Flag to indicate that the eeprom is full */
    uint8_t EEPROM_Full;
    /* Variable to track the cell in the memory to be read*/
    uint16_t Read_Pointer;
    

    void EEPROM_Data_Write(void);
    void EEPROM_Data_Read (void);
    void EEPROM_Initialization(void);
    void EEPROM_Store_FSR(void);
    void EEPROM_Store_Freq(void);
    void EEPROM_Store_Temp(void);


#endif
