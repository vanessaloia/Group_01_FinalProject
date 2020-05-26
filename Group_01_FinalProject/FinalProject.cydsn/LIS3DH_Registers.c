/* 
* \file LIS3DH_Registers.c 
* \source file for the configuration of accelerometer registers
*/

#include "LIS3DH_Registers.h"
#include "project.h"
#include "I2C_Interface.h"
#include "stdio.h"

/* 
* \brief function to configure accelerometer registers
* \Accelerometer registers are non volatile so this function is called only the first time the device is turn on 
* \settings modifiable by the user are set to default values and will be changed at runtime when the user requires it
* \(default values for sampling frequency and FSR: 1Hz and Â±2g)
*/
void Accelerometer_Configuration(void) {
    
    /* String to print out messages on the UART */
    char message[50];
    
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


/* [] END OF FILE */
