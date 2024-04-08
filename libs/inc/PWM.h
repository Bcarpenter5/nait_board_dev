
#include "gpio.h"
#include "stm32g031xx.h"

typedef enum {
    PWM_Start = 1,
    PWM_Stop = 0
} PWM_State;


//*************************************************************************
//  @Method: char PWM_TIM1_Init(uint16_t period, uint16_t dutyCycle){
//  @Purpose: Initialize PWM on TIM1 and channel 1
//  @Args: uint16_t period -- the period of the PWM
//       @ uint16_t dutyCycle -- the duty cycle of the PWM
//  @returns: char -- true if successful, false if not
//*************************************************************************
char PWM_TIM1_Init(uint16_t period, uint16_t dutyCycleA);


//*************************************************************************
//  @Method: void PWM_Init(TIM_TypeDef* , uint8_t channel, uint16_t period, uint16_t dutyCycle);
//  @Purpose: Initialize PWM
//  @Args: TIM_TypeDef*  -- the timer to be used
//       @ uint16_t period -- the period of the PWM
//       @ uint16_t dutyCycle -- the duty cycle of the PWM
//  @returns: bool -- true if successful, false if not
//*************************************************************************
char PWM_Init(TIM_TypeDef* timer , uint16_t period, uint16_t dutyCycle);


//*************************************************************************
//  @Method: void PWM_SetDutyCycle(TIM_TypeDef* timer, uint8_t channel, uint16_t dutyCycle);
//  @Purpose: Set the duty cycle of the PWM
//  @Args: TIM_TypeDef* timer -- the timer to be used
//       @ uint16_t dutyCycle -- the duty cycle of the PWM
//  @returns: void
//*************************************************************************
void PWM_SetDutyCycle(TIM_TypeDef* timer, uint16_t dutyCycle);

//*************************************************************************
//  @Method: void PWM_SetPeriod(TIM_TypeDef* timer, uint16_t period);
//  @Purpose: Set the period of the PWM
//  @Args: TIM_TypeDef* timer -- the timer to be used
//       @ uint16_t period -- the period of the PWM
//  @returns: void
//*************************************************************************
void PWM_SetPeriod(TIM_TypeDef* timer, uint16_t period);

//*************************************************************************
//  @Method: bool PWM_startStop(TIM_TypeDef* timer, uint8_t channel, PWM_State go);
//  @Purpose: Start or stop the PWM
//  @Args: TIM_TypeDef* timer -- the timer to be used
//       @ PWM_State go -- PWM_Start or PWM_Stop
//  @returns: bool
//*************************************************************************
int  PWM_startStop(TIM_TypeDef* timer, PWM_State go);






