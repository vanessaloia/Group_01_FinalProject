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
/* [] END OF FILE */
