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
#include "I2C_Interface.h"
#include "LIS3DH_Registers.h"
#include "25LC256.h"
#include "MemoryCells.h"

//Questo da rimuovere
#include "EEPROMCommunication.h"

/* string to print meessages/menu to UART */
char message[100];


/* Array to store accelerometer output data read from FIFO (starting from the position zero, for 32 samples: LSB and MSB (left-justified) of X,Y and Z axis acceleration ) */
uint8_t AccelerometerData[BYTES_READ_FROM_FIFO];


/* brief ISR on TIMER OVERFLOW
* \- if reset button has been pressed, increase time_counter variable
* \- sample a new temperature value and store it in the Temperature_Data array according to temp_counter value
* \- check if 32 new data are available in the array
* \-if so, rise the flag to allow preparation of data to be sent to EEPROM in main.c 
*/
CY_ISR(Custom_isr_TIMER){
    
    /*Read status register to reset the isr*/
    Timer_ReadStatusRegister();
    
    UART_PutString("isr timer\r\n");
    
    /*Counting time only if the button is pressed*/
    if(button_pressed == BUTTON_PRESSED){
        time_counter ++;
    }
    
    /*Read temperature data from the 10 bit adc*/
    Temperature_Data[temp_counter] = ADC_DelSig_Read16();
    
    /* check constraints on minimum and maximum values of Temperature_Data[i] */
    if (Temperature_Data[temp_counter] < 0) Temperature_Data[temp_counter] = 0;
    if (Temperature_Data[temp_counter] > 1023) Temperature_Data[temp_counter] = 1023;
   
    /* if 32 temperature data are ready the flag is put high  */
    if (temp_counter == WATERMARK_LEVEL) TempDataReadyFlag = 1;
    /* temp_counter is increased, considering a maximum value of 63*/
    temp_counter = (temp_counter+1) % ((WATERMARK_LEVEL + 1) * 2);
    if (temp_counter == 0) TempDataReadyFlag = 1; 
}

/* brief ISR on UART CHARACTER RECEIVED
* \each time the user types a character, his input is evaluated:
* \depending on which menu is displayed different character are valid
* \and for each valid input differents flags are updated
*/
CY_ISR(Custom_isr_UART)
{
      char ch_received;
       /* character on the Rx line stored */
       ch_received= UART_GetChar();
       
       /*Initial menu (While_Working_Menu) possible options*/
       if (change_settings_flag==0)
       {
            
           switch (ch_received)
           {
                case '?':
                
                    /* show Key_menu menu */
                    show_menu_flag = SHOW_MENU;
                    /* this flag is put high so that the next user's input is evaluated between different possible keys */
                    change_settings_flag = 1;
                    break;
            
                case 'v': 
                case 'V':
                    /* Turn on the external led */
                    Red_LED_Write(1);
                    /* set high the flag to START the visualization of data and stop the acquisition */
                    display_data=START;
                    break;
                
                case 'u':
                case 'U':
                    /* turn off the external led */
                    Red_LED_Write(0);
                    /* set the flag to STOP to stop streaming of data in bridge control panel */
                    display_data=STOP;
                    break;
              
                /*every other character is invalid */
                default:
                    /* flag high to show an error message */
                    display_error = SHOW_ERROR;
                    break;
            }
    }
     /*Main menu (Keys_Menu) possible options*/
     else {
        
        /* if the user is on the Keys_menus yet */
        if(option_table == DONT_SHOW_TABLE)
        {
            switch( ch_received ) 
            {
                case 'b':
                case 'B':
                    /* start flag put to START so that in the main actions needed 
                    * / (start acquisition and storage) are done 
                    */
                    start = START;
                    /* flag indicates that we are in acquisition condition */
                    begin_pressed = START;
                    break;
                
                case 's': 
                case 'S':
                    /* start flat put to STOP so that in the main actions needed 
                    * / (stop acquisition and storage) are done 
                    */
                    start = STOP;
                    /* flag indicated that we are in non-acquisition condition */
                    begin_pressed = STOP;
                    break;
                
                case 'f':
                case 'F':
                    /* option_table updated in oder to change the valid keys at the next user's input
                    * /and to show the full scale range table
                    */
                    option_table= F_S_R;
                    break;
                
                case 'p':
                case 'P':
                    /* option_table updated in oder to change the valid keys at the next user's input
                    * /and to show the sampling frequency table
                    */
                    option_table= SAMP_FREQ;
                    break;
                
                case 't':
                case 'T':
                    /* option_table updated in oder to change the valid keys at the next user's input
                    * /and to show the temoerature format table
                    */
                    option_table= TEMP;
                    break;
                
                case 'q':
                case 'Q':  
                    
                    /* set flag to enable the options of the initial menu (While_Working_Menu) */
                    change_settings_flag=0;
                    /* set flag to display the initial menu (While_Working_Menu) */
                    while_working_menu_flag = SHOW_MENU;
                    break;
                
                case 'v': 
                case 'V':
                    /* Turn on the external led */
                    Red_LED_Write(1);
                    /* set high the flag to START the visualization of data and stop the acquisition */
                    display_data=START;
                    break;
                
                case 'u':
                case 'U':
                    /* turn off the external led */
                    Red_LED_Write(0);
                    /* set the flag to STOP to stop streaming of data in bridge control panel */
                    display_data=STOP;
                    break;
                    
                /*every other character is invalid */
                default:
                    /* flag high to show an error message */
                    display_error = SHOW_ERROR;
                    break;
            }
        }
        /* valid inputs for Show_Table() menu */
        else {
            /* if the table shown was the full scale range one or the sampling frequency one
            * / the inputs allowed are the same 
            */
            if(option_table == F_S_R || option_table == SAMP_FREQ){
                switch (ch_received){
                    /*feature_selected is set at the same value of the input*/
                    case '1':
                        feature_selected = 1;
                        break;
                    case '2':
                        feature_selected = 2;
                        break;
                    case '3':
                        feature_selected = 3;
                        break;
                    case '4':
                        feature_selected = 4;
                        break;
                    /* in case 'q' is pressed the Keys_menu is shown again */
                    case 'q':
                    case 'Q':  
                        /* set flags to go back to the main menu (Keys_Menu) */
                        change_settings_flag=1;
                        show_menu_flag = SHOW_MENU;
                        option_table = DONT_SHOW_TABLE;
                        break;
                    /*every other character is invalid */
                    default:
                        /* flag high to show an error message */
                        display_error = SHOW_ERROR;
                        break;
                }
            }
            /* If the table shown was the one relative to the temperature format only two inputs are valid */
            else if(option_table == TEMP){
                switch (ch_received){
                    /* feature selected =1 when the key entered is 'c'
                    * /feature_selected =2 when the key pressed is 'f'
                    */
                    case 'c':
                    case 'C':
                        feature_selected = 1;
                        break;
                    case 'F':
                    case 'f':
                        feature_selected = 2;
                        break;
                    /* in case 'q' is pressed the keys_menu is shown again */
                    case 'q':
                    case 'Q':  
                        /* set flags to go back to the main menu (Keys_menu) */
                        change_settings_flag=1;
                        show_menu_flag = SHOW_MENU;
                        option_table = DONT_SHOW_TABLE;
                        break;
                    /*every other character is invalid */
                    default:
                        /* flag high to show an error message */
                        display_error = SHOW_ERROR;
                        break;
                }
            }
                
        }
    }
}


