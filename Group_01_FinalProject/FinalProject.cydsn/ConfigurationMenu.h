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
    
    
    
    
    #define LENGTH_MEX 50 
    #define WORD_SIZE 20
    #define NUMBER_OF_TABLES 3
    
    /* struct that defines the information to display in the table for accelerometer options */
    typedef struct 
    {   char header1[WORD_SIZE];
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
    
    char struct_initialized;
    
    options_to_display FSR;
    options_to_display SampFreq;
    options_to_display TempFormat;
    options_to_display set_of_tables[NUMBER_OF_TABLES];

    void While_Working_Menu(void);
    void Switch_to_BridgeControlPanel(void);
    void Show_table(uint8_t index_table);
    void Keys_menu (void);
    void Menu_edge(void);
    
#endif

/* [] END OF FILE */
