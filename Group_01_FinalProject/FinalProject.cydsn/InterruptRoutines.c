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
#include "InterruptRoutines.h"
#include "project.h"

int32 temperature_digit = 0;
int32 temperature_mv = 0;
float temperature_celsius = 0;
char message[20];
CY_ISR(CUSTOM_ISR_TIMER){
    Timer_ReadStatusRegister();
    
    temperature_digit = ADC_DelSig_Read32();
    
    if(temperature_digit < 0) temperature_digit = 0;
    if(temperature_digit > 65535) temperature_digit = 65535;
    
    temperature_mv = ADC_DelSig_CountsTo_mVolts(temperature_digit);
    
    temperature_celsius = (temperature_mv - OFFSET_TEMPERATURE_CELSIUS)/SENSITIVITY_TEMPERATURE_CELSIUS;

    sprintf(message,"%d\r\n",(int)temperature_celsius);
    UART_PutString(message);
    
}
=======

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


               

>>>>>>> 8fb6d51433da442cd79ac85e038c83e5ccd68d14
/* [] END OF FILE */
