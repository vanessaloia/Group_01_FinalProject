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

#ifndef __INTERRUPT_ROUTINES_H
    #define __INTERRUPT_ROUTINES_H
    
    #include "cytypes.h"
    #include "stdio.h"
    
    /*   **** MACROS FOR Custom_isr_UART **** */
    
    /* macros that define the possible values of option_table variable */
    #define F_S_R 0
    #define SAMP_FREQ 1
    #define TEMP 2
    #define DONT_SHOW_TABLE 3
    
    /* macros that define the possible values of show_menu_flag and while_working_menu_flag */
    #define SHOW_MENU 1
    #define DONT_SHOW_MENU 0
    
    /* macros that define the possible values of start and display_data variables */
    #define STOP 0
    #define START 1
    #define ACTIONS_DONE 2
    
    /* macros that define the possible values of diaply_error variable */
    #define SHOW_ERROR 1
    #define DONT_SHOW_ERROR 0
    
    
    
     /*   **** MACROS FOR Custom_isr_TIMER **** */
    
    /* Number of Bytes to be read from the accelerometer corresponding to one sample (one value of acceleration for each axis) */
    #define ACCELEROMETER_DATA_BYTES 6
    /* FIFO watermark level: a number of samples equal to (WATERMARK_LEVEL + 1) are available in fifo when an interrupt request on fifo watermark level is generated */
    #define WATERMARK_LEVEL 31
    /* Number of bytes to be stored in EEPROM for each accelerometer sample */
    #define EEPROM_ACCELEROMETER_DATA_BYTES 4
    /* Number of bytes to be stored in EEPROM for each temperature sample */
    #define EEPROM_TEMPERATURE_DATA_BYTES 2
    /* Number of bytes of each packet stored in EEPROM 
    * \ EEPROM_ACCELEROMETER_DATA_BYTES + EEPROM_TEMPERATURE_DATA_BYTES
    */
    #define EEPROM_PACKET_BYTES 6
    
    
    /*  **** MACROS FOR Custom_isr_FIFO **** */
    
    /* Number of bytes to be read from the accelerometer FIFO at each ISR on FIFO watermark level 
    * \ ACCELEROMETER_DATA_BYTES * (WATERMARK_LEVEL + 1)
    */
    #define BYTES_READ_FROM_FIFO 192
    
    /* **** MACROS FOR Custom_isr_BUTTON **** */
    /* macros that define the possible values of button_pressed variable */
    #define BUTTON_PRESSED 0
    #define BUTTON_RELEASED 1
    
    
    
    
    
    /* **** VARIABLES FOR Custom_isr_UART **** */
    
    /* when '?' is pressed this flag is high so that the possible acceptable characters
    * \become: 'b', 's', 'f', 'p', 't', 'v', 'u', 'q'
    */
    volatile uint8_t change_settings_flag;
    
    /* flag that distisguishes three possible cases:
    * \ start = START -> key 'b' has been pressed 
    * \ start = STOP -> key 's' has been pressed
    * \ start = ACTIONS_DONE  -> all the actions needed to be done when 'b' or 's' are pressed are done 
    */
    volatile uint8_t start;
    
    /* flag that distinguishes 4 possible cases:
    * \ option_table = F_S_R -> key 'f' has been pressed
    * \ option_table = SAMP_FREQ -> key 'P' has been pressed
    * \ option_table = TEMP -> key 't' has been pressed
    * \ option_table = DONT_SHOW_TABLE -> no key has been pressed
    */
    volatile uint8_t option_table;
    /* variable that assumes the values of the user's input when one of the table is shown
    * \ for full scale range and sampling frequency table feature selected can be: 1, 2, 3, 4
    * \ for temperature: feature_selected = 1 for 'c' input and 2 for 'f' input
    */
    volatile uint8_t feature_selected;
    /* flag that distinguishes three possible cases:
    * \ display_data= START -> key'v' has been pressed
    * \ display_data = STOP -> key 'u' has been pressed 
    * \ display_data= ACTIONS_DONE -> actions needed to be done are completed
    */ 
    volatile uint8_t display_data;
    
    /* flag that is put high when the user's input is invalid */
    volatile uint8_t display_error;
    
    /* flag that is put high when While_Working_Menu has to be shown */
    volatile uint8_t while_working_menu_flag;
    
    /* flag that is put high when Keys_Menu has to be shown */
    volatile uint8 show_menu_flag;
    
    /* flag that is put high when one of the table needs to be shown */
    volatile uint8 keys_menu;
    
    /* flag that is high when we are in the acquisition condition (key 'b' has been pressed) */
    volatile uint8_t begin_pressed;
    
    
    /* **** VARIABLES FOR Custom_isr_TIMER **** */
    
    /* Circular counter to store the position of the array Temperature_Data in which to store new sampled data */
    volatile uint8_t temp_counter; 
    
    /* Array to store 64 temperature data */
    int16_t Temperature_Data[(WATERMARK_LEVEL + 1) * 2];
    
    /* Flag to indicate that 32 new temperature data are available */
    volatile uint8_t TempDataReadyFlag;
    
    /* Variable that is updated each second when the button for the reset is pressed */
    volatile uint8_t time_counter;
    
    
    /* **** VARIABLES FOR Custom_isr_FIFO **** */
    
    /* array to store the 3 accelerations in digit(from the position zero, for 32 samples: X axis, Y axis, Z axis) */
     int16_t Accelerations_digit[BYTES_READ_FROM_FIFO/2];
    
    /* Flag to indicate that new data have been read from FIFO and must be sent to EEPROM */
    volatile uint8_t FIFODataReadyFlag;   


    /* **** VARIABLES FOR Custm_isr_BUTTON */
    
    /* flag that is high when the reset button is pressed */
    volatile uint8_t button_pressed;
     
    
    
    
    /* Prototypes ISR functions */
    CY_ISR_PROTO(Custom_isr_TIMER);
    CY_ISR_PROTO(Custom_isr_UART);
    CY_ISR_PROTO(Custom_isr_FIFO);
    CY_ISR_PROTO(Custom_isr_BUTTON);
    
    
    
    
#endif


/* [] END OF FILE */
