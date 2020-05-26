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
#include "LIS3DH_Registers.h"
#include "I2C_Interface.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    UART_Start();
    isr_UART_StartEx(Custom_isr_UART);
    
    
    change_settings_flag=0;
    option_table=0;
    
    

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    Timer_Start();
    UART_Start();
    ADC_DelSig_Start();
    
    isr_TIMER_StartEx(Custom_isr_TIMER);
    ADC_DelSig_StartConvert();
    FlagReady = 0;
    
    
    I2C_Master_Start();
    
    /* String to print out messages on the UART */
    char message[50];
    
    /* variable to check error in I2C communication */
    ErrorCode error;    

    // Check which devices are present on the I2C bus
    for (int i = 0 ; i < 128; i++)
    {
        if (I2C_Peripheral_IsDeviceConnected(i))
        {
            // print out the address is hex format
            sprintf(message, "Device 0x%02X is connected\r\n", i);
            UART_PutString(message); 
        }
        
    }
    
    /* Configuration of control registers 1 and 4 to output 3 Axis accelerometer data in High Resolution Mode at 100 Hz in the ±4.0 g FSR. */
    /* See registers.h for single bits meaning */
    /* Both register are first read and then, only if their content is not the desired one, they are overwritten and read again to check correct writing */

    
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
    
    
    DataBuffer2[0] = 0xA0;
    DataBuffer2[7] = 0xC0;
    isr_FIFO_StartEx(Custom_isr_FIFO);
    
    
    uint8_t i;
    for(;;)
    {
        /* Place your application code here. */
            if (PacketReadyFlag) {
                for (i=0; i<WATERMARK_LEVEL+1; i++) {
                  DataBuffer2[1]=DataBuffer[6*i];  
                  DataBuffer2[2]=DataBuffer[6*i+1]; 
                  DataBuffer2[3]=DataBuffer[6*i+2];  
                  DataBuffer2[4]=DataBuffer[6*i+3];  
                  DataBuffer2[5]=DataBuffer[6*i+4];  
                  DataBuffer2[6]=DataBuffer[6*i+5];    
                  UART_PutArray(DataBuffer2, 8); /*API to transmit an array of bytes */
        }
        PacketReadyFlag = 0;
        
    }
}
}
/* [] END OF FILE */
