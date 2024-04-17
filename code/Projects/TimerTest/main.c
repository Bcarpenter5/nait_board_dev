#include "stm32g031xx.h"
#include <stdio.h>

#include "clock.h"
#include "gpio.h"
#include "I2C.h"

#include "timer.h"

/*************************** Defines ****************************/


/*************************** Globals ****************************/

/*************************** main function ****************************/
int main(void) {
    __enable_irq();
    ClockSet(_16MHz);
    printf("clock : %d\n", SystemCoreClock);
    TimerInit();

    RCC->IOPENR |= RCC_IOPENR_GPIOCEN; // enable the clock to GPIOC
    gpio_init(GPIOC,  GPIO_MODE_OUTPUT, 6); // set up the built in LED
    
    // initialize the TIM2 RCC clock
    RCC->APBENR1 |= RCC_APBENR1_TIM2EN;
    // set 1 tick to 1 us
    TIM2->PSC = 16 - 1;
    TIM2->ARR = 0xFFFF;
    TIM2->DIER |= TIM_DIER_UIE; // enable update interrupt
    TIM2->CR1 |= TIM_CR1_CEN;
    // enable the TIM2 interrupt
    NVIC_EnableIRQ(TIM2_IRQn);
    // enable the counter

      


    char data = 56;

/*************************** main loop ****************************/
    while(1) {// loop forever

    


    }
}
/*************************** Functions ****************************/

/*************************** Interrupts ****************************/

void TIM2_IRQHandler(void) {
    // clear the interrupt flag
    TIM2->SR &= ~TIM_SR_UIF;
    // toggle the LED
    GPIOC->ODR ^= (1 << 6);
    

    // clear the interrupt flag
    NVIC_ClearPendingIRQ(TIM2_IRQn);
}


/*************************** End of file ****************************/
