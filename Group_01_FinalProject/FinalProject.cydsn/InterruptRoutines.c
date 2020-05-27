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


int16 temperature_digit = 0;
int16 temperature_mv = 0;
float temperature_celsius = 0;
char message[20];


/* array to store accelerometer output data read from FIFO (starting from the position zero, for 32 samples: LSB and MSB (left-justified) of X,Y and Z axis acceleration ) */
uint8_t AccelerometerData[BYTES_READ_FROM_FIFO];

/* array to store the 3 accelerations in digit(from the position zero, for 32 samples: X axis, Y axis, Z axis) */
int16_t Accelerations_digit[BYTES_READ_FROM_FIFO/2];

int16_t Temperature_Data[WATERMARK_LEVEL + 1];
uint8_t EEPROM_Data[EEPROM_PACKET_BYTES * (WATERMARK_LEVEL + 1)];
//uint8_t DataBuffer[BYTES_READ_FROM_FIFO];
//uint8_t DataBuffer2[ACCELEROMETER_DATA_BYTES+2];
//volatile uint8_t PacketReadyFlag = 0;
volatile uint8_t FIFODataReadyFlag = 0;   
volatile uint8_t TempDataReadyFlag = 0;

CY_ISR(Custom_isr_TIMER){
    Timer_ReadStatusRegister();
    
    temperature_digit = ADC_DelSig_Read16();
    
    if(temperature_digit < 0) temperature_digit = 0;
    if(temperature_digit > 1023) temperature_digit = 1023;
    
    temperature_mv = ADC_DelSig_CountsTo_mVolts(temperature_digit);
    
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
                    ShowMenuFlag = 1;
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
        
        else {
            
            if(option_table == F_S_R){
                switch (ch_received){
                    case 1:
                    //todo SELECT FSR
                    feature_selected = 1;
                    break;
                    case 2:
                    //todo SELECT FSR
                    feature_selected = 2;
                    break;
                    case 3:
                    //todo SELECT FSR
                    feature_selected = 3;
                    break;
                    case 4:
                    //todo SELECT FSR
                    feature_selected = 1;
                    break;
                }
              
            }else if(option_table == SAMP_FREQ){
                switch (ch_received){
                    case 1:
                        //todo SELECT SAMP_FREQ
                        feature_selected = 1;
                    break;
                    case 2:
                        //todo SELECT SAMP_FREQ
                        feature_selected = 1;
                    break;
                    case 3:
                        //todo SELECT SAMP_FREQ
                        feature_selected = 1;
                    break;
                    case 4:
                        //todo SELECT SAMP_FREQ
                        feature_selected = 1;
                    break;
                }
            }else if(option_table == TEMP){
                switch (ch_received){
                    case 'c':
                    case 'C':
                        //todo SELECT TEMPERATURE UNIT
                        feature_selected = 'c';
                    break;
                    case 'F':
                    case 'f':
                        //todo SELECT TEMPERATURE UNIT
                        feature_selected = 'f';
                    break;
                }
            
            }
                
        }
    }
}

/*
* \brief ISR on FIFO watermark level (generated every xx):
* \-read the accelerometer register int1_src to clear the interrupt signal on INT1 pin
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
//              DataBuffer[i*2]=Accelerations_digit[i] & 0xFF;
//              DataBuffer[i*2+1]=Accelerations_digit[i]>>8;
                
            }      
            FIFODataReadyFlag = 1;
        }

    }
}
               

/* [] END OF FILE */
