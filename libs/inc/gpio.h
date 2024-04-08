#include "stm32g031xx.h"


#ifndef GPIO_H
#define GPIO_H
typedef enum {
    GPIO_MODE_INPUT = 0,
    GPIO_MODE_OUTPUT = 1,
    GPIO_MODE_ALTERNATE = 2,
    GPIO_MODE_ANALOG = 3
} gpio_mode;

typedef enum SwStateTypedef__
{
  Idle,
  Pressed,
  Held,
  Released,
}SwState;

typedef enum {
    GPOI_USART = 7,
    
} gpio_alternate_function;



//*************************************************************************
//  @Method: void gpio_init(GPIO_TypeDef *port,gpio_mode mode,uint8_t pin);
//  @Purpose: init a gpio pin
//  @Args: GPIO_TypeDef *port -- the port
//       @gpio_mode mode -- a GPIO_MODE_XXX enum
//       @uint8_t pin -- pin number 
//  @returns: void 
//*************************************************************************
void gpio_init(GPIO_TypeDef *port,gpio_mode mode,uint8_t pin);

//*************************************************************************
//  @Method: void GPIO_InitAlternateF(GPIO_TypeDef*, uint16_t, uint16_t);
//  @Purpose: init a gpio pin to alternate function
//  @Args: GPIO_TypeDef* -- the port
//       @ uint16_t -- 
//       @ uint16_t -- 
//  @returns: void
//*************************************************************************
void GPIO_InitAlternateF(GPIO_TypeDef* port, uint16_t pin, uint16_t function);

//*************************************************************************
//  @Method: void gpio_set(GPIO_TypeDef *port,uint8_t pin);
//  @Purpose: set a gpio pin to high
//  @Args: GPIO_TypeDef *port -- the port
//       @uint8_t pin -- pin number
//  @returns: void 
//*************************************************************************
void gpio_set(GPIO_TypeDef *port,uint8_t pin);

//*************************************************************************
//  @Method: void gpio_reset(GPIO_TypeDef *port,uint8_t pin);
//  @Purpose: set a gpio pin to low
//  @Args: GPIO_TypeDef *port -- the port
//       @uint8_t pin -- pin number
//  @returns: void
//*************************************************************************
void gpio_reset(GPIO_TypeDef *port,uint8_t pin);

//*************************************************************************
//  @Method: void gpio_toggle(GPIO_TypeDef *port,uint8_t pin);
//  @Purpose: toggle a gpio pin
//  @Args: GPIO_TypeDef *port -- the port
//       @uint8_t pin -- pin number
//  @returns: void
//*************************************************************************
void gpio_toggle(GPIO_TypeDef *port,uint8_t pin);

//*************************************************************************
//  @Method: uint8_t gpio_read(GPIO_TypeDef *port,uint8_t pin);
//  @Purpose: read a gpio pin
//  @Args: GPIO_TypeDef *port -- the port
//       @uint8_t pin -- pin number
//  @returns: uint8_t -- 0 or 1
//*************************************************************************
uint8_t gpio_read(GPIO_TypeDef *port,uint8_t pin);


//*************************************************************************
//  @Method: void gpio_IE(GPIO_TypeDef *port,uint8_t pin,uint8_t edge);
//  @Purpose: set a gpio pin to interrupt
//  @Args: GPIO_TypeDef *port -- the port
//       @uint8_t pin -- pin number
//       @uint8_t edge -- 0:rising edge 1:falling edge
//  @returns: void
//*************************************************************************
void gpio_IE(GPIO_TypeDef *port,uint8_t pin,uint8_t edge);

//*************************************************************************
//  @Method: SwState Sw_Process(SwState* state, GPIO_TypeDef* port, uint8_t pin){
//  @Purpose: process the state of a switch
//  @Args: SwState* state -- the state of the switch
//       @ GPIO_TypeDef* port -- the port
//       @ uint8_t pin -- pin number
//  @returns: SwState
//*************************************************************************
SwState Sw_Process(SwState* state, GPIO_TypeDef* port, uint8_t pin);

#endif
