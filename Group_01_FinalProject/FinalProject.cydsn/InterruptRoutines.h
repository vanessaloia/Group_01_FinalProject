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

#ifndef __INTERRUPT_ROUTINES_H
    #define __INTERRUPT_ROUTINES_H
    
    #include "cytypes.h"
    #include "stdio.h"
    
    #define OFFSET_TEMPERATURE_CELSIUS 500.0
    #define SENSITIVITY_TEMPERATURE_CELSIUS 10.0
    
    #define F_S_R 0
    #define SAMP_FREQ 1
    #define TEMP 2
    #define DONT_SHOW_TABLE 3
    
    volatile uint8 FlagReady;
    
    CY_ISR_PROTO(Custom_isr_TIMER);



    
    
    char change_settings_flag;
    char start;
    uint8_t option_table;
    uint8_t feature_selected;
    
    
    CY_ISR_PROTO(Custom_isr_UART);
    
    
    #define ACCELEROMETER_DATA_BYTES 6
    #define WATERMARK_LEVEL 31
    #define BYTES_READ_FROM_FIFO ACCELEROMETER_DATA_BYTES * (WATERMARK_LEVEL + 1)
    #define EEPROM_ACCELEROMETER_DATA_BYTES 4
    #define EEPROM_TEMPERATURE_DATA_BYTES 2
    #define EEPROM_PACKET_BYTES EEPROM_ACCELEROMETER_DATA_BYTES + EEPROM_TEMPERATURE_DATA_BYTES
    
    volatile uint8 ShowMenuFlag;

    
    CY_ISR_PROTO(Custom_isr_FIFO);
    
    /* array to store the 3 accelerations in digit(from the position zero, for 32 samples: X axis, Y axis, Z axis) */
    extern int16_t Accelerations_digit[BYTES_READ_FROM_FIFO/2];
    extern int16_t Temperature_Data[WATERMARK_LEVEL + 1];
    extern uint8_t EEPROM_Data[EEPROM_PACKET_BYTES * (WATERMARK_LEVEL + 1)];
    //extern uint8_t DataBuffer[BYTES_READ_FROM_FIFO];    
    //extern uint8_t DataBuffer2[ACCELEROMETER_DATA_BYTES+2]; 
    //extern volatile uint8_t PacketReadyFlag;
    extern volatile uint8_t FIFODataReadyFlag;   
    extern volatile uint8_t TempDataReadyFlag;
#endif


/* [] END OF FILE */
