//////////////////////////////////////////////////////////////////////////////////////
// Project: FlashTest
//       
//       
// Author: Brandon Carpenter 
//       
//       
//       
// updates: 
//       created: 03/06/2024
//       
//       
//////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32g031xx.h"

#include "gpio.h"
#include "clock.h"
#include "UART.h"
#include "I2C.h"
#include "flash.h"

// GPIO button pins all on port A and the A 0-7 pins
// this is for A 0-7 a total of 8 pins
#define A0 0
#define A1 1
#define A2 4
#define A3 5
#define A4 12
#define A5 11
#define A6 6
#define A7 7
#define Button_PORT GPIOA

// setup the location for the flash page
#define Page31 (int*)0x0800F800


char str[] = "Hello World\n\r";

char read_str[20] = "";

uint64_t blah_blah = 1;


int main(void) {

    // Enable interrupts globally
    __enable_irq();

    // Configure system clock
    ClockSet(_20MHz);
    // enable the GPIOA clock j
    // RCC->APBENR1 |= RCC_APBENR1_USART2EN;  it is enabled in Vcom init 
    
    //Vcom_Init(115200, 1);

    GPIO_InitAlternateF(GPIOA, 9,1);
    GPIO_InitAlternateF(GPIOA, 10,1);
    GPIO_InitAlternateF(GPIOB, 6,0); 
    GPIO_InitAlternateF(GPIOB, 7,0);


    // init the GPIO pins for the buttons
    gpio_init(GPIOA,GPIO_MODE_INPUT, A0);
    gpio_init(GPIOA,GPIO_MODE_INPUT, A1);
    gpio_init(GPIOA,GPIO_MODE_INPUT, A2);
    gpio_init(GPIOA,GPIO_MODE_INPUT, A3);
    gpio_init(GPIOA,GPIO_MODE_INPUT, A4);
    gpio_init(GPIOA,GPIO_MODE_INPUT, A5);
    gpio_init(GPIOA,GPIO_MODE_INPUT, A6);
    gpio_init(GPIOA,GPIO_MODE_INPUT, A7);


    RCC->APBENR2 |= 1 << RCC_APBENR2_USART1EN_Pos;
    //UART_Init(USART1, 250000, 1);

    //NVIC_EnableIRQ(USART1_IRQn);

    //TimerInit();

    // enable flash clock
    RCC->APBENR2 |= RCC_AHBENR_FLASHEN;
    flash_unlock();
    // flash_erase_page(31);
    // flash_program(Page31, str, sizeof(str));
    // memcpy(read_str, (char*)Page31, sizeof(str));


    //flash_wait_for_last_operation();
    //FLASH->OPTR |= 0xFFFFFEAA & ~FLASH_OPTR_NRST_MODE | FLASH_OPTR_NRST_MODE_0;

    //flash_wait_for_last_operation();
    //FLASH->CR |= FLASH_CR_OPTSTRT;
    //flash_wait_for_last_operation();
    //flash_lock_option_bytes();


    //UART_TxStr(USART2, read_str);
    printf(read_str);

    //Page31 = (int *)0x0800F800;

    



    for(;;) {
        // send a character 
        //UART_TxStr(USART1, "To Boldy Go where No man has gone before\n\r");
        // UART_TxStr(VCOM, "To Boldy Go where No man has gone before\n\r");
        // wait for the character to be sent
        // DelayMs(10);
        // this is here
    }
}



/***********************************************  interrupt handlers *****************************************************/

void USART1_IRQHandler(void){ 
    // check if the interrupt is from the RXNE flag
    char data;
    if(UART_RxByte(USART1, &data)){
    }
    NVIC_ClearPendingIRQ(USART1_IRQn);
}

void USART2_IRQHandler(void){
    // check if the interrupt is from the RXNE flag
    char data;
    if(UART_RxByte(USART2, &data)){
        // UART_TxChar(VCOM, data);
    }
    NVIC_ClearPendingIRQ(USART2_IRQn);
}



/*************************** End of file ****************************/
