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
    
    #define F 0
    #define P 1
    #define T 2
    #define DONT_SHOW_TABLE 3
    
    volatile uint8 FlagReady;
    
    CY_ISR_PROTO(Custom_isr_TIMER);



    
    
    char change_settings_flag;
    char start;
    char option_table;
    
    
    CY_ISR_PROTO(Custom_isr_UART);
    
    
    #define DATA_BYTES 6
    #define WATERMARK_LEVEL 31
    #define BYTES_READ_FROM_FIFO DATA_BYTES*(WATERMARK_LEVEL+1)
    
    CY_ISR_PROTO(Custom_isr_FIFO);
    
    extern uint8_t DataBuffer[BYTES_READ_FROM_FIFO];    
    extern uint8_t DataBuffer2[DATA_BYTES+2]; 
    extern volatile uint8_t PacketReadyFlag;
    
#endif


/* [] END OF FILE */
