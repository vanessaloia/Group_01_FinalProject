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
