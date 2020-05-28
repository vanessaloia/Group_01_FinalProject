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
    
/* Circular counter to store the position of the array Temperature_Data in which to store new sampled data */
volatile uint8_t Temp_Counter = 0;

//int16 temperature_mv = 0;

/* Array to store 64 temperature data */
int16_t Temperature_Data[(WATERMARK_LEVEL + 1) * 2];

/* Flag to indicate that 32 new temperature data are available */
volatile uint8_t TempDataReadyFlag = 0;


char message[20];


/* Array to store accelerometer output data read from FIFO (starting from the position zero, for 32 samples: LSB and MSB (left-justified) of X,Y and Z axis acceleration ) */
uint8_t AccelerometerData[BYTES_READ_FROM_FIFO];

/* Array to store the 3 accelerations in digit(from the position zero, for 32 samples: X axis, Y axis, Z axis) */
int16_t Accelerations_digit[BYTES_READ_FROM_FIFO/2];

/* Flag to indicate that new data have been read from FIFO and must be sent to EEPROM */
volatile uint8_t FIFODataReadyFlag = 0;   

/* brief ISR on TC 
* \- sample a new temperature value and store it in the Temperature_Data array according to Temp_Counter value
* \- check if 32 new data are available in the array
* \-if so, rise the flag to allow preparation of data to be sent to EEPROM in main.c 
*/
CY_ISR(Custom_isr_TIMER){
    
    Timer_ReadStatusRegister();
    
    Temperature_Data[Temp_Counter] = ADC_DelSig_Read16();
    
    if (Temperature_Data[Temp_Counter] < 0) Temperature_Data[Temp_Counter] = 0;
    if (Temperature_Data[Temp_Counter] > 1023) Temperature_Data[Temp_Counter] = 1023;
    
    //temperature_mv = ADC_DelSig_CountsTo_mVolts(temperature_digit);
    
    if (Temp_Counter == WATERMARK_LEVEL) TempDataReadyFlag = 1;
    Temp_Counter = (Temp_Counter+1) % ((WATERMARK_LEVEL + 1) * 2);
    if (Temp_Counter == 0) TempDataReadyFlag = 1; 
}


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
        
        
        if(option_table == DONT_SHOW_TABLE)
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
                    option_table= F_S_R;
                    break;
                
                case 'p':
                case 'P':
                    /* show accelerometer sampling frequency */
                    option_table= SAMP_FREQ;
                    break;
                
                case 't':
                case 'T':
                    /* show temperature sensor unit of measurement table */
                    option_table= TEMP;
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
        
        //else {
            
            /* change settings */
            //option_table=DONT_SHOW_TABLE;
            
        //}
                
    }
}


/*
* \brief ISR on FIFO watermark level (generated every xx):
* \-read the accelerometer register int1_src to clear the interrupt signal on INT1 pin
* \-read the 32 accelerometer samples from FIFO (function for multi register reading is used)
* \-convert the data into 16-bit variable right justified.
* \-rise the flag to prepare the packet to be sent to EEPROM
*/

CY_ISR(Custom_isr_FIFO) {
    
    uint8_t int1_src_reg;
    ErrorCode error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                                  INT1_SRC_ADDR,
                                                  &int1_src_reg);
    
    if (error == NO_ERROR) {

        error = I2C_Peripheral_ReadRegisterMulti( LIS3DH_DEVICE_ADDRESS,
                                                  OUT_X_L_ADDR,
                                                  192,
                                                  AccelerometerData);   
        
        if (error == NO_ERROR) {
            
            uint8_t i;
            for(i = 0; i < BYTES_READ_FROM_FIFO/2; i++) {
                
                /* right shift of 6 to get right-justified 10 bits */
                Accelerations_digit[i] = (int16)((AccelerometerData[i*2] | (AccelerometerData[i*2+1] << 8))) >> 6;
                
            }      
            FIFODataReadyFlag = 1;
        }

    }
}
               

/* [] END OF FILE */
