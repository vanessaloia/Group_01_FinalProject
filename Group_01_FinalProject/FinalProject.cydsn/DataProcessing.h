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

#ifndef DATA_PROCESSING_H 
    #define DATA_PROCESSING_H
    
    #include "project.h"
    #include "InterruptRoutines.h"
    
    /* Data from the temperature sensor have to be converted in Celsius as:
    * /             (value_mv - 500)* 0.1 
    * /macros OFFSET_MV, M_CELSIUS, Q_CELSIUS defined to do the conversion
    */
    #define M_CELSIUS 0.1
    #define Q_CELSIUS 0
    #define OFFSET_mV 500
    
    /* Conversion from Celsius to Fahrenheit is (°C * 1.8) + 32
    * \So the conversion from the value in mv from the temperature sensor is
    * \     (value_mv - 500) *0.18 +32
    * \ macro defined to do the conversion
    */
    #define M_FAHRENEIT 0.18
    #define Q_FAHRENHEIT 32
    
    
    #define PACKET_DATA 4
    #define PACKET_BYTES PACKET_DATA * 2
    
    /* coefficient to make the conversion from 1 mg to m/s^2
    * \ 1 mg = 0.00981 m/s^2
    */
    #define MG_TO_MS2 0.00981
    
    /* 10 bits resolution data */
    #define ADC_RESOLUTION 10
    /* 1024 possible values with 10 bits  */
    #define MAX_VALUE_DIGIT 1024
   
    #define END_EEPROM_PACKETS 20
    
    
    
    
    int16_t  EEPROM_Data_digit [192/*PACKET_DATA*(WATERMARK_LEVEL +1)*/];
    float  Data_UOM [PACKET_DATA*(WATERMARK_LEVEL +1)];
    uint8_t Data_Buffer[PACKET_BYTES*(WATERMARK_LEVEL +1)];
    uint8_t Packet_To_Send[1 + PACKET_BYTES +1];
    float m_temp_conversion;
    float q_temp_conversion;
    
    uint8_t number_of_packets;
        
     
    
    void Digit_To_EEPROM_Conversion(void);
    void EEPROM_To_Digit_Conversion (void); 
    void Digit_To_UOM_Conversion (void); 
    void Buffer_Creation(void);
    void Packets_To_Send_Creation(void);
    
    
    
#endif
/* [] END OF FILE */
