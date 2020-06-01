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
    
    /* Data from the temperature sensors have to be converted in Celsius as:
    * /             (value_mv - 500)* 0.1 
    * /macros OFFSET_MV, M_CELSIUS, Q_CELSIUS defined to do the conversion
    */
    #define M_CELSIUS 0.1
    #define Q_CELSIUS 0
    #define OFFSET_mV 500
    /* Conversion from Celsius to Fahrenheit is (°C * 1.8) + 32
    * \So the conversion from the value in mv from the temperature sensor is
    * \     (value_mv -500) *0.18 +32
    * \ macro defined to do the conversion
    */
    #define M_FAHRENEIT 0.18
    #define Q_FAHRENHEIT 32
    
    #define PACKET_DATA 4
    #define PACKET_BYTES 8
    
    #define MG_TO_MS2 0.00981
    #define ADC_RESOLUTION 10
    #define MAX_VALUE_DIGIT 1<<10
    #define END_EEPROM_PACKETS 10
    
    
    
    
    uint8_t x_l;
    uint8_t x_h;
    uint8_t y_l;
    uint8_t y_h;
    uint8_t z_l;
    uint8_t z_h;
    uint8_t t_l;
    uint8_t t_h;    
    int16_t EEPROM_Data_Digit[PACKET_DATA*(WATERMARK_LEVEL +1)];
    float Data_UOM[PACKET_DATA*(WATERMARK_LEVEL +1)];
    uint8_t Data_Buffer[PACKET_BYTES*(WATERMARK_LEVEL +1)];
    uint8_t Packet_To_Send[1 + PACKET_BYTES +1];
    float m_temp_conversion;
    float q_temp_conversion;
    
    uint8_t number_of_packets;
        
    uint8_t xl;
    uint8_t xh;
    uint8_t yl;
    uint8_t yh;
    uint8_t zl;
    uint8_t zh;
    uint8_t tl;
    uint8_t th;
    
    void Digit_To_EEPROM_Conversion(void);
    void EEPROM_To_Digit_Conversion (void); 
    void Digit_To_UOM_Conversion (void); 
    void Buffer_Creation(void);
    void Packets_To_Send_Creation(void);
    
    
    
#endif
/* [] END OF FILE */

