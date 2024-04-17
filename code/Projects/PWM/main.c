//////////////////////////////////////////////////////////////////////////////////////
// Project: PWM Test
//       
//       
// Author: Brandon Carpenter 
//       
//       
//       
// updates: 
//       created: 01/19/2024
//       
//       
//////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include "stm32g031xx.h"
#include "gpio.h"
#include "clock.h"
//#include "PWM.h"

int duty = 500;
char mode = 0;


void init_PWM_OUT();// this is a function to set up the PWM output

int main(void) {

    // Enable interrupts globally
    __enable_irq();

    // Configure system clock
    ClockSet(_20MHz);

    // for a min to be PWM out it must be set to alternate function 2
    GPIO_InitAlternateF(GPIOA, 8, 2); // this is D9
    GPIO_InitAlternateF(GPIOA, 9, 2);
    gpio_init(GPIOA, GPIO_MODE_INPUT,0); // set up pin A0 as input

    // We must enable the clock for the timer in order to use it
    RCC->APBENR2 |= RCC_APBENR2_TIM1EN;

    // Check if TIM1 clock enable was successful
    if ((RCC->APBENR2 & RCC_APBENR2_TIM1EN) != RCC_APBENR2_TIM1EN) {
        printf("Error: TIM1 clock enable failed!\n");
        while (1); // You might want to handle this more gracefully
    }
    init_PWM_OUT();
    
    /* // set the TIM1 To PWM input mode
    TIM1->TISEL &= ~TIM_TISEL_TI1SEL; // set to channel 1
    TIM1->CCMR1 &= ~(3<< TIM_CCMR1_CC1S_Pos); // clear the CC1S bits
    TIM1->CCMR1 |= TIM_CCMR1_CC1S_0; // set to input mode
                                     //
    // set the polarity of the input active on rising edge
    TIM1->CCER &= ~TIM_CCER_CC1P; // se
    TIM1->CCER &= ~TIM_CCER_CC1NP_Msk; // enable input

    //set the input of counter 2
    TIM1->CCMR1 &= ~(3<< TIM_CCMR1_CC1S_Pos); // clear the CC1S bits

    // set CC2 to use the same pin as channel 1
    TIM1->CCMR1 &= ~(3<< TIM_CCMR1_CC2S_Pos); // clear the CC1S bits
    TIM1->CCMR1 |= TIM_CCMR1_CC2S_1; // // write 10 to set to input mode */


    TIM1->EGR |= TIM_EGR_UG; // Update registers


           TIM1->CR1 |= TIM_CR1_CEN;// toggle PWM on and off
           TIM1->CCER |= TIM_CCER_CC1E; // toggle output 1









    TimerInit(); // set a background timer to toggle allow dellays


    for(;;) {
        if (gpio_read(GPIOA, 0)) {
          TIM1->CR1 ^= TIM_CR1_CEN;// toggle PWM on and off
          TIM1->CCER ^= TIM_CCER_CC1E; // toggle output 1
            printf("state == %d\n", TIM1->CCER & TIM_CCER_CC1E);

          
            DelayMs(500);
            printf("button pressed\n");
        }
            duty = duty + 1 > 1000 ? 0 : duty + 1;
            TIM1->CCR1 = duty;
            TIM1->EGR |= TIM_EGR_UG; // Update registers
            printf("%d \n",duty);
            printf("%d \n",TIM1->CCR1);
            DelayMs(100);
    }
}


void init_PWM_OUT(){

    // Configure TIM1 for PWM settings  // these are default settings so not technically needed
    TIM1->CR1 &= ~TIM_CR1_DIR; // Set timer to count up
    TIM1->CR1 &= ~TIM_CR1_CMS; // Set timer to edge-aligned mode


    //enable the timer
    TIM1->CR1 |= TIM_CR1_CEN;


    // Configure PWM mode 1 or 2, this requires 0110 or 0111 in the OC1M and OC2M bits
    // this if for OC1(output compare 1)
    TIM1->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
    TIM1->CCMR1 |= TIM_CCMR1_OC1M_0; // clear the lowest bit

    // this is for OC2(output compare 2)
    TIM1->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;
    TIM1->CCMR1 |= TIM_CCMR1_OC2M_0; // clear the lowest bit

    // Enable preload registers || this holds the settings until the UG bit is set
    TIM1->CCMR1 |= TIM_CCMR1_OC1PE; // Enable preload register 
    TIM1->CCMR1 |= TIM_CCMR1_OC2PE; // Enable preload register
    //this is also needed
    TIM1->CR1 |= TIM_CR1_ARPE; // Enable auto-reload preload register
    
    // Set polarity to active high
    // output 1 pin D9 or PA8
    TIM1->CCER &= ~TIM_CCER_CC1P; // Clear polarity bit negative polarity
    TIM1->CCER |= TIM_CCER_CC1E;  // Enable output
    TIM1->CCER |= TIM_CCER_CC1NE; // Enable complementary output // as second output with same settings


    //output 2 pin D5 or PA9
    TIM1->CCER |= TIM_CCER_CC2P; // Positive plarity
    TIM1->CCER |= TIM_CCER_CC2E;  // Enable output
    TIM1->CCER |= TIM_CCER_CC2NE; // Enable complementary output

    // this is the UG bit !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    TIM1->EGR |= TIM_EGR_UG; // Update registers
                             //
    // Set period, duty cycle, and update registers
    TIM1->PSC = 1;     // Set prescaler to 0 
    TIM1->ARR = 1000;   // Set auto-reload register to 1000 (Period)
    // set duty cycle for output 1 and 2
    TIM1->CCR1 = 500;   // Set duty cycle to 50%
    TIM1->CCR2 = 200;   // Set duty cycle to 50%

    // Enable timer after all configurations are done
    TIM1->EGR |= TIM_EGR_UG; // Update registers

    // Enable main output
    TIM1->BDTR |= TIM_BDTR_MOE;
}

/*************************** End of file ****************************/
