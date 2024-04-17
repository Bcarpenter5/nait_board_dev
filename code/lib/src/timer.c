#include "timer.h"
#include "stm32g031xx.h"

volatile uint32_t msTicks; // counts 1ms timeTicks

void TimerInit(void) {
    // Enable TIM2 clock
    RCC->APBENR1 |= RCC_APBENR1_TIM2EN;
    TIM2->CR1 &= ~ TIM_CR1_CEN;// Disable timer
    // Set prescaler to 1600
    TIM2->PSC = 1600 - 1;
    // Set auto-reload value to 10000-1
    TIM2->ARR = 10000 - 1;
    // Update interrupt enable
    TIM2->DIER |= TIM_DIER_UIE;
    // Enable timer
    TIM2->CR1 |= TIM_CR1_CEN;
    // Enable interrupt in NVIC
    NVIC_EnableIRQ(TIM2_IRQn);
}


void TIM2_IRQHandler(void)
{
  /*
  Clearing a flag always involves 2 steps:
  - Writing a "0" in the flag BIT
  - Clearing pending IRQ  
  */
  TIM2->SR &= ~TIM_SR_UIF_Msk;
  NVIC_ClearPendingIRQ(TIM2_IRQn);
}

