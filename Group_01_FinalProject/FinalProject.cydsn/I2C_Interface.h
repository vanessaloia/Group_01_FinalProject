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
 * \author Davide Marzorati
 * \date September 12, 2019
*/
#ifndef I2C_Interface_H
    #define I2C_Interface_H
    
    #include "cytypes.h"
    #include "ErroreCodes.h"
    
    /*  
    * \Start the I2C peripheral
    */
    ErrorCode I2C_Peripheral_Start(void);
    
    /* 
    * \Stop the I2C peripheral
    */
    ErrorCode I2C_Peripheral_Stop(void);
    
    /*
    *\Read one byte over I2C.
    *\This function performs a complete reading operation over I2C from a single register.
    *
    *\device_address is the address of the device to talk to.
    *\register_address is the address of the register to be read.
    *\data pointer points to a variable where the byte will be saved.
    */
    ErrorCode I2C_Peripheral_ReadRegister(uint8_t device_address, 
                                            uint8_t register_address,
                                            uint8_t* data);
    
    /*
    *\Read multiple bytes over I2C.
    *\This function performs a complete reading operation over I2C from multiple registers.
    *
    *\device_address is the address of the device to talk to.
    *\register_address is the address of the first register to be read.
    *\register_count is the number of registers we want to read.
    *\data pointer points to an array where data will be saved.
    */
    ErrorCode I2C_Peripheral_ReadRegisterMulti(uint8_t device_address,
                                                uint8_t register_address,
                                                uint8_t register_count,
                                                uint8_t* data);
    /* 
    *\Write a byte over I2C.
    *\This function performs a complete writing operation over I2C to a single register.
    *
    *\device_address is the address of the device to talk to.
    *\register_address is the address of the register to be written.
    *\data is the data to be written
    */
    ErrorCode I2C_Peripheral_WriteRegister(uint8_t device_address,
                                            uint8_t register_address,
                                            uint8_t data);
    
    /*
    *\Write multiple bytes over I2C.
    *\This function performs a complete writing operation over I2C to multiple registers
    *
    *\ device_address I2C address of the device to talk to.
    *\ register_address Address of the first register to be written.
    *\ register_count Number of registers that need to be written.
    *\ data Array of data to be written
    */
    ErrorCode I2C_Peripheral_WriteRegisterMulti(uint8_t device_address,
                                            uint8_t register_address,
                                            uint8_t register_count,
                                            uint8_t* data);
    
    /*
    *\brief Check if device is connected over I2C.
    *\This function checks if a device is connected over the I2C lines.
    *
    *\device_address is the address of the device to be checked.
    *\retval Returns true (>0) if device is connected.
    */
    uint8_t I2C_Peripheral_IsDeviceConnected(uint8_t device_address);
    
#endif // I2C_Interface_H
/* [] END OF FILE */
