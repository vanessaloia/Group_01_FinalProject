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

/* [] END OF FILE */

#ifndef _EEPROMCOMMUNICATION_H_
    
    #define _EEPROMCOMMUNICATION_H_
    
    #include "cytypes.h"
    #include "25LC256.h"
    #include "InterruptRoutines.h"
    
    #define POINTER_LIMIT 32704
    #define BYTES_RW_EEPROM (EEPROM_PACKET_BYTES)*(WATERMARK_LEVEL + 1)
    
    uint16_t Pointer;
    
    uint8_t Flag_Cell;
    
    uint8_t EEPROM_Data[BYTES_RW_EEPROM];
    
    uint8_t EEPROM_Data_read[BYTES_RW_EEPROM];
    
    uint8_t EEPROM_Full;
    
    uint16_t Read_Pointer;
    

    void EEPROM_Data_Write(void);
    void EEPROM_Data_Read (void);
    void EEPROM_Initialization(void);
    void EEPROM_Store_FSR(void);
    void EEPROM_Store_Freq(void);
    void EEPROM_Store_Temp(void);


#endif
