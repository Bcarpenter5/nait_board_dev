#include "stm32g031xx.h"

typedef enum clock_speed_
{
    _4MHz = 4,
    _12MHz = 12,
    _16MHz = 16,
    _20MHz = 20,
    _24MHz = 24,
    _32MHz = 32,
} clock_speed;


//*************************************************************************
// @Method: void ChangePll(int n, int m, int r);
// @Purpose: set the PLL to the desired frequency 
// @using the HSI16 clock and set the system clock to the PLLR
// @Formula is: f = (16MHz * (m/n)) / r
//  @int n -- this is the PLLN value
//  @int m -- this is the PLLM value
//  @int r -- this is the PLLR value
//*************************************************************************
void ChangePll(int n, int m, int r);

//*************************************************************************
//  @Method: void ClockChange(clock_speed speed);
//  @Purpose: set the system clock to the desired speed
//  @Args: clock_speed speed -- this is the desired clock speed
//*************************************************************************
void ClockSet(clock_speed speed);

//*************************************************************************
//  @Method: void TimerInit(void);
//  @Purpose: initialize the timer to count milliseconds
//  @Args: void -- no arguments
//  @returns: void
//*************************************************************************
void TimerInit(void);


//*************************************************************************
//  @Method: void TimerInitCallback(void (*callback)(void));
//  @Purpose: set the callback function to be called every millisecond
//  @Args: void (*callback -- this is the function to be called
//  @returns: void
//*************************************************************************
void TimerInitCallback(void (*callback)(void));

//*************************************************************************
//  @Method: void DelayMs(int ms);
//  @Purpose: delay for the desired number of milliseconds
//  @Args: int ms -- this is the number of milliseconds to delay
//  @returns: void
//*************************************************************************
void DelayMs(unsigned int ms);


