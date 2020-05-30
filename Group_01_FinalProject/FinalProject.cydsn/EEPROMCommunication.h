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
    
    extern uint16_t Pointer;
    
    extern uint8_t Flag_Cell;
    
    
    uint8_t EEPROM_Data[EEPROM_PACKET_BYTES * (WATERMARK_LEVEL + 1)];
    
    uint8_t EEPROM_Full;
    
    void EEPROM_Data_Write(void);
    void EEPROM_Initialization(void);

#endif
