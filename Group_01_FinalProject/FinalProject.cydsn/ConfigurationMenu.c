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
#include "25LC256.h"
#include "MemoryCells.h"

char message [LENGTH_MEX];

/* Menu displayed when the system is running. 
* \Possible options are:
* \- go to the Keys_Menu to change acquisition settings
* \- visualize data
* \- interrupt visualization of data 
*/
void While_Working_Menu(void)
{   
    /* layout of the menu */
    Menu_Edge();
    
    /* if the system is acquiring data */
    if(begin_pressed == START) 
    {
        UART_PutString("Ongoing data acquisition... \r\n\n");
    }
    /* if the acquisition has been stopped */
    else if (begin_pressed == STOP)
    {
        UART_PutString("Data acquisition stopped \r\n\n");
    }
    UART_PutString("Press '?' to change the configuration settings for data acquisition\r\n");
    UART_PutString("Press 'v' to send data to the bridge control panel\r\n");
    UART_PutString("Press 'u' to stop sending data to the bridge control panel\r\n");
 
    /* layout of the menu */
    Menu_Edge();
    
}

/* When the user presses 'v' this message is displayed to inform to switch to Bridge control panel to visualize the acquired data */
void Switch_to_BridgeControlPanel(void)
{   
    /* layout of the menu */
    Menu_Edge();
    UART_PutString("Open Bridge Control Panel to visualize data.\r\n");
    UART_PutString("Press 'u' to stop the visualization and to go to the initial menu.\r\n\n");
    
    /* layout of the menu */
    Menu_Edge();
}

/* Function that displays the correct table depending on which parameter the user wants to change.
* \ This information is given as input to the function.
* \ set_of_tables[0] contains the struct with the information to display related with the accelerometer full scale range options
* \ set_of tables[1] contains the struct with the information to display reòated with the accelerometer sampling frequency options
* \ set_of tables[2] contains the struct with the information to display related with the unit of measurement of the temperature sensor data
* \ A pointer is initialized to "enter" inside the struct variables 
*/

void Show_Table(uint8_t index_table) 
{   
    
    uint8_t i;
    uint8_t start_loop;
    uint8_t stop_loop;
    
    
    /* pointer variable "index" points "header1" variable of the correct element of set_of_tables array */
    char *index= (set_of_tables[index_table].header1);
    
    
    /* start variable is used in the for loop to define the initial memory cell pointed. 
    * \ header1 and header2 are printed before to enter in the for loop. The first cell to point is 2*WORD_SIZE (dimension of the strings) cells after the cell memory 
    * \pointed by index
    */
    start_loop= 2* WORD_SIZE;
    
    /* stop variable is used in the for loop to define the last memory cell pointed in the last cycle.
    * \ if the paramter to change is the temperature data format just two options have to be displayed.
    * \ So, in this case the last cell to be pointed is after 1+WORD_SIZE (key1 size+ option1 size).
    * \ In the other cases, the table is composed by four options so the last cell memory to be pointed in the for loop is (1+WORD_SIZE)*3
    */
    stop_loop= (option_table == TEMP)? (start_loop +(1+ WORD_SIZE +1)): (start_loop + (1+WORD_SIZE)*3+1);
    
    /* layout of the menu */
    Menu_Edge();
    
    
    /* header1 and header2 of the correct table displayed*/
    sprintf(message,"   %s | %s\r\n", index, index+WORD_SIZE);
    UART_PutString(message);
    
    /* loop starting from start_loop and ending in stop_loop.
    * / In each cycle a row of the table is displayed so the i of the loop 
    * / is updated each time to (1 + WORD_SIZE) = (1 key (character) + 1 option (string)
    */
    for (i=start_loop; i<stop_loop; i+= (1+ WORD_SIZE))
    {
        sprintf(message, "       %c     |   %s\r\n", *(index+i) , index+i+1);
        UART_PutString( message);
    }
    
    /* Inform the user that if he wants go back, he had to press up */
    UART_PutString("\nPress 'q' to go back to the main menu\r\n");
    
    /* layout of the menu */
    Menu_Edge();
}

/* Function that displays the main menu.
* / Current acquisition configuration is displayed.
* / Possible keys to change acquisition settings are displayed
* / keys to enter to visualize/stop visualization data are displayed
*/
void Keys_Menu (void) 
{
    /* menu layout */
    Menu_Edge();
     /* Accelerometer acquisition configuration displayed */
    sprintf(message, "Current accelerometer acquisition configuration:\r\nFull scale range: %s\r\n",set_of_tables[0].option1 + (WORD_SIZE+1)* (EEPROM_readByte(FULL_SCALE_RANGE_ADDRESS)-1));
    UART_PutString(message);
    
    sprintf(message, "Sampling frequency: %s\r\n", set_of_tables[1].option1 + (WORD_SIZE+1)* (EEPROM_readByte(SAMPLING_FREQUENCY_ADDRESS)-1));
    UART_PutString(message);
    
    /* Temperature data format displayed */
    sprintf(message, "Temperature sensor data format: %s\r\n\n", (EEPROM_readByte(TEMPERATURE_UNIT_ADDRESS)==2)? "Fahrenheit" : "Celsius");
    UART_PutString(message);
    
    /* Keyboard commands to change configuation and to handle data visualization */
    UART_PutString("Press 'b' to start data acquisition and storage\r\n\n");
    
    UART_PutString("Press 's' to stop data acquisition and storage\r\n\n");
    
    UART_PutString("Press 'f' to change the accelerometer full scale range .Previous stored data will be deleted\r\n\n");
    
    UART_PutString("Press 'p' to change the accelerometer sampling frequency. Previous data will be deleted\r\n\n");
    
    UART_PutString("Press 't' to change the temperature data format\r\n\n");
    
    UART_PutString("Press 'q' to quit the menu\r\n\n");
    
    UART_PutString("Press 'v' to visualize data (Data acquisition will be stopped)\r\n\n");
    
    UART_PutString("Press 'u' to stop the visualization of data\r\n\n");
    
    /* menu layout */
    Menu_Edge();
}
/* Function for menu layout */
void Menu_Edge(void)
{
    UART_PutString("\r\n     ************************\r\n\n");
}
    
    
/* Function that displays a warning message for the user when his input is not valid */    
void Display_Error(){
    UART_PutString("\nSelection invalid. Please choose one of the available characters\r\n");
}

    
    
    
    
    
    
    
    
    
    

/* [] END OF FILE */
