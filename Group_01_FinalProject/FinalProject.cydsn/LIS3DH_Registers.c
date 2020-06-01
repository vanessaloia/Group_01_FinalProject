/* 
* \file LIS3DH_Registers.c 
* \source file for the configuration of accelerometer registers
*/

#include "LIS3DH_Registers.h"
#include "I2C_Interface.h"
#include "stdio.h"
#include "InterruptRoutines.h"
#include "ConfigurationMenu.h"

/* 
* \brief function to configure accelerometer registers
* \Accelerometer registers are non volatile so this function is called only the first time the device is turn on 
* \settings modifiable by the user are set to default values and will be changed at runtime when the user requires it
* \(default values for sampling frequency and FSR: 1Hz and Â±2g)
*/
/* String to print out messages on the UART */
char message[100];

void Accelerometer_Configuration(void) {
    
    
    
    /* Variable to check error in I2C communication */
    ErrorCode error;    

    /* Check which devices are present on the I2C bus */
    for (int i = 0 ; i < 128; i++)
    {
        if (I2C_Peripheral_IsDeviceConnected(i))
        {
            /* print out the address in hex format */
            sprintf(message, "Device 0x%02X is connected\r\n", i);
            UART_PutString(message); 
        }
        
    }
    
    /* REGISTERS CONFIGURATION: registers are first read and then, only if their content is not the desired one, they are overwritten and read again to check correct writing */    
    
    
    /* Configuration of control registers 1 and 4 to output 3 Axis accelerometer data in Normal Resolution Mode (10-bit) at 1 Hz in the Â±2.0 g FSR. */
    /* See registers.h for single bits meaning */

    /* Read Control Register 1 */
    uint8_t ctrl_reg1; 
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        CTRL_REG_1_ADDR,
                                        &ctrl_reg1);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 1: 0x%02X\r\n", ctrl_reg1);
        UART_PutString(message); 
    }
    else
    {
        UART_PutString("Error occurred during I2C comm to read control register 1\r\n");   
    }
    
    /* Write Control Register 1 (only if the read content is not the desired one) */
    if (ctrl_reg1 != CTRL_REG_1_CONTENT)  
    {
        UART_PutString("\r\nWriting new values..\r\n");
        
        ctrl_reg1 = CTRL_REG_1_CONTENT;
    
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             CTRL_REG_1_ADDR,
                                             ctrl_reg1);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 1 successfully written as: 0x%02X\r\n", ctrl_reg1);
            UART_PutString(message); 
        }
        else
        {
            UART_PutString("Error occurred during I2C comm to set control register 1\r\n");   
        }
    
    
        /* Read Control Register 1 again to check for correct writing */
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                            CTRL_REG_1_ADDR,
                                            &ctrl_reg1);
        
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 1 after overwrite operation: 0x%02X\r\n", ctrl_reg1);
            UART_PutString(message); 
        }
        else
        {
            UART_PutString("Error occurred during I2C comm to read control register 1\r\n");   
        }
    }
    
    /* Read Control Register 4 */
    uint8_t ctrl_reg4; 
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        CTRL_REG_4_ADDR,
                                        &ctrl_reg4);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 4: 0x%02X\r\n", ctrl_reg4);
        UART_PutString(message); 
    }
    else
    {
        UART_PutString("Error occurred during I2C comm to read control register 4\r\n");   
    }
    
    /* Write Control Register 4 (only if the read content is not the desired one) */
    if (ctrl_reg4 != CTRL_REG_4_CONTENT)  
    {
        UART_PutString("\r\nWriting new values..\r\n");
        
        ctrl_reg4 = CTRL_REG_4_CONTENT;
    
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             CTRL_REG_4_ADDR,
                                             ctrl_reg4);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 4 successfully written as: 0x%02X\r\n", ctrl_reg4);
            UART_PutString(message); 
        }
        else
        {
            UART_PutString("Error occurred during I2C comm to set control register 4\r\n");   
        }
    
    
        /* Read Control Register 4 again to check for correct writing */
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                            CTRL_REG_4_ADDR,
                                            &ctrl_reg4);
        
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 4 after overwrite operation: 0x%02X\r\n", ctrl_reg4);
            UART_PutString(message); 
        }
        else
        {
            UART_PutString("Error occurred during I2C comm to read control register 4\r\n");   
        }
    }    
    
    
    /* Configuration of control registers 3 and 5 and fifo control register to enable FIFO in stream mode and enable an interrupt on FIFO watermark level (set to 31)*/
    /* See registers.h for single bits meaning */

    /* Read Control Register 3 */
    uint8_t ctrl_reg3; 
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        CTRL_REG_3_ADDR,
                                        &ctrl_reg3);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 3: 0x%02X\r\n", ctrl_reg3);
        UART_PutString(message); 
    }
    else
    {
        UART_PutString("Error occurred during I2C comm to read control register 3\r\n");   
    }
    
    /* Write Control Register 3 (only if the read content is not the desired one) */
    if (ctrl_reg3 != CTRL_REG_3_CONTENT)  
    {
        UART_PutString("\r\nWriting new values..\r\n");
        
        ctrl_reg3 = CTRL_REG_3_CONTENT;
    
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             CTRL_REG_3_ADDR,
                                             ctrl_reg3);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 3 successfully written as: 0x%02X\r\n", ctrl_reg3);
            UART_PutString(message); 
        }
        else
        {
            UART_PutString("Error occurred during I2C comm to set control register 3\r\n");   
        }
    
    
        /* Read Control Register 3 again to check for correct writing */
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                            CTRL_REG_3_ADDR,
                                            &ctrl_reg3);
        
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 3 after overwrite operation: 0x%02X\r\n", ctrl_reg3);
            UART_PutString(message); 
        }
        else
        {
            UART_PutString("Error occurred during I2C comm to read control register 3\r\n");   
        }
    } 

    /* Read Control Register 5 */
    uint8_t ctrl_reg5; 
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        CTRL_REG_5_ADDR,
                                        &ctrl_reg5);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 5: 0x%02X\r\n", ctrl_reg5);
        UART_PutString(message); 
    }
    else
    {
        UART_PutString("Error occurred during I2C comm to read control register 5\r\n");   
    }
    
    /* Write Control Register 5 (only if the read content is not the desired one) */
    if (ctrl_reg5 != CTRL_REG_5_CONTENT)  
    {
        UART_PutString("\r\nWriting new values..\r\n");
        
        ctrl_reg5 = CTRL_REG_5_CONTENT;
    
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             CTRL_REG_5_ADDR,
                                             ctrl_reg5);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 5 successfully written as: 0x%02X\r\n", ctrl_reg5);
            UART_PutString(message); 
        }
        else
        {
            UART_PutString("Error occurred during I2C comm to set control register 5\r\n");   
        }
    
    
        /* Read Control Register 5 again to check for correct writing */
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                            CTRL_REG_5_ADDR,
                                            &ctrl_reg5);
        
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 5 after overwrite operation: 0x%02X\r\n", ctrl_reg5);
            UART_PutString(message); 
        }
        else
        {
            UART_PutString("Error occurred during I2C comm to read control register 5\r\n");   
        }
    }
    
    /* Read Fifo Control Register */
    uint8_t fifo_ctrl_reg; 
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        FIFO_CTRL_REG_ADDR,
                                        &fifo_ctrl_reg);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "FIFO CONTROL REGISTER: 0x%02X\r\n", fifo_ctrl_reg);
        UART_PutString(message); 
    }
    else
    {
        UART_PutString("Error occurred during I2C comm to read fifo control register \r\n");   
    }
    
    /* Write Fifo Control Register (only if the read content is not the desired one) */
    if (fifo_ctrl_reg != FIFO_CTRL_REG_CONTENT)  
    {
        UART_PutString("\r\nWriting new values..\r\n");
        
        fifo_ctrl_reg = FIFO_CTRL_REG_CONTENT;
    
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             FIFO_CTRL_REG_ADDR,
                                             fifo_ctrl_reg);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "FIFO CONTROL REGISTER successfully written as: 0x%02X\r\n", fifo_ctrl_reg);
            UART_PutString(message); 
        }
        else
        {
            UART_PutString("Error occurred during I2C comm to set fifo control register \r\n");   
        }
    
    
        /* Read Fifo Control Register again to check for correct writing */
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                            FIFO_CTRL_REG_ADDR,
                                            &fifo_ctrl_reg);
        
        if (error == NO_ERROR)
        {
            sprintf(message, "FIFO CONTROL REGISTER after overwrite operation: 0x%02X\r\n", fifo_ctrl_reg);
            UART_PutString(message); 
        }
        else
        {
            UART_PutString("Error occurred during I2C comm to read fifo control register \r\n");   
        }
    }   
}

