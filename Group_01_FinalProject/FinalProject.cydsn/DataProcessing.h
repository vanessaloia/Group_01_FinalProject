/*file DataProcessing.h*/

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
    
    /* Number of data for each packet in digit or UOM */
    #define PACKET_DATA 4
    /* Number of bytes for each packet in the buffer to be sent to bridge */
    #define PACKET_BYTES 8
    
    /* coefficient to make the conversion from 1 mg to m/s^2
    * \ 1 mg = 0.00981 m/s^2
    */
    #define MG_TO_MS2 0.00981
    
    /* 10 bits resolution data */
    #define ADC_RESOLUTION 10
    #define MAX_VALUE_DIGIT 1<<10
    /* Number of packets stored in the eeprom when the end is reached (10 instead of 32 since only 64 cells are free) */
    #define END_EEPROM_PACKETS 10
    
    
    
    /* uint8_variables that contain LSB and MSB (left-adjusted) data of the axes (x, y, z) and of the temperature */
    uint8_t x_l;
    uint8_t x_h;
    uint8_t y_l;
    uint8_t y_h;
    uint8_t z_l;
    uint8_t z_h;
    uint8_t t_l;
    uint8_t t_h;   
    
    /* Array to store data converted in digit after reading from eeprom */
    int16_t EEPROM_Data_Digit[PACKET_DATA*(WATERMARK_LEVEL +1)];
    /* Array to store data converted in the right unit of measurement from digit */
    float Data_UOM[PACKET_DATA*(WATERMARK_LEVEL +1)];
    /* Big buffer that contain 256 bytes to be sent to bridge corresponding to 32 packets */
    uint8_t Data_Buffer[PACKET_BYTES*(WATERMARK_LEVEL +1)];
    /*little buffer that will be sent to the bridge with header and tail (one packet) */
    uint8_t Packet_To_Send[1 + PACKET_BYTES +1];
    /* coeffients for conversion of temperature data from mv to the correct format (Celsius or Fahrenheit) */ 
    float m_temp_conversion;
    float q_temp_conversion;
    
    /* Number of packets to be read/write in eeprom at once */
    uint8_t number_of_packets;
        
    
    
    void Digit_To_EEPROM_Conversion(void);
    void EEPROM_To_Digit_Conversion (void); 
    void Digit_To_UOM_Conversion (void); 
    void Buffer_Creation(void);
    void Packets_To_Send_Creation(void);
    
    
    
#endif
/* [] END OF FILE */

