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
<<<<<<< HEAD
#ifndef __INTERRUPT_ROUTINES_H
    #define __INTERRUPT_ROUTINES_H
    
    #include "cytypes.h"
    #include "stdio.h"
    
    #define OFFSET_TEMPERATURE_CELSIUS 500.0
    #define SENSITIVITY_TEMPERATURE_CELSIUS 10.0
    
    volatile uint8 FlagReady;
    
    CY_ISR_PROTO(CUSTOM_ISR_TIMER);
#endif
=======

#ifndef INTERRUPT_ROUTINES_H
    #define INTERRUPT_ROUTINES_H
    
    #include "cytypes.h"
    
    
    char change_settings_flag;
    char start;
    char option_table;
    
    
    CY_ISR_PROTO(Custom_isr_UART);
    
#endif

>>>>>>> 8fb6d51433da442cd79ac85e038c83e5ccd68d14
/* [] END OF FILE */
