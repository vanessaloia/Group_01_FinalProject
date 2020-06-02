****Group_01_FinalProject****

PSoC_5 microcontroller used.
Acquisition of data from LIS3DH accelerometer and TMP36GT9Z temperature sensor.
Storage of data in 25LC256 EEPROM.
Visualization of data via Bridge Control Panel.
Acquisition settings of the devices and switch from acquisition and storage mode 
to visualization mode is made through a user's interface (CoolTerm).  

I2C communication between PSoC_5 microcontroller and LIS3DH accelerometer.
SPI communication between PSoC_5 microcontroller and 25LC256 EEPROM.
UART communication between PSoC_5 microcontroller and Bridge Control Panel and CoolTerm 

**InizializationEEPROM**
Before running FinalProject run this code to reset EEPROM memory cells.


**FinalProject**
The purpose of this project is to acquire data from LIS3DH accelerometer and TMP36GT9Z temperature 
sensor and store them in 25LC256 EEPROM. 
The user can change the acquisition settings:
	- full scale range of the accelerometer 
	- sampling frequency of the accelerometer
	- temperature unit of measurement
The user can require at any time to visualize data already stored in memory in the Bridge Control Panel.
A red led is on when streaming of data is occurring. 
The on-board blue led blinks with 1 s period when data acquisition is on and with 250 ms period when
the EEPROM is full. 
The user can reset data stored in EEPROM by pressing for 5 s the on-board button.
