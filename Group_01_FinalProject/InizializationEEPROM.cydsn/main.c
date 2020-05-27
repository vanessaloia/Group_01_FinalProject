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
    
    UART_PutString("UART started\r\n");
    uint8_t data[DATA_SIZE];
    uint16 i=0,count = 1;
    uint16 addr = 0x0000;
   
    for(i=0;i<DATA_SIZE;i++){
        data[i] = 0;
    }
    
    UART_PutString("Begin writing\r\n");
    CyDelay(500);
    for(i=0; i<EEPROM_BYTES/64; i++){
        EEPROM_writePage(addr, data, DATA_SIZE);
        EEPROM_waitForWriteComplete();
        CyDelay(10);
        if(i % 512/10 == 0){
            sprintf(message,"Writing at %d percent \r\n",count*10);
            UART_PutString(message);
            count++;
        }
        addr+=64;
    }
    
    UART_PutString("Writing done\r\n\n");
    CyDelay(500);
    
    uint16 addr_read = 0x0000;
    uint8_t data_read[EEPROM_BYTES];
    
    UART_PutString("Begin reading\r\n");
    CyDelay(500);
    
    count = 1;
    
    for(i=0;i<EEPROM_BYTES/64;i++){    
        EEPROM_readPage(addr_read,&data_read[addr_read],DATA_SIZE);
        CyDelay(10);
        addr_read+=64;
        if(i % 512/10 == 0){
            sprintf(message,"Reading at %d percent \r\n",count*10);
            UART_PutString(message);
            count++;
        }
    }
    
    UART_PutString("Reading done\r\n\n");
    CyDelay(500);

    int count_notzeros = 0;
    
    UART_PutString("Control on data read started\r\n");
    
    for(i=0;i<EEPROM_BYTES;i++){
        if(data_read[i] != 0){
            sprintf(message,"VALUE DIFFERENT FROM 0(CELLA 0x%x)\r\n",i);
            UART_PutString(message);
            count_notzeros++;
        }
    }
    if(count_notzeros == 0){
        UART_PutString("\nEEPROM RESETTED SUCCESFULLY, THE DEVICE IS READY TO BE USED\r\n");
    }
    else{
        UART_PutString("\nAN ERROR OCCURED ON EEPROM RESET, PLEASE RETRY\r\n");
    }
    CyDelay(10);
    UART_PutString("*************************************\r\n");
    
    for(;;)
    {
        
    }
    return 0;
}

/* [] END OF FILE */
