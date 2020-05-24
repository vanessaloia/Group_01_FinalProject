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
#include <stdio.h>
#include <string.h>
#include "25LC256.h"

#define DATA_SIZE 64
#define EEPROM_BYTES 32768
char message[100];

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    UART_Start();
    SPIM_Start(); 
    CyDelay(10);
    
    UART_PutString("UART started\r\n");
    int8_t data[DATA_SIZE];
    int i;
    uint16 addr = 0x0000;
    
    for (i=0;i<DATA_SIZE;i++){
        data[i] = 0;
    }
    UART_PutString("EEPROM RESETTED\r\n");

    
   /* for(i=0; i<EEPROM_BYTES/64; i++){
        EEPROM_writePage(addr, (uint8_t*) data, DATA_SIZE);
        EEPROM_waitForWriteComplete();
        sprintf(message,"%d pages completed\r\n",i);
        UART_PutString(message);
        addr += 64;
        CyDelay(10);
    }*/
        
    uint8_t data_read[3];
    uint16 addr_read = 0x0000;
    
    //for(i=0;i<EEPROM_BYTES;i++){
        data_read[0] = EEPROM_readByte(30000);
        sprintf(message, "Cella 0x%x = %d\r\n", 30000, data_read[0]);
        UART_PutString(message);
        /*data_read[1] = EEPROM_readByte(67);
        sprintf(message, "Cella 0x%x = %d\r\n", 67, data_read[1]);
        UART_PutString(message);
        data_read[2] = EEPROM_readByte(2020);
        sprintf(message, "Cella 0x%x = %d\r\n", 2020, data_read[2]);
        UART_PutString(message);*/
        //addr_read += 1;
        CyDelay(10);
   // }
    
    UART_PutString("*************************************\r\n");
    
    for(;;)
    {
        
    }
    return 0;
}

/* [] END OF FILE */
