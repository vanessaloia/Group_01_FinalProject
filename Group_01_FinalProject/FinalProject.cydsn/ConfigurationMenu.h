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
#ifndef CONFIGURATION_MENU_H
    #define CONFIGURATION_MENU_H
    
    #include "project.h"
    #include "stdio.h"
    #include "InterruptRoutines.h"
    
    
    
    /* size of the message to print */
    #define LENGTH_MEX 100 
    /* size of the strings inside the struct "options_to_display */
    #define WORD_SIZE 20
    /* tables to display: full scale range, sampling frequency, temperature format */
    #define NUMBER_OF_TABLES 3
    
    /* struct that defines the variables to display in the tables for the accelerometer options */
    typedef struct 
    {   
        char header1[WORD_SIZE];
        char header2[WORD_SIZE];
        char key1;
        char option1[WORD_SIZE];
        char key2;
        char option2[WORD_SIZE];
        char key3;
        char option3[WORD_SIZE];
        char key4;
        char option4[WORD_SIZE];
    } options_to_display;
    
    /* array of three elements containing the structs "option to display" */
    options_to_display set_of_tables[NUMBER_OF_TABLES];
   
    /* declaration of functions that display different menu to the user */ 
    void While_Working_Menu(void);
    void Switch_to_BridgeControlPanel(void);
    void Show_Table(uint8_t index_table);
    void Keys_Menu (void);
    void Menu_Edge(void);
    void Display_Error(void);
   
    
#endif

/* [] END OF FILE */
