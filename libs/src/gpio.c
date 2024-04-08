#include "gpio.h"
#include "stm32g031xx.h"
#include <stdlib.h>



void gpio_init(GPIO_TypeDef* port,gpio_mode mode,uint8_t pin){
    if(port == GPIOA){
        RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
    }
    else if(port == GPIOB){
        RCC->IOPENR |= RCC_IOPENR_GPIOBEN;
    }
    else if(port == GPIOC){
        RCC->IOPENR |= RCC_IOPENR_GPIOCEN;
    }
    else if(port == GPIOD){
        RCC->IOPENR |= RCC_IOPENR_GPIODEN;
    }
    else if(port == GPIOF){
        RCC->IOPENR |= RCC_IOPENR_GPIOFEN;
    }

    // Enable clock for port
    port->MODER &= ~(3 << (pin*2));
    port-> MODER |= (mode << (pin*2));
    // Set mode for pin
}

// this is rippped off from Carlos !!!!!!!!!!!!
// changed alot though
void GPIO_InitAlternateF(GPIO_TypeDef* port, uint16_t pin, uint16_t function)
{
    gpio_init(port,GPIO_MODE_ALTERNATE,pin);

    if(pin <= 7)
    {
      port->AFR[0] &= ~(0xF << (pin*4));     //Clear AF setting
      port->AFR[0] |= function << (pin*4);   //Write AF
    }
    else if(pin < 16)
    {
      port->AFR[1] &= ~(0xF << ((pin-8)*4));     //Clear AF setting
      port->AFR[1] |= function << ((pin-8)*4);   //Write AF      
    }
}



void gpio_set(GPIO_TypeDef *port,uint8_t pin)
{
    port->ODR |= (1 << pin);
}

void gpio_reset(GPIO_TypeDef *port,uint8_t pin)
{
    port->ODR &= ~(1 << pin);
}

void gpio_toggle(GPIO_TypeDef *port,uint8_t pin)
{
    port->ODR ^= (1 << pin);
}

uint8_t gpio_read(GPIO_TypeDef *port,uint8_t pin)
{
    return (port->IDR & (1 << pin));
}



SwState Sw_Process(SwState* state, GPIO_TypeDef* port, uint8_t pin){
    if(gpio_read(port, pin)){
        if (*state == Idle) {
            *state = Pressed;
        }
        else {
            *state = Held;
        }
    }
    else{
        if (*state == Held) {
            *state = Released;
        }
        else {
            *state = Idle;
        }
    }
    return *state;
}
