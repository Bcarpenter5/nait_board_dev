/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Generic application start

*/

#include <stdio.h>
#include <stdlib.h>
#include "stm32g031xx.h"
#include "clock.h"

/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/


//void ChangePll(int n, int m, int r); // this is the function to change the pll settings and the system clock

                                     
void Hal_Init(void);                 // this is the function to enable the clock for the gpio

int main(void) {
    long delay;
    char msg[12];


    // **************************************** setup ****************************************
    Hal_Init(); // enable the clock for the gpio and enable the interupts

    // set the clock to 20MHz
    // ChangePll(10,4,2);

    // set the clock to 20MHz
    ClockSet(_20MHz);


    //set systemtimer to 500 ms


    printf("SystemClock: %d \n\r", SystemCoreClock);
     
// **************************************** loop ****************************************
  while (1){
  delay = 2000000;
  while(--delay);
  printf("HelloWorld \n\r");
//  GPIOC->ODR ^= GPIO_ODR_OD6_Msk;
  }
}


// original clock function
/* void ChangePll(int n, int m, int r) {


    RCC->CR &= ~ RCC_CR_PLLON;
    while(RCC->CR & RCC_CR_PLLRDY);
    // Fvco = Fpllin * (N/M)
    // Fpllin = the clock input frequency
    

    // lets select our clock source
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSI; // set the source to HSI


    // N
    //       this number must range from 8 to 86
    // and HSE_fx * N must be between 64 and 344 MHz
    //
    // M
    //      this number must range from 1 to 8
    // and HSE_fx*N  / M must be between 4 and 16 MHz
    //
    //// lets set this for 20 MHz
    // with a 16MHz HSI
    // 16 * 10 = 160
    //  / 4 = 40
    //  / 2 = 20
    //  lets set n to 20 and m to 4
    //
    //
    // in the M the prescaler is m-1 ie 0 = 1



    RCC->PLLCFGR |= n << RCC_PLLCFGR_PLLN_Pos; // set the n value
    RCC->PLLCFGR |= --m << RCC_PLLCFGR_PLLM_Pos; // set the m value
    RCC->PLLCFGR |= r << RCC_PLLCFGR_PLLR_Pos; // set the r value


    // lets enable the clock
    RCC->CR |=  RCC_CR_PLLON;// this will enable the clock

    // wait for the clock to be ready

    printf("waiting for PLLRDY \n\r");
    while(!(RCC->CR & RCC_CR_PLLRDY)); // wait for the PLL to be ready
    printf("PLLRDY \n\r");

    RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN; // enable the PLLR output
    

    //set this as the system clock
    //kSW[2:0]: System clock switch
    //
    // This bitfield is controlled by software and hardware. The bitfield selects the clock for
    // SYSCLK as follows:
    // 000: HSISYS
    // 001: HSE
    // 010: PLLRCLK
    // 011: LSI
    // 100: LSE
    // Others: Reserved
    // The setting is forced by hardware to 000 (HSISYS selected) when the MCU exits Stop,
    // Standby, or Shutdown mode, or when the setting is 001 (HSE selected) and HSE oscillator
    // failure is detected.
    //
    RCC->CFGR |= 1 << RCC_CFGR_SW_Pos; // set the system clock to PLL 

    //this is the cmos function
    SystemCoreClockUpdate();
} */

void Hal_Init(void) {
    // enable the clock for the GPIOC
    RCC->IOPENR |= RCC_IOPENR_GPIOCEN_Msk;
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN_Msk;

    // enable interupts
    __enable_irq();
    NVIC_EnableIRQ(EXTI0_1_IRQn);

    // set GPIOA pin 0 as input with interupt enabled
    GPIOA->MODER &=~GPIO_MODER_MODE0_Msk;

    // set the interupt
    EXTI->IMR1 |= EXTI_IMR1_IM0_Msk;
    EXTI->RTSR1 |= EXTI_RTSR1_RT0_Msk;
    EXTI->EXTICR[0] |= 0x00;// EXTI0 is connected to PA0

    // set GPIOC pin 6 as output
    GPIOC->MODER &=~GPIO_MODER_MODE6_Msk;
    GPIOC->MODER |= GPIO_MODER_MODE6_0;

    // enable HSI16
    RCC->CR |= RCC_CR_HSION;// this will enable the clock
                            // wait for the clock to be ready
    while(!(RCC->CR & RCC_CR_HSIRDY));
    // enable instruction prefetch, instruction cache, data cache
    FLASH->ACR |= FLASH_ACR_PRFTEN_Msk | FLASH_ACR_ICEN_Msk;
    
    // set the latency to 1 wait state
    FLASH->ACR &= ~FLASH_ACR_LATENCY_Msk;
    FLASH->ACR |= FLASH_ACR_LATENCY_0;
}

//******************************************************************** interrupt handler ********************************************************************

void EXTI0_1_IRQHandler(void){

  EXTI->RPR1 |= EXTI_RPR1_RPIF0_Msk; // clear the pending bits for the interupt
  NVIC_ClearPendingIRQ(EXTI0_1_IRQn);

  GPIOC->ODR ^= GPIO_ODR_OD6_Msk;

  printf("interupt \n");
}



/*************************** End of file ****************************/
