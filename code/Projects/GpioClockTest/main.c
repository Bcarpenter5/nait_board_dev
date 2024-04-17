//////////////////////////////////////////////////////////////////////////////////////
// Project: ClockGPIOTeset
//       this is meant to test the clock and GPIO Librarys
//       
// Author: Brandon Carpenter 
//       
//       
// updates: 
//       created: 11/20/2023
//       
//       
//////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include "stm32g031xx.h"

#include "clock.h"
#include "gpio.h"
#include "system_stm32g0xx.h"


/*************************** Defines ****************************/
#define LED_PIN 6

#define D9 8 
#define PA8 D9

#define A0 0 
#define PA0 A0
/*************************** Globals ****************************/

volatile uint32_t _msTicks; // counts 1ms timeTicks



/*************************** main function ****************************/
int main(void) {
    __enable_irq();
    ClockSet(_20MHz);
    SysTick_Config(20000);// 1ms tick
    TimerInit();
    
    gpio_init(GPIOC, GPIO_MODE_OUTPUT, LED_PIN );// set the LED pin to output
    gpio_init(GPIOA, GPIO_MODE_OUTPUT, D9 );// set the LED pin to output
    gpio_init(GPIOA, GPIO_MODE_INPUT, A0 );// set the LED pin to output



    while(1) {// loop forever
    gpio_toggle(GPIOC, LED_PIN);
    DelayMs(100);
    printf("sent");
    }
}
/*************************** Functions ****************************/



/*************************** Interrupts ****************************/
// void SysTick_Handler(void){
//    _msTicks++;
//    if(_msTicks % 500 == 0){
//        gpio_toggle(GPIOC, LED_PIN);
//    }
// }

/*************************** End of file ****************************/
