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
    
    /* Circular counter to store the position of the array Temperature_Data in which to store new sampled data */
    extern volatile uint8_t Temp_Counter;
    
    CY_ISR_PROTO(Custom_isr_TIMER);

    char change_settings_flag;
    char start;
    volatile uint8_t stop;
    uint8_t option_table;
    uint8_t feature_selected;
    uint8_t display_error;
    
    CY_ISR_PROTO(Custom_isr_UART);
    
    volatile uint8 ShowMenuFlag;
    volatile uint8 KeysMenu;
    
    /* Number of Bytes to be read from the accelerometer corresponding to one sample (one value of acceleration for each axis) */
    #define ACCELEROMETER_DATA_BYTES 6
    /* FIFO watermark level: a number of samples equal to (WATERMARK_LEVEL + 1) are available in fifo when an interrupt request on fifo watermark level is generated */
    #define WATERMARK_LEVEL 31
    /* Number of bytes to be read from the accelerometer FIFO at each ISR on FIFO watermark level */
    #define BYTES_READ_FROM_FIFO ACCELEROMETER_DATA_BYTES * (WATERMARK_LEVEL + 1)
    /* Number of bytes to be stored in EEPROM for each accelerometer sample */
    #define EEPROM_ACCELEROMETER_DATA_BYTES 4
    /* Number of bytes to be stored in EEPROM for each temperature sample */
    #define EEPROM_TEMPERATURE_DATA_BYTES 2
    /* Number of bytes of each packet stored in EEPROM */
    #define EEPROM_PACKET_BYTES EEPROM_ACCELEROMETER_DATA_BYTES + EEPROM_TEMPERATURE_DATA_BYTES
    
    void Display_error();
    

    
    CY_ISR_PROTO(Custom_isr_FIFO);
    
    /* array to store the 3 accelerations in digit(from the position zero, for 32 samples: X axis, Y axis, Z axis) */
    extern int16_t Accelerations_digit[BYTES_READ_FROM_FIFO/2];
    
    /* Array to store 64 temperature data */
    extern int16_t Temperature_Data[(WATERMARK_LEVEL + 1) * 2];

    /* Flag to indicate that new data have been read from FIFO and must be sent to EEPROM */
    extern volatile uint8_t FIFODataReadyFlag;   

    /* Flag to indicate that 32 new temperature data are available */
    extern volatile uint8_t TempDataReadyFlag;
    
#endif


/* [] END OF FILE */
