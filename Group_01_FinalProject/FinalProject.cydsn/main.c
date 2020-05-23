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
#include "project.h"
#include "InterruptRoutines.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    UART_Start();
    isr_UART_StartEx(Custom_isr_UART);
    
    
    change_settings_flag=0;
    option_table=0;
    
    

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    Timer_Start();
    UART_Start();
    ADC_DelSig_Start();
    
    isr_TIMER_StartEx(Custom_isr_TIMER);
    ADC_DelSig_StartConvert();
    FlagReady = 0;
    
    
    isr_FIFO_StartEx(Custom_isr_FIFO);
    for(;;)
    {
        /* Place your application code here. */
        
    }
}

/* [] END OF FILE */
