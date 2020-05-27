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
#include "ConfigurationMenu.h"
#include "LIS3DH_Registers.h"
#include "I2C_Interface.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    UART_Start();
    isr_UART_StartEx(Custom_isr_UART);
    
    
    change_settings_flag=0;
    option_table= DONT_SHOW_TABLE;
    initialized=0;
    

    Show_table(0);
    char v=0;
    for (;;) 
    {   
       
        
        
        
        
        
    }
}
    
/* [] END OF FILE */
