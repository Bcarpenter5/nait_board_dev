#include "clock.h"
#include <stdint.h>
#include <stdio.h>

volatile uint64_t  msTicks = 0;

void(*_callback)(void) = 0;

void ChangePll(int n, int m, int r) {
    // Fvco = Fpllin * (N/M)


    // lets disable the PLL
    RCC->CR &= ~ RCC_CR_PLLON;
    while(RCC->CR & RCC_CR_PLLRDY);// wait for the PLL to be ready

    // lets select our clock source
    RCC->PLLCFGR |= 0b10 << RCC_PLLCFGR_PLLSRC_Pos; // set the clock source to HSE

    //   N
    // -----
    //       this number must range from 8 to 86
    // and HSE_fx * N must be between 64 and 344 MHz

    //  M
    //----
    //      this number must range from 1 to 8
    // and HSE_fx*N  / M must be between 4 and 16 MHz


    //// lets set this for 20 MHz
    // with a 16MHz HSI
    // 16 * 10 = 160
    //  / 4 = 40
    //  / 2 = 20
    //  lets set n to 20 and m to 4


    // in the M the prescaler is m-1 ie 0 = 1

    // lets set the PLLN and PLLM and PLLR values
    RCC->PLLCFGR &= ~(0b1111111 << RCC_PLLCFGR_PLLN_Pos); // clear the n value
    RCC->PLLCFGR |=  n << RCC_PLLCFGR_PLLN_Pos; // set the n value
    RCC->PLLCFGR &= ~(0b111 << RCC_PLLCFGR_PLLM_Pos); // clear the m value
    RCC->PLLCFGR |= --m << RCC_PLLCFGR_PLLM_Pos; // set the m value
    RCC->PLLCFGR &= ~(0b111 << RCC_PLLCFGR_PLLR_Pos); // clear the r value
    RCC->PLLCFGR |= --r << RCC_PLLCFGR_PLLR_Pos; // set the r value

    // lets enable the clock
    RCC->CR |=  RCC_CR_PLLON;// this will enable the clock

    // wait for the clock to be ready

    while(!(RCC->CR & RCC_CR_PLLRDY)); // wait for the PLL to be ready

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
    RCC->CFGR |= 2 << RCC_CFGR_SW_Pos; // set the system clock to PLL 

    //this is the cmos function
    SystemCoreClockUpdate();
}

void ClockSet(clock_speed speed) {
    //       _4MHz = 4,
    //       _12MHz = 12,
    //       _16MHz = 16,
    //       _20MHz = 20,
    //       _24MHz = 24,
    //       _32MHz = 32,
    // lets set the clock
                                            //
    //   N
    // -----
    //       this number must range from 8 to 86
    // and HSE_fx * N must be between 64 and 344 MHz

    //  M
    //----
    //      this number must range from 1 to 8
    // and HSE_fx*N  / M must be between 4 and 16 MHz

    // R = 2 - 8 divider for PLLRCLK


    switch (speed) {
        case _4MHz:
            // 16 * 10 = 160 / 5 / 8 = 4
            ChangePll(10, 5, 8);
            break;
        case _12MHz:
            //16 * 15 = 240 / 5 / 4 = 12
            ChangePll(15, 5, 4);
            break;
        case _16MHz:
            // this is the default speed
            return;
            break;
        case _20MHz:
            //16 * 10 = 160 / 4 / 2 = 20
            ChangePll(10, 4, 2);
            break;
        case _24MHz:
            //16 * 12 = 192 / 4 / 2 = 24
            ChangePll(12, 4, 2);
            break;
        case _32MHz:
            //16 * 16 = 256 / 4 / 2 = 32
            ChangePll(16, 4, 2);
            break;
        default:
            break;
    }
    printf("SystemCoreClock = %d\n", SystemCoreClock);
}




void TimerInitCallback(void (*callback)(void)){
    TimerInit();
    _callback = callback;
}

void TimerInit(void){
    msTicks = 0;
    // lets set the systick timer
    SysTick_Config(SystemCoreClock / 1000); // 1 microsecond ticks
}
void DelayMs(unsigned int ms){
    unsigned int curTicks = msTicks;
    while((msTicks - curTicks) < ms);
}
void SysTick_Handler(void){
    msTicks++;
    if(_callback != 0){
        _callback();
    }
}
