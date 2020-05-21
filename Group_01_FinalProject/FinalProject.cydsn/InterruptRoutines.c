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

CY_ISR(Custom_isr_UART)
{
      char ch_received;
       /* character on the Rx line stored */
       ch_received= UART_GetChar();
    
       if (change_settings_flag==0)
       {
            
           switch (ch_received)
           {
            
                case '?':
                
                    /* show Configuration menu */
                
                    /* futher characters used for changing configurations of the acquisition */
                    change_settings_flag=1;
                   
                    break;
                
                case 'v': 
                case 'V':
                
                    /* show data in the bridge control panel */
                
                    /* stop acquisition and storing in the EEPROM */
                    break;
                
                case 'u':
                case 'U':
                
                    /* stop streaming of data in bridge control panel */
                    
                    
                    break;
            
                /*for every other character */
                default:
                    break;
            }
    }
    
    else {
        
        
        if(option_table==0)
        {
            switch( ch_received ) 
            {
                case 'b':
                case 'B':
                    /* start data acquisition and storage in EEPROM */
                    
                    /* change the value of the start/stop flag */
                    start=1;
                    /* save the value of the flag in the EEPROM */
                
                    break;
                
                case 's': 
                case 'S':
                    /* stop data acquisition and storage in EEPROM */
                    
                    /* change the value of the start/stop flag */
                    start=0;
                    /* save the value of the flag in the EEPROM */
                
                    break;
                
                case 'f':
                case 'F':
                
                    /* show accelerometer full scale range table */
                    option_table= ch_received;
                    break;
                
                case 'p':
                case 'P':
                    /* show accelerometer sampling frequency */
                    option_table=ch_received;
                    break;
                
                case 't':
                case 'T':
                    /* show temperature sensor unit of measurement table */
                    option_table= ch_received;
                    break;
                
                case 'q':
                case 'Q': 
                    
                    /* set flag to 0 to quit the configuration menu */
                    change_settings_flag=0;
                    break;
                /* do nothing for every other character */
                default:
                        break;
            }
        }
        
        else {
            
            /* change settings */
            option_table=0;
        }
                
    }
}
               

/* [] END OF FILE */
