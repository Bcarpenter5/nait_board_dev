//////////////////////////////////////////////////////////////////////////////////////
// Project: timer.h
//       
//       
// Author: Brandon Carpenter 
//       
//       
//       
// updates: 
//       created: 02/13/2024
//       
//       
//////////////////////////////////////////////////////////////////////////////////////

#include "stm32g031xx.h"
#include <stdio.h>

//*************************************************************************
//  @Method: void timer_init(void);
//  @Purpose: initialize the timer set the prescaler and the auto reload value
//  @Args: void -- none
//  @returns: void
//*************************************************************************
void timer_init(void);


//*************************************************************************
//  @Method: void delay_ms(uint32_t delay);
//  @Purpose: delay for a given number of milliseconds
//  @Args: uint32_t delay -- the number of milliseconds to delay
//  @returns: void
//*************************************************************************
void delay_ms(uint32_t delay);

//*************************************************************************
//  @Method: void delay_us(uint32_t delay);
//  @Purpose: delay for a given number of microseconds
//  @Args: uint32_t delay -- the number of microseconds to delay
//  @returns: void
//*************************************************************************
void delay_us(uint32_t delay);



