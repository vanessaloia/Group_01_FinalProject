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
#include "OptionsToBeDisplayed.h"
#include "25LC256.h"
#include "MemoryCells.h"

char message [LENGTH_MEX];


 

/* Menu displayed to the user while the system is running. Actual configuration of the accelerometer 
* \and temperature sensor are shown. Keyboard commands to change settings for the acquisition,
* \to visualize data or to stop the visualization of data are indicated
*/

void While_Working_Menu(void)
{   
    
    Menu_edge();
    /* !!DATA TO BE INSERTED */
    UART_PutString("Press '?' to change the configuration settings for data acquisition\r\n");
    
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
    
   
    
    Menu_edge();
    
}

/* when the user presses 'v' this message is displayed to inform to switch to Bridge control panel to visualize the acquired data */
void Switch_to_BridgeControlPanel(void)
{
    Menu_edge();
    /* !! DATA TO BE INSERTED */
    sprintf(message, "Data will be displayed in %d seconds. Open Bridge Control Panel\n\n",1);
    UART_PutString(message);
    Menu_edge();
}

/* Function that displays the correct table depending on which parameter the user wants to change.
* \This information is contained in option_table global variable The first time that the function is called 
* \the array set_of tables is initialized.
* \ set_of_tables[0] contains the struct with the information to display related with the accelerometer full scale range options
* \ set_of tables[1] contains the struct with the information to display reòated with the accelerometer sampling frequency options
* \ set_of tables[2] contains the struct with the information to display related with the unit of measurement of the temperature sensor data
* \ A pointer is defined and associanted with the first cell memory of the variable header1 of the correct element of th set_of tables array.
* \ The elements of the struct are printed though a foor loop using the pointer.
*/

void Show_table(uint8_t index_table) 
{   
    
    uint8_t i;
    uint8_t start;
    uint8_t stop;
    
    /* global variable "initialized" is set in main.c to 0. The first time that the function is called the array set_of_tables
    * / is initialized and the variable "initialized" is set to 1
    */
    if( !struct_initialized) {
        options_to_display FSR = {"Character", "Full scale range",'1', "+/- 2g", '2', "+/- 4g",'3', "+/- 8g", '4', "+/- 16g"};
        options_to_display SampFreq = {"Character", "Sampling Frequency",'1', "1 Hz", '2', "10 Hz",'3', "25 Hz", '4', "50 Hz"};
        options_to_display TempFormat = {"Character","Temperature format",'c', "Celsius", 'f', "Fahrenheit",' ' , " ",' ' , " "};
        set_of_tables[0]= FSR;
        set_of_tables[1]= SampFreq;
        set_of_tables[2]= TempFormat;
        struct_initialized=1;
    }
    /* pointer variable "index" points the memory cell of the first element of the struct related with the correct element of set_of_tables array */
    char *index= (set_of_tables[index_table].header1);
    
    
    /* start variable is used in the for loop to define the initial memory cell pointed. 
    * \ header1 and header2 are printed before to enter in the for loop. The first cell to point is 2*WORD_SIZE (dimension of the header strings) cells after the cell memory 
    * \pointed by index
    */
    
    start= 2* WORD_SIZE;
    
    /* stop variable is used in the for loop to define the last memory cell pointed in the last cycle.
    * \ if the paramter to change is the temperature data format just to options have to be displayed.
    * \ So, in this case the last cell to be pointed is after 1+WORD_SIZE (key1 size+ option1 size).
    * \ In the other cases, the table is composed by 4 options so the last cell memory to be pointed in the for loop is (1+WORD_SIZE)*3
    */
    stop= (option_table == TEMP)? (start +(1+ WORD_SIZE +1)): (start + (1+WORD_SIZE)*3+1);
    Menu_edge();
    
    
    /* header1 and header2 of the correct table sent through UART */
    sprintf(message,"   %s | %s\r\n", index, index+WORD_SIZE);
    UART_PutString(message);
    
    /* in each cycle a row is printed (n-key = character) +( n-option = string) */
    for (i=start; i<stop; i+= (1+ WORD_SIZE))
    {
        sprintf(message, "       %c     |   %s\r\n", *(index+i) , index+i+1);
        UART_PutString( message);
    }  
    Menu_edge();
}
void Keys_menu (void) 
{
    Menu_edge();
     /* Accelerometer acquisition configuration displayed */
    
    sprintf(message, "Current accelerometer acquisition configuration:\r\nFull scale range: %d\r\n",EEPROM_readByte(FULL_SCALE_RANGE_ADDRESS));
    UART_PutString(message);
    sprintf(message, "Sampling frequency: %d\r\n",EEPROM_readByte(SAMPLING_FREQUENCY_ADDRESS));
    UART_PutString(message);
    /* !!DATA TO BE INSERTED */ 
    
    /* Temperature sensor acquisition configuration displayed */
    sprintf(message, "Temperature sensor data format:\r\n%s\r\n\n", (EEPROM_readByte(TEMPERATURE_UNIT_ADDRESS)==2)? "Fahrenheit" : "Celsius");
    UART_PutString(message);
    
    /* Keyboard commands to change configuation and to handle data visualization */
    
    UART_PutString("Press 'b' to start data acquisition and storage\r\n\n");
    
    UART_PutString("Press 's' to stop data acquisition and storage\r\n\n");
    
    UART_PutString("Press 'f' to change the accelerometer full scale range.Previous stored data will be deleted\r\n\n");
    
    UART_PutString("Press 'p' to change the accelerometer sampling frequency.Previous data will be deleted\r\n\n");
    
    UART_PutString("Press 't' to change the temperature data format\r\n\n");
    
    UART_PutString("Press 'q' to quit the menu\r\n\n");
    
    UART_PutString("Press 'v' to visualize data (Data acquisition will be stopped)\r\n\n");
    
    UART_PutString("Press 'u' to stop the visualization of data\r\n\n");
    Menu_edge();
}

void Menu_edge(void)
{
    UART_PutString("\r\n     ************************\r\n\n");
}
    
    
    
    
    
    
    
    
    
    
    
    
    

/* [] END OF FILE */
