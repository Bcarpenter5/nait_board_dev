//////////////////////////////////////////////////////////////////////////////////////
// Project:  IRTimer.hpp -- Brandon's implimentation
//  -- rip off of the IRTimer.hpp for the IR sensor 
//  -- see the GitHub for the original and legal stuff
//       
//       
// Author: Brandon Carpenter 
//       
//       
//       
// updates: 
//       created: 02/21/2024
//       
//       
//////////////////////////////////////////////////////////////////////////////////////
///

// NO GUARD here, we have the GUARD below with #ifdef _IRSND_H_ and #ifdef _IRMP_H_.
#include "IRTimer.h"
#include "system_stm32g0xx.h"
#include "irsnd.h"


#if !defined(TIMER_DECLARED)
#define TIMER_DECLARED
#endif

// define the timer stuff ie interupts etc in this part




#if defined(_IRMP_H_)
// we compile for irmp
#undef IR_INTERRUPT_FREQUENCY
#define IR_INTERRUPT_FREQUENCY      F_INTERRUPTS                // define frequency for receive

#elif defined(_IRSND_H_)
// we compile for irsnd
#undef IR_INTERRUPT_FREQUENCY
#define IR_INTERRUPT_FREQUENCY      IRSND_INTERRUPT_FREQUENCY   // define frequency for send

#endif // defined(_IRMP_H_)

// The eclipse formatter has problems with // comments in undefined code blocks
// !!! Must be without comment and closed by @formatter:on
// @formatter:off
void initIRTimerForReceive(void)
{
// BluePill in 2 flavors
    // http://dan.drown.org/stm32duino/package_STM32duino_index.json
    /* sReceiveAndSendInterruptTimer.setMode(TIMER_CH1, TIMER_OUTPUT_COMPARE);
    sReceiveAndSendInterruptTimer.setPrescaleFactor(1);
    sReceiveAndSendInterruptTimer.setOverflow(F_CPU / IR_INTERRUPT_FREQUENCY);
    //sReceiveAndSendInterruptTimer.setPeriod(1000000 / IR_INTERRUPT_FREQUENCY);
    sReceiveAndSendInterruptTimer.attachInterrupt(TIMER_CH1, irmp_timer_ISR);
    sReceiveAndSendInterruptTimer.refresh();                                  // Set the timer's count to 0 and update the prescaler and overflow values. */

    // initialize the TIM2 RCC clock
    RCC->APBENR1 |= RCC_APBENR1_TIM2EN;
    // set 1 tick to 1 us
    TIM2->CR1 &= ~TIM_CR1_CEN;
    TIM2->PSC = 1-1;
    TIM2->ARR = SystemCoreClock / F_INTERRUPTS - 1;
    TIM2->DIER |= TIM_DIER_UIE; // enable update interrupt
    //TIM2->CR1 |= TIM_CR1_CEN;
    // enable the TIM2 interrupt
    NVIC_EnableIRQ(TIM2_IRQn);
    // enable the counter
}
void initIRTimerForSend(void){
    // initialize the TIM2 RCC clock
    RCC->APBENR1 |= RCC_APBENR1_TIM3EN;
    // set 1 tick to 1 us
    TIM3->CR1 &= ~TIM_CR1_CEN;
    TIM3->PSC = 1-1;
    TIM3->ARR = SystemCoreClock / F_INTERRUPTS - 1;
    TIM3->DIER |= TIM_DIER_UIE; // enable update interrupt
    //TIM3->CR1 |= TIM_CR1_CEN;
    // enable the TIM2 interrupt
    NVIC_EnableIRQ(TIM3_IRQn);
    // enable the counter
}

// @formatter:on
#if !defined(TIMER_FUNCTIONS_DEFINED)
#define TIMER_FUNCTIONS_DEFINED
#endif
/** Temporarily storage for timer register*/
uint32_t sTimerOverflowValue;


/*
 * If we do not use receive, we have no timer defined at the first call of this function.
 * But for AVR saving the timer settings is possible anyway, since it only consists of saving registers.
 * This helps cooperation with other libraries using the same timer.
 */
void storeIRTimer(void) {

}

/*
 * NOT used if IRMP_ENABLE_PIN_CHANGE_INTERRUPT is defined
 * Initialize timer to generate interrupts at a rate F_INTERRUPTS (15000) per second to poll the input pin.
 */
void disableIRTimerInterrupt(void) {

    // sReceiveAndSendInterruptTimer.detachInterrupt()j
    // sReceiveAndSendInterruptTimer.setMode(LL_TIM_CHANNEL_CH1, TIMER_DISABLED);
    TIM2->CR1 &= ~TIM_CR1_CEN; // disable the counter
    NVIC_DisableIRQ(TIM2_IRQn); // disable the TIM2 interrupt
    

}

// used by AllProtocols example
void enableIRTimerInterrupt(void) {

    // http://dan.drown.org/stm32duino/package_STM32duino_index.json
    // sReceiveAndSendInterruptTimer.setMode(TIMER_CH1, TIMER_OUTPUT_COMPARE);
    // sReceiveAndSendInterruptTimer.attachInterrupt(TIMER_CH1, irmp_timer_ISR);
    // sReceiveAndSendInterruptTimer.refresh(); // Set the timer's count to 0 and update the prescaler and overflow values.
    TIM2->CR1 |= TIM_CR1_CEN; // enable the counter
    NVIC_EnableIRQ(TIM2_IRQn); // enable the TIM2 interrupt

}


/*
 * If both irmp and irsnd are used, compile it only once in the second step, when all variables are declared.
 */
#if (! defined(USE_ONE_TIMER_FOR_IRMP_AND_IRSND) || ( defined(_IRMP_ARDUINO_EXT_H) && defined(_IRSND_ARDUINO_EXT_H) )) && ! defined(ISR_DEFINED)
#  if !defined(ISR_DEFINED)
#define ISR_DEFINED
#  endif
#endif
/*
 * ISR is active while signal is sent AND during the trailing pause of IR frame
 * Called every 13.5us
 * Bit bang requires 5.9 us. 5 us for 16 push and 16 pop etc. and 0.9 us for function body
 * Together with call of irsnd_ISR() 10.5 us (frame) or 9.4 (trailing pause) - measured by scope
 * We use TIMER2_COMPB_vect to be compatible with tone() library
 */

void irmp_timer_ISR(void)
// Start of ISR
{


    static uint8_t sDivider;        // IR signal toggle rate is 2 (4) times IRSND call rate


    /*
     * Send part of ISR
     
   . if(irsnd_busy) {
        if (irsnd_is_on)
        {
            if(sDivider & 0x01) // true / inactive if sDivider is 3 or 1, so we start with active and end with inactive
            {

            } else {
            }


        } else {
            // irsnd off here
        }

        /*
         * Call irsnd_ISR() every second (fourth) call if current LED state is inactive
         
        if (--sDivider == 0)
        {
            // This in turn calls irsnd_on() or irsnd_off(). Empty call requires additional 0.7 us.
            if (!irsnd_ISR())
            {
                sDivider = 1; // to call irsnd_ISR() directly at next interrupt
            } else {
                sDivider = 4;
            }
        }
    } //*/
    irsnd_ISR();

}


void TIM2_IRQHandler(void) {
    // clear the interrupt flag
    TIM2->SR &= ~TIM_SR_UIF;
    NVIC_ClearPendingIRQ(TIM2_IRQn);

    irmp_ISR();
}
void TIM3_IRQHandler(void) {
    TIM3->SR &= ~TIM_SR_UIF;
    NVIC_ClearPendingIRQ(TIM3_IRQn);

    irmp_timer_ISR();
}






