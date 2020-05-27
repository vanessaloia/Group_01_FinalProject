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
    CyDelay(3000);
    SPIM_ClearRxBuffer();
    SPIM_ClearTxBuffer();
    
    UART_PutString("UART started\r\n");
    uint8_t data[DATA_SIZE];
    int i=0;
    uint16 addr = 0x0000;
   
    for(i=0;i<DATA_SIZE;i++){
        data[i] = 0;
    }
    UART_PutString("Inizio scrittura :) \r\n");
    CyDelay(500);
  /*  for(i=0; i<EEPROM_BYTES/64; i++){
        EEPROM_writePage(addr, data, DATA_SIZE);
        EEPROM_waitForWriteComplete();
        CyDelay(10);
        sprintf(message,"Addr_write = 0x%x \r\n",addr);
        UART_PutString(message);
        addr+=64;
    }*/
    
    UART_PutString("All EEPROM cells initialized to 0 (ahahahahahaha)\r\n");
    CyDelay(500);
    
    uint16 addr_read = 0x0000;
    uint8_t data_read[EEPROM_BYTES];
    
    UART_PutString("scrivo tutti 3 in data read :) \r\n");
    for (i=0;i<EEPROM_BYTES;i++){
        data_read[i] = 3;
    }
    UART_PutString("fattooooooooo\r\n");
    CyDelay(500);
    
    UART_PutString("Inizio lettura\r\n");
    for(i=0;i<EEPROM_BYTES/64;i++){    
        EEPROM_readPage(addr_read,&data_read[addr_read],DATA_SIZE);
        CyDelay(10);
        addr_read+=64;
        sprintf(message,"Addr_read = %x \r\n",addr_read);
        UART_PutString(message);
    }
    
    UART_PutString("Lettura finita");
    CyDelay(500);
    
    addr_read = 0x0000;
    int count = 1;
    int count_zeri = 0;
    
    UART_PutString("INIZIO A CONTROLLAREEEEEEEEEE");
    
    for(i=0;i<EEPROM_BYTES;i++){
        if(data_read[i] != 0){
            sprintf(message,"VALORE DIVERSO DA 0 NOOOOOOOO (CELLA 0x%x)\r\n",addr_read+i);
            UART_PutString(message);
        }
        
        if(i % EEPROM_BYTES/10 == 0){
            sprintf(message,"Sono al %d percentoooooooooooooooooooo :) \r\n",count*10);
            UART_PutString(message);
            count++;
        }
    }
    if(count_zeri == 0){
        UART_PutString("SONO TUTTI A 0 HO LA EEPROM ROTTA IMPAZZISCO\r\n");
    }
    else{
        UART_PutString("NON SONO TUTTI 0 :( \r\n");
    }
    CyDelay(10);

    
    UART_PutString("*************************************\r\n");
    
    for(;;)
    {
        
    }
    return 0;
}

/* [] END OF FILE */