/* This function changes the full scale range of the accelerometer depending on the user input.
*\   It uses the I2C protocol communication to change the content of the CTRL_REG_4 register content.
*/
void Change_Accelerometer_FSR(uint8_t feature_selected) 
{
    uint8_t register_content;
    ErrorCode error;
    
    /* "register_content" contains the value to be inserted inside CTRL_REG_4 register
    * \FSR= ±2g -> CTRL_REG_4[5:4]= 00, feature_selected= 1
    * \FSR= ±4g -> CTRL_REG_4[5:4]= 01, feature_selected= 2
    * \FSR= ±8g -> CTRL_REG_4[5:4]= 10, feature_selected= 3
    * \FSR= ±16g -> CTRL_REG_4[5:4]= 11, feature_selected= 4
    */
  
    register_content = (feature_selected-1) <<4;
    
    
    /* pointer that points to the correct variable inside the struct of the FSR. It's used to print the FSR
    * \ set up in the accelerometer according to the user's input.
    * \(n-key + n-option)= WORD_SIZE (string size) +1 (char size)
    * \the pointer points to option1= ±2g so it has to go to (feature_selected -1) variables after option1
    */
   
    char * fsr_to_print = set_of_tables[option_table].option1 + (WORD_SIZE+1)* (feature_selected-1);
    
    /* Change the value of the Control register 4 according to the user's input */
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             CTRL_REG_4_ADDR,
                                             register_content);
    
        if (error == NO_ERROR)
        {
            /* print the new value */
            sprintf(message, "Full scale range successfully changed at: %s \r\n", fsr_to_print);
            UART_PutString(message); 
        }
        else
        {
            /* print error message */
            UART_PutString("Error occurred during I2C comm to write  control register 4\r\n");   
        }   
    
}

