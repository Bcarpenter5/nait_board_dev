#include "stm32g031xx.h"
#include "I2C.h"
#include "gpio.h"
#include <stdio.h>
#include "clock.h"



void I2C_init(void){


    RCC->APBENR1 |= RCC_APBENR1_I2C1EN; // Enable clock for I2C1
    
    GPIO_InitAlternateF(GPIOA, 9, 6); // set pin d5/pa_9 is scl
    GPIOA->OTYPER |= (1 << 9); // set pin d5/pa_9 is open drain
    GPIOA->OSPEEDR |= (3 << 18); // set pin d5/pa_9 is high speed
    // GPIOA->PUPDR |= (1 << 18); // set pin d5/pa_9 is pull up

    GPIO_InitAlternateF(GPIOA, 10, 6); // set pin d4/pa_10 is sda
    GPIOA->OTYPER |= (1 << 10); // set pin d4/pa_10 is open drain
    GPIOA->OSPEEDR |= (3 << 20); // set pin d4/pa_10 is high speed
    // GPIOA->PUPDR |= (1 << 20); // set pin d4/pa_10 is pull up


    I2C1->CR1 &= ~I2C_CR1_PE; // Disable I2C1

    // the settings for 100kHz on a 16MHz clock
    uint32_t presc = 0x3 << I2C_TIMINGR_PRESC_Pos;
    uint32_t scll = 0x13 << I2C_TIMINGR_SCLL_Pos;
    uint32_t sclh = 0xF << I2C_TIMINGR_SCLH_Pos;
    uint32_t sdadel = 0x2 << I2C_TIMINGR_SDADEL_Pos;
    uint32_t scldel = 0x4 << I2C_TIMINGR_SCLDEL_Pos;
    I2C1->TIMINGR = scll | sclh | sdadel | scldel | presc;


    I2C1->CR2 |= I2C_CR2_HEAD10R; // 10 bit addressing mode

    // now enable the I2C1 peripheral
    I2C1->CR1 |= I2C_CR1_PE; // Enable I2C1
}

void I2C_address(I2C_TypeDef *I2Cx, uint8_t adress, i2c_RW rw){
    I2Cx->CR2 &= ~I2C_CR2_SADD_Msk;
    I2Cx->CR2 |= adress << 1;

    I2Cx->CR2 &= ~I2C_CR2_RD_WRN_Msk;
    I2Cx->CR2 |= rw << I2C_CR2_RD_WRN_Pos;

    I2Cx->CR2 &= ~I2C_CR2_NBYTES_Msk;
    I2Cx->CR2 |= 0 << I2C_CR2_NBYTES_Pos;

    I2Cx->CR2 &= ~I2C_CR2_RELOAD_Msk;
    I2Cx->CR2 &= ~I2C_CR2_AUTOEND_Msk;

    // I2Cx->CR2 |= I2C_CR2_START;
    //
    //
    // // set reload and autoend to zero
    // while(!(I2Cx->ISR & I2C_ISR_TXE));
    //
    // // send stop condition
    //  I2Cx->CR2 |= I2C_CR2_STOP;
}



void I2C_write(I2C_TypeDef *I2Cx, uint8_t adress, uint8_t data, i2c_Stop stop){
    I2C_address(I2Cx, adress, I2C_WRITE);
    
    I2Cx->CR2 &= ~I2C_CR2_NBYTES_Msk;
    I2Cx->CR2 |= 1 << I2C_CR2_NBYTES_Pos;

    I2Cx->CR2 |= I2C_CR2_START;
    printf("waiting for txe, sending %x\n", data);
    while(!(I2Cx->ISR & I2C_ISR_TXE));
    printf("txe\n");
    I2Cx->TXDR = data;
    printf("waiting for tc\n");
    while(!(I2Cx->ISR & I2C_ISR_TC));
    printf("tc\n");
    if(stop == I2C_STOP){
        I2Cx->CR2 |= I2C_CR2_STOP;
    }
}

void I2C_CommandWrite(I2C_TypeDef *I2Cx, uint8_t adress, uint8_t memadress, uint8_t data){
    DelayMs(10);
    I2C_address(I2Cx, adress, I2C_WRITE);
    I2Cx->CR2 &= ~I2C_CR2_NBYTES_Msk;
    I2Cx->CR2 |= 2 << I2C_CR2_NBYTES_Pos;

    I2Cx->CR2 |= I2C_CR2_START;
    while(!(I2Cx->ISR & I2C_ISR_TXE));
    I2Cx->TXDR = memadress;
    while(!(I2Cx->ISR & I2C_ISR_TXE));
    I2Cx->TXDR = data;
    while(!(I2Cx->ISR & I2C_ISR_TC));
    I2Cx->CR2 |= I2C_CR2_STOP;
}


void I2C_MemWrite(I2C_TypeDef *I2Cx, uint8_t adress, uint8_t memadress, uint8_t *data, uint8_t length){

    I2C_address(I2Cx, adress, I2C_WRITE);


    I2Cx->CR2 |= (length+1) << I2C_CR2_NBYTES_Pos;
    // I2Cx->CR2 |= I2C_CR2_RELOAD;
    // I2Cx->CR2 |= I2C_CR2_AUTOEND;
    I2Cx->CR2 |= I2C_CR2_START;
    while(!(I2Cx->ISR & I2C_ISR_TXE));
    I2Cx->TXDR = memadress;
    for(uint8_t i = 0; i < length; i++){
        while(!(I2Cx->ISR & I2C_ISR_TXE));
        I2Cx->TXDR = data[i];
    }
    while(!(I2Cx->ISR & I2C_ISR_TC));
    I2Cx->CR2 |= I2C_CR2_STOP;

}





void I2C_AutoWrite(I2C_TypeDef *I2Cx, uint8_t adress,uint8_t *data, uint8_t length){
    I2C_address(I2Cx, adress, I2C_WRITE);
    I2Cx->CR2 |= length << I2C_CR2_NBYTES_Pos;
    I2Cx->CR2 |= I2C_CR2_RELOAD;
    I2Cx->CR2 |= I2C_CR2_START;
    for(uint8_t i = 0; i < length; i++){
        while(!(I2Cx->ISR & I2C_ISR_TXE));
        I2Cx->TXDR = data[i];

    }
    while(!(I2Cx->ISR & I2C_ISR_TC));
    I2Cx->CR2 |= I2C_CR2_STOP;
}






