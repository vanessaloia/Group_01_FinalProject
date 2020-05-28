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
#include "ConfigurationMenu.h"
#include "LIS3DH_Registers.h"
#include "I2C_Interface.h"
#include "25LC256.h"
#include "MemoryCells.h"

int main(void)
{
    
    CyGlobalIntEnable; /* Enable global interrupts. */
 
    UART_Start();
    isr_UART_StartEx(Custom_isr_UART);
        
    change_settings_flag=0;
    option_table= DONT_SHOW_TABLE;
    initialized=0;
    feature_selected = 0;
    /* array used to change the period of the timer when the user changes the sampling frequency] */
    uint16 timer_periods[4] = { 1000, 100, 40, 20 }; 
    
    /* After tables displaying the possible setting have shown and the user 
    * / has inseerted a valid input, the accelerometer setting or temperature data 
    * / format are changed (da cambiare fa schifo)
    */
    if (option_table != DONT_SHOW_TABLE && feature_selected !=0) 
    {  
        /* data need to be deleted: the timer is stopped to not generate new data */
        Timer_Stop();
        
        /* Value of option table defines which settings have to be modified:
        * option table= FSR -> change the full scale range of the accelerometer
        * option table = SAMP_FREQ -> change the sampling frequency of the acceleromter
        * option table = TEMP -> change temprature data format
        * Depending on option table the value of feature_selected variable is used 
        * to operate the correct change on the acquisition settings */
        
        switch (option_table) 
        {
            case F_S_R:
                /* change full scale range */
                Change_Accelerometer_FSR();
            case SAMP_FREQ:
                /* change sampling freqeuncy */
                Change_Accelerometer_SampFreq();
                /* change timer frequency in order to change the fequency of the isr */
                Timer_WritePeriod(timer_periods[feature_selected-1]);
            case TEMP:
                /* to do */
        }
        option_table= DONT_SHOW_TABLE;
        feature_selected = 0;
    }
        
    
}

    



    
/* [] END OF FILE */
