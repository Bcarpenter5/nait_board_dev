//////////////////////////////////////////////////////////////////////////////////////
// Project: UASRT Lib
//       
//       
// Author: Brandon Carpenter 
//       
//       
// updates: 
//       created: 01/25/2024
//       
//////////////////////////////////////////////////////////////////////////////////////

#include "UART.h"
#include "stm32g031xx.h"
#include "gpio.h"
#include <stdint.h>
#include <stdio.h>


void Vcom_Init(uint32_t baudrate, UART_Interupt interupt){
    // Enable interrupts globally
    __enable_irq();
    // Enable USB clock

    RCC->APBENR1 |= RCC_APBENR1_USART2EN;
    /// note the vcom port is usart 2 on PA2 and PA3
    GPIO_InitAlternateF(GPIOA, 2, 1); // set PA2 to alternate function 1 TX
    GPIO_InitAlternateF(GPIOA, 3, 1); // set PA3 to alternate function 1 RX

    UART_Init(USART2, baudrate, interupt);
}


void UART_Init(USART_TypeDef *usart, uint32_t baudrate, UART_Interupt interupt){

    usart->CR1 &= ~(USART_CR1_UE|USART_CR1_TE|USART_CR1_RE); // disable the usar

    //disable over runs
    usart->CR3 |= USART_CR3_OVRDIS;

    // set the baud rate
    usart->BRR = SystemCoreClock / baudrate; // baud rate calculation

    usart->RQR |= USART_RQR_RXFRQ; // clear the RXNE flag like carlos sudgested

    usart->CR1 |= USART_CR1_UE|USART_CR1_TE|USART_CR1_RE ; // enable the usart

    if(interupt){
        usart->CR1 |= USART_CR1_RXNEIE_RXFNEIE;
        switch((uint64_t)usart){
            case (uint64_t)USART1:
                NVIC_EnableIRQ(USART1_IRQn);
                break;
            case (uint64_t)USART2:
                NVIC_EnableIRQ(USART2_IRQn);
                break;
            default:
                break;
        }

    }
    else{
        usart->CR1 &= ~(USART_CR1_RXNEIE_RXFNEIE);
        switch((uint64_t)usart){
            case (uint64_t)USART1:
                NVIC_DisableIRQ(USART1_IRQn);
                break;
            case (uint64_t)USART2:
                NVIC_DisableIRQ(USART2_IRQn);
                break;
            default:
                break;
        }
    }

   usart->RQR |= USART_RQR_RXFRQ; // clear the RXNE flag like carlos sudgested
}

void UART_TxChar(USART_TypeDef *usart, uint8_t data){
    while(!(usart->ISR & USART_ISR_TXE_TXFNF));
    usart->TDR = data;
    printf("data: %c\n", data);
    printf("data: %d\n", data);

    printf("reg in hex: %x\n", usart->TDR);


}

void UART_TxStr(USART_TypeDef *usart, char *data){
    while(*data){
        UART_TxChar(usart, *data++);
    }
}

void UART_Packet(USART_TypeDef *usart, uint8_t *data, uint8_t size){
    for(uint8_t i = 0; i < size; i++){
        UART_TxChar(usart, data[i]);
    }
}

char UART_RxByte(USART_TypeDef *usart, uint8_t *data){
    if((usart->ISR & USART_ISR_RXNE_RXFNE)){
        *data = usart->RDR;
        return 1;
    }
    return 0;
}









