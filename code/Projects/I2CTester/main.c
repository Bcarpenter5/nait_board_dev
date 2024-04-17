#include "stm32g031xx.h"
#include <stdio.h>

#include "clock.h"
#include "gpio.h"
#include "I2C.h"
#include "ssd1306.h"

/*************************** Defines ****************************/
// nuclio board has an external led on pin d6/pc_6
#define LED_PIN 6
// this pin is connected to the external led
#define D9 8 
#define PA8 D9
// this pin is connected to a button
#define A0 0 
#define PA0 A0
// set pin d5/pa_9 is scl
#define D4 10
#define PA10 D4
// set pin d4/pa_10 is sda
#define D5 9
#define PA9 D5



/*************************** Globals ****************************/

/*************************** main function ****************************/
int main(void) {
    __enable_irq();
    ClockSet(_16MHz);
    printf("clock : %d\n", SystemCoreClock);
    TimerInit();
    // I2C_init();

    // GPIO init manual version for testing purposes
    GPIO_InitAlternateF(GPIOA, 9, 6); // set pin d5/pa_9 is scl
    GPIOA->OTYPER |= (1 << 9); // set pin d5/pa_9 is open drain
    GPIOA->OSPEEDR |= (3 << 18); // set pin d5/pa_9 is high speed
    GPIO_InitAlternateF(GPIOA, 10, 6); // set pin d4/pa_10 is sda
    GPIOA->OTYPER |= (1 << 10); // set pin d4/pa_10 is open drain
    GPIOA->OSPEEDR |= (3 << 20); // set pin d4/pa_10 is high speed

    RCC->APBENR1 |= RCC_APBENR1_I2C1EN; // Enable clock for I2C1

    I2C1->CR1 &= ~I2C_CR1_PE; // Disable I2C1
                              //
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

    // pins already configured in main.c
    //ssd1306_Init();
    //ssd1306_DrawPixel(10,10,White);
   //ssd1306_UpdateScreen();
    
    
    // I2C_address(I2C1, 0x79, I2C_READ);
    //

    // set the adress of the slave
    I2C_TypeDef *I2Cx = I2C1;
    uint32_t adress = (0x3C);
    uint32_t rw = I2C_WRITE;



    // I2C_address(I2C1, 0x79, I2C_WRITE);
    printf("entering the Main loop\n");
    ssd1306_Init();



    ssd1306_SetCursor(12, 25);
    ssd1306_WriteString("Get Ligma", Font_11x18, White);
    ssd1306_UpdateScreen();





    char data = 56;

/*************************** main loop ****************************/
    while(1) {// loop forever

    DelayMs(1000);


    }
}
/*************************** Functions ****************************/

/*************************** Interrupts ****************************/


/*************************** End of file ****************************/
