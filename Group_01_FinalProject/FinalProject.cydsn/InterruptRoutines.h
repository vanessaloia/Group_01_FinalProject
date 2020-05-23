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
    
    volatile uint8 FlagReady;
    
    CY_ISR_PROTO(Custom_isr_TIMER);



    
    
    char change_settings_flag;
    char start;
    char option_table;
    
    
    CY_ISR_PROTO(Custom_isr_UART);
    
    CY_ISR_PROTO(Custom_isr_FIFO);
    
#endif


/* [] END OF FILE */
