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
#include "ConfigurationMenu.h"

char message [LENGTH_MEX];

/* Menu dislpayed to the user while the system is running. Actual configuration of the accelerometer 
* \and temperature sensor are shown. Keyboard commands to change settings for the acquisition,
* \to visualize data or to stop the visualization of data are indicated
*/

void While_Working_Menu(void)
{   
    /* !!DATA TO BE INSERTED */
    
    /* if the system is acquiring data */
    if(1/* the flag start/stop is in start*/) 
    {
        sprintf(message, "Ongoing data acquisition... \r\n\n");
        UART_PutString(message);
    }
    /* if the acquisition has been stopped */
    else 
    {
        sprintf(message, "Data acquisition stopped \r\n\n");
        UART_PutString(message);
    }
        
    /* !!DATA TO BE INSERTED */ 
    
    /* Accelerometer acquisition configuration displayed */
    sprintf(message, "Current accelerometer acquisition configuration:\r\nFull scale range: %d\r\nSampling freqeuncy: %d\r\n\n",1,2);
    UART_PutString(message);
    
    /* !!DATA TO BE INSERTED */ 
    
    /* Temperature sensor acquisition configuration displayed */
    sprintf(message, "Temperature sensor data saved as:\r\n%s\r\n\n", (1==1)? "Farenheit" : "Celsius");
    UART_PutString(message);
    
    /* Keyboard commands to change configuation and to handle data visualization */
    sprintf(message, "Press '?' to change any configuration setting\r\nPress 'v' to visualize data (Data acquisition will be stopped)\r\nPress 'u' to stop the visualization of data\r\n\n");
    UART_PutString(message);
}
/* [] END OF FILE */
