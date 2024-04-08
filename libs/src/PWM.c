#include "PWM.h"
#include "gpio.h"
#include "stm32g031xx.h"



char PWM_TIM1_Init(uint16_t period, uint16_t dutyCycle){

    GPIO_InitAlternateF(GPIOA, 8, 2);
    // We must enable the clock for the timer in order to use it
    RCC->APBENR2 |= RCC_APBENR2_TIM1EN;
    // Check if TIM1 clock enable was successful
    if ((RCC->APBENR2 & RCC_APBENR2_TIM1EN) != RCC_APBENR2_TIM1EN) {
        return 0;
    }
    return PWM_Init(TIM1, period, dutyCycle);
}




char  PWM_Init(TIM_TypeDef* timer , uint16_t period, uint16_t dutyCycle){
    // Configure TIM1 for PWM settings  // these are default settings so not technically needed
    timer->CR1 &= ~TIM_CR1_DIR; // Set timer to count up
    timer->CR1 &= ~TIM_CR1_CMS; // Set timer to edge-aligned mode


    //enable the timer
    timer->CR1 |= TIM_CR1_CEN;


    // Configure PWM mode 1 or 2, this requires 0110 or 0111 in the OC1M and OC2M bits
    // this if for OC1(output compare 1)
    timer->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
    timer->CCMR1 |= TIM_CCMR1_OC1M_0; // clear the lowest bit

    // Enable preload registers || this holds the settings until the UG bit is set
    timer->CCMR1 |= TIM_CCMR1_OC1PE; // Enable preload register 
    //this is also needed
    timer->CR1 |= TIM_CR1_ARPE; // Enable auto-reload preload register
    
    // Set polarity to active high
    timer->CCER |= TIM_CCER_CC1P; // Set active high polarity
    timer->CCER |= TIM_CCER_CC1E;  // Enable output
    timer->CCER |= TIM_CCER_CC1NE; // Enable complementary output // as second output with same settings

    // this is the UG bit !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    timer->EGR |= TIM_EGR_UG; // Update registers
                             //
    // Set period, duty cycle, and update registers
    timer->PSC = 0;     // Set prescaler to 0 
    timer->ARR = period;   // Set auto-reload register to 1000 (Period)
    // set duty cycle for output 1 and 2
    timer->CCR1 = dutyCycle;   // Set duty cycle to 50%

    // Enable timer after all configurations are done
    timer->EGR |= TIM_EGR_UG; // Update registers

    // Enable main output
    timer->BDTR |= TIM_BDTR_MOE;
    return 1;
}

void PWM_SetDutyCycle(TIM_TypeDef* timer,  uint16_t dutyCycle){
    
        // Set duty cycle for output 1 and 2
        timer->CCR1 = dutyCycle;   // Set duty cycle to 50%
    
        // Enable timer after all configurations are done
        timer->EGR |= TIM_EGR_UG; // Update registers
    
        // Enable main output
        timer->BDTR |= TIM_BDTR_MOE;
    }

void PWM_SetPeriod(TIM_TypeDef* timer, uint16_t period){
    timer->ARR = period;   // Set auto-reload register to 1000 (Period)
    timer->EGR |= TIM_EGR_UG; // Update registers
}

int  PWM_startStop(TIM_TypeDef* timer, PWM_State go){
    if(go == PWM_Start){
        timer->CR1 |= TIM_CR1_CEN;
        return 1;
    }
    else{
        timer->CR1 &= ~TIM_CR1_CEN;
        return 0;
    }
}
