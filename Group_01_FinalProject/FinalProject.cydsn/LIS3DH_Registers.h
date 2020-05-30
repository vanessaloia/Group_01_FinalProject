/*
* \file LIS3DH_Registers.h
* \header file for the configuration of accelerometer registers 
* \this file contains, for all the registers of the LIS3DH accelerometer used in this project:
* \-the definition of the register address
* \-the definition of the content written in the register at the first start up configuration (if the register is written)
* \accelerometer registers are non volatile so they are all configured only at the first start up
* \control register 1 and 4 are then changed at runtime according to the user settings
*/


#ifndef _REGISTERS_H_ 
    #include "project.h"
    #define _REGISTERS_H_
    
    /* Address of the LIS3DH slave device */
    #define LIS3DH_DEVICE_ADDRESS 0x18
    
    /* This register must be read to clear the interrupt signal on INT1 pin */
    #define INT1_SRC_ADDR 0x31
    
    /* OUT LOW AND HIGH REGISTERS (READ ONLY) contain, for each axis, the 10 bit output expressed as two's complement left-justified */
    #define OUT_X_L_ADDR 0x28
    #define OUT_X_H_ADDR 0x29
    
    #define OUT_Y_L_ADDR 0x2A
    #define OUT_Y_H_ADDR 0x2B
    
    #define OUT_Z_L_ADDR 0x2C
    #define OUT_Z_H_ADDR 0x2D
    
    
    /* REGISTERS FOR THE CONFIGURATION OF SAMPLING FREQUENCY, FSR AND RESOLUTION FOR ACCELERATION DATA */ 
    
    /*
    * \CONTROL REGISTER 1 (READ AND WRITE) configured in this way:
    * \-ctrl_reg_1[7:4] = 0000 --> data rate output set to 0 Hz (these bits will be then modified to the correct frequency when the user insert the begin command)
    * \(when the begin command is received, if the rate has never been set by the user, 1 Hz will be used as default value)
    * \(when the stop command is received the rate is set again to 0 Hz to stop acquisition of accelerometer data)
    * \-ctrl_reg_1[3] = 0      --> low power mode disabled
    * \-ctrl_reg_1[2] = 1      --> Z axis enabled
    * \-ctrl_reg_1[1] = 1      --> Y axis enabled    
    * \-ctrl_reg_1[0] = 1      --> X axis enabled    
    */
    #define CTRL_REG_1_ADDR 0x20
    #define CTRL_REG_1_CONTENT 0x07
    
    /*
    * \CONTROL REGISTER 4 (READ AND WRITE) configured in this way:
    * \-ctrl_reg_4[7] = 0    --> continuous update of output registers
    * \-ctrl_reg_4[6] = 0    --> data LSB at lower register address, data MSB at higher address
    * \-ctrl_reg_4[5:4] = 00 --> full scale range ± 2g (default value used until the user set the desired FSR)
    * \-ctrl_reg_4[3] = 0    --> high resolution mode disabled  
    * \-ctrl_reg_4[2:1] = 00 --> self-test disabled
    * \-ctrl_reg_4[0] = 0    --> default value for SPI mode (don't care value for this bit since SPI is not used in this project) 
    */
    #define CTRL_REG_4_ADDR 0x23
    #define CTRL_REG_4_CONTENT 0x00
    
    
    /* REGISTERS FOR THE CONFIGURATION OF THE FIFO */
    
    /*
    * \CONTROL REGISTER 3 (READ AND WRITE) configured in this way:
    * \-ctrl_reg_3[7:3] = 00000  --> other interrupts disabled
    * \-ctrl_reg_4[2] = 1        --> FIFO watermark interrupt on INT1 pin enabled
    * \-ctrl_reg_4[1:0] = 00     --> other interrupts disabled
    */
    #define CTRL_REG_3_ADDR 0x22
    #define CTRL_REG_3_CONTENT 0x04
    
    /*
    * \CONTROL REGISTER 5 (READ AND WRITE) configured in this way:
    * \-ctrl_reg_5[7] = 0      --> reboot memory content disabled 
    * \-ctrl_reg_5[6] = 1      --> FIFO enabled    
    * \-ctrl_reg_5[5:4] = 00   --> no meaning bits
    * \-ctrl_reg_5[3:0] = 0000 --> default value (no latch interrupt and no 4D detection) 
    */
    #define CTRL_REG_5_ADDR 0x24
    #define CTRL_REG_5_CONTENT 0x40
  
    /*
    * \FIFO CONTROL REGISTER (READ AND WRITE) configured in this way:
    * \-fifo_ctrl_reg[7:6] = 10    --> stream mode for the fifo
    * \-fifo_ctrl_reg[5] = 0       --> trigger event on INT1 pin
    * \-fifo_ctrl_reg[4:0] = 11111 --> watermark level = 31
    */
    #define FIFO_CTRL_REG_ADDR 0x2E
    #define FIFO_CTRL_REG_CONTENT 0x9F
    
    /* Part of the register that has to be maitained unchanged when the register 
    * \ is modified according to the sampling frequency required by the user 
    */
    #define CTRL_REG_1_CONST 0b111
    
       
    void Accelerometer_Configuration(void);
    void Change_Accelerometer_FSR(uint8_t feature_selected);
    void Change_Accelerometer_SampFreq(uint8_t feature_selected);
    
#endif

/* [] END OF FILE */
