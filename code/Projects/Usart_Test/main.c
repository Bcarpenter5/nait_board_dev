//////////////////////////////////////////////////////////////////////////////////////
// Project: Usart Demo test
//       
//       
// Author: Brandon Carpenter 
//       
//       
//       
// updates: 
//       created: 01/24/2024
//       
//       
//////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "stm32g031xx.h"

#include "gpio.h"
#include "clock.h"
#include "UART.h"

char urt1_flag = 0;
char urt1_data = 0;
char vurt_flag =0;
char vurt_data = 0;


int main(void) {

    // Enable interrupts globally
    __enable_irq();
    printf("begining");
    // Configure system clock
    ClockSet(_20MHz);
    // enable the GPIOA clock j
    // RCC->APBENR1 |= RCC_APBENR1_USART2EN;  it is enabled in Vcom init 
    
    Vcom_Init(300, 1);

    RCC->APBENR2 |= 1 << RCC_APBENR2_USART1EN_Pos;
    UART_Init(USART1, 300, 1);

    //GPIO_InitAlternateF(GPIOA, 9,1);
    //GPIO_InitAlternateF(GPIOA, 10,1);
    //having two pins set too the same output causes issues
    GPIO_InitAlternateF(GPIOB, 6,0); 
    GPIO_InitAlternateF(GPIOB, 7,0);

    


    
    // UART_Init(VCOM, 115200, 0);
    //NVIC_EnableIRQ(USART1_IRQn);

    TimerInit();
    printf("entering main");



    for(;;) {
        // send a character 
        //UART_TxStr(USART1, "To Boldy Go where No man has gone before\n\r");
        // UART_TxStr(VCOM, "To Boldy Go where No man has gone before\n\r");
        // wait for the character to be sent
        // DelayMs(10);

        if(vurt_flag){
            vurt_flag = 0;
            UART_TxChar(USART1, vurt_data);
            // UART_TxChar(VCOM, vurt_data);
        }
        if(urt1_flag){
            urt1_flag = 0;
            UART_TxChar(VCOM, urt1_data);
            // UART_TxChar(USART1, urt1_data);
        }
        
    }
}

void USART1_IRQHandler(void){
    // check if the interrupt is from the RXNE flag
    char data;
    if(UART_RxByte(USART1, &data)){
        // UART_TxChar(VCOM, data);
        urt1_flag = 1;
        urt1_data = data;
        //printf("%c",data);
      
    }
    NVIC_ClearPendingIRQ(USART1_IRQn);
}

void USART2_IRQHandler(void){
    // check if the interrupt is from the RXNE flag
    char data;
    if(UART_RxByte(USART2, &data)){
        // UART_TxChar(USART1, data);
        // UART_TxChar(VCOM, data);
        vurt_flag = 1;
        vurt_data = data;
        //printf("%c",data);
    }
    NVIC_ClearPendingIRQ(USART2_IRQn);
}



/*************************** End of file ****************************/
