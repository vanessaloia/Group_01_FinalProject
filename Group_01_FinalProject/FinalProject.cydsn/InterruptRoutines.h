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

#ifndef INTERRUPT_ROUTINES_H
    #define INTERRUPT_ROUTINES_H
    
    #include "cytypes.h"
    
    
    char change_settings_flag;
    char start;
    char option_table;
    
    
    CY_ISR_PROTO(Custom_isr_UART);
    
#endif

/* [] END OF FILE */