/* brief ISR on FIFO watermark level (generated every xx):
* \-read the accelerometer register int1_src to clear the interrupt signal on INT1 pin
* \-read the 32 accelerometer samples from FIFO (function for multi register reading is used)
* \-convert the data into 16-bit variable right justified.
* \-rise the flag to prepare the packet to be sent to EEPROM
*/

CY_ISR(Custom_isr_FIFO) {
    
    UART_PutString("isr fifo \r\n");
    
    uint8_t int1_src_reg;
    uint8_t i;
    
    /* read the accelerometer register int1_src to clear the interrupt signal on INT1 pin */
    ErrorCode error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                                  INT1_SRC_ADDR,
                                                  &int1_src_reg);
    /* if no errors occured during I2C communication */
    if (error == NO_ERROR) {
        
        /* read 192 bytes from the accelerometer output registers */
        error = I2C_Peripheral_ReadRegisterMulti( LIS3DH_DEVICE_ADDRESS,
                                                  OUT_X_L_ADDR,
                                                  BYTES_READ_FROM_FIFO,
                                                  AccelerometerData);  
        
        /* if no errors occured during I2C communication */
        if (error == NO_ERROR) {
            
            /* LSB and MSB bytes for each axis (x, y, z) are put together in a 16 bit array element */
            for(i = 0; i < BYTES_READ_FROM_FIFO/2; i++) {
                
                /* right shift of 6 to get right-justified 10 bits */
                Accelerations_digit[i] = (int16)((AccelerometerData[i*2] | (AccelerometerData[i*2+1] << 8))) >> 6;
                
            }   
            /* flag is put high because packets from FIFO are ready */
            FIFODataReadyFlag = 1;
        }

    }
    
}
/* brief ISR on BUTTON
* \each time that the state of the reset button changes (pressed/released) 
* \buttton_pressed variable changes its value. if the button is released, time_counter is set at zero
*/
CY_ISR(Custom_isr_BUTTON){
    /*button_pressed value commutated on every rising or falling edge*/
    button_pressed = (!button_pressed);
    /*time counter resetted to 0 if the button is released*/
    if(button_pressed == BUTTON_RELEASED) time_counter = 0;
}

/* [] END OF FILE */