/* This function changes the sampling frequency of the accelerometer depending on the user input.
*\   It uses the I2C protocol communication to change the content of the CTRL_REG_1 register content.
*/
void Change_Accelerometer_SampFreq(uint8_t feature_selected)
{
    uint8_t register_content;
    ErrorCode error;
    
    /*  "register_content" contains the value to be inserted inside CTRL_REG_1 register
    * \SampFreq= 1 Hz -> CTRL_REG_1[5:4]= 01, feature_selected= 1
    * \SampFreq= 10 Hz -> CTRL_REG_1[5:4]= 10, feature_selected= 2
    * \SampFreq= 25 Hz -> CTRL_REG_1[5:4]= 11, feature_selected= 3
    * \SampFreq= 50 Hz -> CTRL_REG_1[5:4]= 100, feature_selected= 4
    * \CTRL_REG_1[3:0] has to be maintained to the default value CTRL_REG_CONST (0b111) 
    */
    register_content = (feature_selected <<4) + CTRL_REG_1_CONST;
    
   
    /* Change the value of the Control register 4 according to the user's input */
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             CTRL_REG_1_ADDR,
                                             register_content);
    
        if (error == NO_ERROR)
        
        {
            if (display_data != ACTIONS_DONE && start== ACTIONS_DONE) 
            {
                /* pointer that points to the correct variable inside the struct of SampFreq. It's used to print the sampling
                * \ frequency set up in the accelerometer according to the user's input.
                * / (n-key + n-option)= WORD_SIZE (string size) +1 (char size)
                * / the pointer points to option1= 1 Hz so it has to go to the (feature_selected -1) after option1 variable
                */
                char * sampfreq_to_print = set_of_tables[option_table].option1 + (WORD_SIZE + 1)* (feature_selected -1);
                sprintf(message, "Sampling frequency successfully changed at: %s \r\n", sampfreq_to_print);
                UART_PutString(message); 
            }
        }
        else
        {
            UART_PutString("Error occurred during I2C comm to read fifo control register \r\n");   
        } 
    
}








/* [] END OF FILE */
