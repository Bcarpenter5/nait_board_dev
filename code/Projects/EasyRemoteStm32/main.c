//////////////////////////////////////////////////////////////////////////////////////
// Project: Accessable Remote
//       
//       
// Author: Brandon Carpenter & Cole Yaremko
//       
//       
//       
// updates: 
//       created: 02/07/2024
//       
//
//ToDo:
//  [] build the main SCI Reciving loop using interupts and a buffer
//  [] build the main menu for the display
//
//
//       
//////////////////////////////////////////////////////////////////////////////////////

#include "IRMP/irsnd.h"
#include "stm32g031xx.h"
#include <stdint.h>
#include <stdio.h>
#include "clock.h"
#include "gpio.h"
#include "I2C.h"
#include "ssd1306.h"
#include "UART.h"

#include "IRMP/irmp.hpp"
#include "IRMP/IRTimer.hpp"
#include "IRMP/irmpSelectAllProtocols.h"

#include "IRMP/irsnd.hpp"
#include "IRMP/irsndSelectAllProtocols.h"

#include "flash.h"



/*************************** Defines ****************************/

// GPIO button pins all on port A and the A 0-7 pins
// this is for A 0-7 a total of 8 pins
#define A0 0
#define A1 1
#define A2 4
#define A3 5
#define A4 12
#define A5 11
#define A6 6
#define A7 7
#define Button_PORT GPIOA

// IR reciver pin on port A pin 8
#define IR_IN_PIN 8 // this is the IR reciver pin
#define IR_in_PORT GPIOA
// IR reciver pin on port B pin 9
#define IR_OUT_PIN 9 // this is the IR reciver pin
#define IR_out_PORT GPIOB

#define ANALYZE

#define Page31 (char*)0x0800F800

#define concat(a, b) a ## b


#define ComPort USART1



/***************************** structs ****************************/

// this is the IRMP data struct || already included in the irmp.hpp
/* typedef struct IRMP_DATA {
   uint8_t protocol;
   uint16_t address;
   uint16_t command;
   uint8_t flags;
   } IRMP_DATA; */


// @Contains the data for the macro
// @num is the macro number
// @data_count is the number of IRMP_DATA structs in the signals array
// @signals is an array of IRMP_DATA structs
typedef struct IRMP_MACRO {
    char macro_num;
    char data_count;
    IRMP_DATA  signals[4];

} IRMP_MACRO;

// holds all macros
IRMP_MACRO macros[20];

// this is the IRMP data struct that we will use to send and recive data
IRMP_MACRO macro_to_save;
char macro_to_save_index = 0;








/*************************** Function Prototypes ****************************/

char get_IRMP_data(IRMP_DATA *data);
void send_IRMP_data(IRMP_DATA *data);
void send_Irmp_Macro(IRMP_MACRO *data);
void program_remote(IRMP_MACRO *macro, char button);
void load_from_flash(void);
void save_to_flash(void);
void TimerCallback(void);
void checkButtons(void);
void display_main_menu(void);
void process_buffer();
void sci_send_IRMP_Macro(IRMP_MACRO *data);

/*************************** Globals ****************************/

// this is a array of the IRMP_MACRO structs


char uart_flag = 0;
volatile char uart_data = 0;

char protocal[10];

// buffer for the SCI
char sci_buffer[20] = {0};
char sci_buffer_index = 0;
char sci_process_flag = 0;


// hold timer values
int timer_value = 0;
char hold_flag = 0;
char program_flag = 0;
//the amount of time to hold the button
const int hold_time = 5000;

// an array of the button pins
const char A[8] = {0, 1,4,5, 12,11,6,7};

// create states for all buttons and have them as idle
SwState btn[8] = {Idle, Idle, Idle, Idle, Idle, Idle, Idle, Idle};






/*************************** main function ****************************/
int main(void) {
    // enable interrupts
    __enable_irq();

    // enable the flash
    RCC->APBENR2 |= RCC_AHBENR_FLASHEN;

    // initiate the timer
    ClockSet(_16MHz);
    printf("clock : %d\n", SystemCoreClock);
    TimerInitCallback(TimerCallback);

    // initiate the display
    I2C_init();
    ssd1306_Init();


    // set the clock for UART1
    RCC->APBENR2 |= 1 << RCC_APBENR2_USART1EN_Pos;
    

    // initialize the sci pins
    GPIO_InitAlternateF(GPIOB, 6,0); 
    GPIO_InitAlternateF(GPIOB, 7,0);

    SYSCFG->CFGR1 &= ~(SYSCFG_CFGR1_PA11_RMP | SYSCFG_CFGR1_PA12_RMP);



    // initiate the GPIO pins 
    gpio_init(GPIOA,GPIO_MODE_INPUT, A0);
    gpio_init(GPIOA,GPIO_MODE_INPUT, A1);
    gpio_init(GPIOA,GPIO_MODE_INPUT, A2);
    gpio_init(GPIOA,GPIO_MODE_INPUT, A3);
    gpio_init(GPIOA,GPIO_MODE_INPUT, A4);
    gpio_init(GPIOA,GPIO_MODE_INPUT, A5);
    gpio_init(GPIOA,GPIO_MODE_INPUT, A6);
    gpio_init(GPIOA,GPIO_MODE_INPUT, A7);


    //initialize the gpio in pin mode thing
    gpio_init(GPIOA,GPIO_MODE_INPUT, 8);
    GPIOA->OSPEEDR |=  (3 << 2*8);

    /// for the gpio b 8 on the portable version
    gpio_init(GPIOB,GPIO_MODE_INPUT, 8);
    GPIOB->OSPEEDR |=  (3 << 2*8);

    // initiate the display with the title
    ssd1306_WriteString("Testing", Font_11x18, White);
    ssd1306_UpdateScreen();

    // initialize the  IRMP reciver and see if we can do some stuff
    irmp_init();
    irsnd_init();
    initIRTimerForReceive();
    initIRTimerForSend();
    disableIRTimerInterrupt();
    // load the macros from the flash
    // macros[0].macro_num = 0;
    // macros[0].data_count = 2;
    // macros[0].signals[0].protocol = 1;
    // macros[0].signals[0].address = 0x1;
    // macros[0].signals[0].command = 0x1;
    // macros[0].signals[0].flags = 0;
    // macros[0].signals[1].protocol = 1;
    // macros[0].signals[1].address = 0x2;
    // macros[0].signals[1].command = 0x3;
    // macros[0].signals[1].flags = 0;
    //

    load_from_flash();







    // display the main menu
    DelayMs(1000);
    display_main_menu();
    
    // the dellay is nessasery for the esp to boot up

    // initiate the UART 
    Vcom_Init(115200, 1);
    // initiate the UART
    UART_Init(ComPort, 115200, 1);

/*************************** main loop ****************************/
    while(1) {// loop forever

        // check the buttons states 
        checkButtons();

        // check the uart buffer
        if(sci_process_flag)
            process_buffer();
    }
}
/*************************** Functions ****************************/


//*************************************************************************
//  @Method: void send_IRMP_data(IRMP_DATA *data){
//  @Purpose: sends the IRMP data to the IRMP reciver
//  @Args: IRMP_DATA *data -- the data that will be used to send the IRMP data
//  @returns: void
//*************************************************************************
void send_IRMP_data(IRMP_DATA *data){

    TIM3->CR1 |= TIM_CR1_CEN; // turn on the irsend timer
    irsnd_send_data(data ,1); // send the data
    TIM3->CR1 &= ~TIM_CR1_CEN;
}

//*************************************************************************
//  @Method: void get_IRMP_data(IRMP_DATA *data){
//  @Purpose: gets the IRMP data and prints it to the display to the ssd1306
//  @Args: IRMP_DATA *data -- the data that will be used to store the IRMP data
//  @returns: void
//*************************************************************************
char get_IRMP_data(IRMP_DATA *data){
    if(irmp_get_data(data)) { // if we have data
        disableIRTimerInterrupt(); // disable the interrupt
        DelayMs(100); // delay for 100 ms
        
        // print the data to the display
        ssd1306_Fill(Black);

        ssd1306_SetCursor(0,0);
        ssd1306_WriteString("IR Recived", Font_6x8, White);

        ssd1306_SetCursor(0,20);
        sprintf(protocal, "Pro: %d", data->protocol);
        ssd1306_WriteString(protocal, Font_6x8, White);

        ssd1306_SetCursor(0,30);
        sprintf(protocal, "Addr: %d", data->address);
        ssd1306_WriteString(protocal, Font_6x8, White);

        ssd1306_SetCursor(0,40);
        sprintf(protocal, "Cmd: %d", data->command);
        ssd1306_WriteString(protocal, Font_6x8, White);

        // update the display
        ssd1306_UpdateScreen();

        // re-enable the interrupt
        enableIRTimerInterrupt();
        return 1;
    }
    return 0;
}

//*************************************************************************
//  @Method: void program_remote(IRMP_MACRO *macro, char button){
//  @Purpose: programs the remote with the IRMP data
//  @Args: IRMP_MACRO *macro -- the macro that will be used to store the data
//       @ char button -- the button that will be used to store the data
//  @returns: void
//*************************************************************************
void program_remote(IRMP_MACRO *macro, char button){

    //display the message
    ssd1306_Fill(Black);
    ssd1306_SetCursor(0,0);
    ssd1306_WriteString("Recive IR", Font_6x8, White);
    ssd1306_SetCursor(0, 20);
    ssd1306_WriteString("Press btn  2 exit", Font_6x8, White);
    ssd1306_UpdateScreen();


    // get the data
    IRMP_DATA irmp_data_for_use;
    int i = 0;

    // wait for the button to be released
    DelayMs(1000);


    //clear the signals
    for(int j = 0; j < 4; j++){
        macro->signals[j].protocol = 0;
        macro->signals[j].address = 0;
        macro->signals[j].command = 0;
        macro->signals[j].flags = 0;
    }

    enableIRTimerInterrupt();

    // get the data
    while(i < 4){
        if(get_IRMP_data(&irmp_data_for_use)){
            // store the data and increment the index
            macro->signals[i++] = irmp_data_for_use;
        }
        // check if the button is pressed
        if(gpio_read(Button_PORT, A[button])){
            break;
        }
    }

    // set the macro number and the data count
    macro->macro_num = button;
    macro->data_count = i;  

    // disable the interrupt
    disableIRTimerInterrupt();
    save_to_flash();
    // display the main menu
    sci_send_IRMP_Macro(macro);

    display_main_menu();
}

//*************************************************************************
//  @Method: void process_buffer(){
//  @Purpose: processes the buffer for the SCI
//  @Args:  -- none
//  @returns: void
//*************************************************************************
void process_buffer(){
    // check if the buffer is send_command or recive_new_command
    IRMP_DATA irmp_data_for_use;


    // copy the buffer to a temp buffer
    char temp_buffer[20];
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, sci_buffer, sizeof(temp_buffer));

    // clear the buffer
    memset(sci_buffer, 0, sizeof(sci_buffer));

 
    // make sure the the first char is a command or shift the buffer to the right
    if (temp_buffer[0] != 'S' && temp_buffer[0] != 'W' && temp_buffer[0] != 'U' && temp_buffer[0] != 'I' ){

          //int  i = 0;
           // printf(temp_buffer);
           // printf("first %d",temp_buffer[0]);
           // if(sizeof(temp_buffer) > 1)
           //     memcpy(temp_buffer, &temp_buffer[1], sizeof(temp_buffer) - 1);

    }





    // clear the buffer index
    sci_process_flag = 0;
    sci_buffer_index =0;

    switch(temp_buffer[0]){
        case 'S':
            // send the command
            printf("sending %d", temp_buffer[1]);
            send_Irmp_Macro(&macros[temp_buffer[1]]);
            break;
        case 'W':
            // write the buffer to the ram then flash
            // memcpy(&macro_to_save, &macros[(macro_to_save.macro_num)], sizeof(macro_to_save));
            memcpy(&macros[(macro_to_save.macro_num)], &macro_to_save, sizeof(macro_to_save));
            // update flash call ---------------------------------------------------------------------- here
            

            // will save to flash here

            save_to_flash();

            printf("saved to %d\n", macro_to_save.macro_num);
            printf("data count: %d\n", macro_to_save.data_count);

            // sci_send_IRMP_Macro(&macro_to_save);
            break;
        case 'U':
            memset(macro_to_save.signals, 0, sizeof(macro_to_save.signals));
            macro_to_save_index = 0;
            macro_to_save.macro_num = temp_buffer[1];
            macro_to_save.data_count = temp_buffer[2];
            printf("macro num: %d\n", macro_to_save.macro_num);
            break;
        case 'I':
            // send the command
            irmp_data_for_use.protocol = temp_buffer[1] ;
            irmp_data_for_use.address = (temp_buffer[2] << 8) | temp_buffer[3]; // [00010 0000] 00010000 this is now the int
            irmp_data_for_use.command = (temp_buffer[4] << 8) | temp_buffer[5];
            irmp_data_for_use.flags = 0;
            //store the command into the temp struct
            macro_to_save.signals[macro_to_save_index++] = irmp_data_for_use;
            printf("protocol: %d\n", irmp_data_for_use.protocol);
            break;
        default:
            break;
    }

}

//*************************************************************************
//  @Method: void load_from_flash(){
//  @Purpose: loads the macros from the flash
//  @Args:  -- none
//  @returns: void
//*************************************************************************
void load_from_flash(){
    // load the macros from the flash
    memcpy(&macros, Page31, sizeof(macros));
}

//*************************************************************************
//  @Method: void save_to_flash(){
//  @Purpose: saves the macros to the flash
//  @Args:  -- none
//  @returns: void
//*************************************************************************
void save_to_flash(){
    // save the macros to the flash
    RCC->APBENR2 |= RCC_AHBENR_FLASHEN;
    flash_unlock();

    // odd things happen when you try to write the struct to the flash and I get a hard fault
    // instead I will write the struct to a char array and write that to the flash
    // this is a work around
    char str[520] = "";

    memcpy(str, &macros, sizeof(macros));

    // erase the page
    flash_erase_page(31);

    // the work around
    flash_program((uint32_t *)Page31, (uint8_t*)str, sizeof(str));


    // this is the original code
    // flash_program((uint32_t *)Page31, (uint8_t*)macros, sizeof(macros));
    // flash_program((uint32_t *)Page31, (uint8_t*)&macros[0], sizeof(macros[0]));

    //lock the flash
    flash_lock();
}


//*************************************************************************
//  @Method: void send_Irmp_Macro(IRMP_MACRO *data){
//  @Purpose: sends the IRMP macro
//  @Args: IRMP_MACRO *data -- the data that will be used to store the IRMP data
//  @returns: void
//*************************************************************************
void send_Irmp_Macro(IRMP_MACRO *data){
    int location = data - macros;
    char st_buffer[20];

    ssd1306_Fill(Black);
    ssd1306_SetCursor(0,0);
    ssd1306_WriteString("Sending IR", Font_6x8, White);
    sprintf(st_buffer, "Macro: %d", location);
    ssd1306_SetCursor(0,10);
    ssd1306_WriteString(st_buffer, Font_6x8, White);

    ssd1306_UpdateScreen();


    for(int i = 0; i < data->data_count; i++){
        send_IRMP_data(&data->signals[i]);
        DelayMs(1000);
    }
    display_main_menu();
}

void sci_send_IRMP_Macro(IRMP_MACRO *data){

    // buffer to store the the IRMP structs
    uint8_t st_buffer[7];
    memset(st_buffer, 0, sizeof(st_buffer));
    st_buffer[0] = 'U';
    st_buffer[1] = data->macro_num;
    st_buffer[2] = data->data_count;
    st_buffer[3] = '\r';

    UART_Packet(ComPort,st_buffer, 4);
    memset(st_buffer, 0, sizeof(st_buffer));
    DelayMs(10);




    for(int i = 0; i < data->data_count; i++){
        st_buffer[0] = 'I';
        st_buffer[1] = data->signals[i].protocol;
        st_buffer[2] = (data->signals[i].address >> 8) & 0xFF;
        st_buffer[3] = data->signals[i].address & 0xFF;
        st_buffer[4] = (data->signals[i].command >> 8) & 0xFF;
        st_buffer[5] = data->signals[i].command & 0xFF;
        st_buffer[6] = '\r';
        UART_Packet(ComPort,st_buffer, 7);
        memset(st_buffer, 0, sizeof(st_buffer));
        DelayMs(10);
    }
    st_buffer[0] = 'W';
    st_buffer[1] = '\r';
    UART_Packet(ComPort,st_buffer, 2);
    memset(st_buffer, 0, sizeof(st_buffer));
}



//*************************************************************************
//  @Method: void checkButtons(){
//  @Purpose: checks the buttons
//  @Args:  -- none
//  @returns: void
//*************************************************************************
void checkButtons(){
    char hold = 0;
    (void)Sw_Process(&btn[0], Button_PORT, A0);
    (void)Sw_Process(&btn[1], Button_PORT, A1);
    (void)Sw_Process(&btn[2], Button_PORT, A2);
    (void)Sw_Process(&btn[3], Button_PORT, A3);
    (void)Sw_Process(&btn[4], Button_PORT, A4);
    (void)Sw_Process(&btn[5], Button_PORT, A5);
    (void)Sw_Process(&btn[6], Button_PORT, A6);
    (void)Sw_Process(&btn[7], Button_PORT, A7);

    // check all the states of the buttons
    for(int i = 0; i < 8; i++){
        switch (btn[i]) {
            case Pressed:
                send_Irmp_Macro(&macros[i]);
                break;
            case Held:
                hold = 1;
                //check for program flag
                if(program_flag){
                    program_flag = 0;
                    // need to make this function
                    program_remote(&macros[i], i);
                }
                break;
            // these dont do anything
            case Released:
                break;
            case Idle:
                break;
        }
    }
    // set the hold flag
    hold_flag = hold;
}

//*************************************************************************
//  @Method: void display_main_menu(){
//  @Purpose: display  main menue on the screen
//  @Args:  -- none
//  @returns: void
//*************************************************************************
void display_main_menu(){
    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("Main Menu", Font_11x18, White);
    ssd1306_SetCursor(0, 20);
    ssd1306_WriteString("press to send IR data", Font_6x8, White);
    ssd1306_UpdateScreen();
}


/*************************** Interrupts ****************************/


//*************************************************************************
//  @Method: void TimerCallback(){
//  @Purpose: Timer callback function
//  @Args:  -- none
//  @returns: void
//*************************************************************************
void TimerCallback(){
    if(hold_flag){
        timer_value++;
        // if the timer value is greater than 1000 then set the hold flag to 0
        if(timer_value >= hold_time){
            hold_flag = 0;
            timer_value = 0;
            program_flag = 1;
        }
    }
}

// the VCOM interrupt handler
void USART2_IRQHandler(void){
    // check if the interrupt is from the RXNE flag
    char data;
    if(UART_RxByte(VCOM, (uint8_t*)&data))
    {
        sci_buffer[sci_buffer_index++] = data;
        if(data == '\r'){
            sci_process_flag = 1;
        }
    }
    NVIC_ClearPendingIRQ(USART2_IRQn);
}



// the local com port interrupt handler
void USART1_IRQHandler(void){
    // check if the interrupt is from the RXNE flag
    char data;
    if(UART_RxByte(ComPort, (uint8_t*)&data))
    {
        // this is to make sure the buffer dosent overflow
        sci_buffer[sci_buffer_index++] = data;
        if(sci_buffer_index >= 20){
            sci_buffer_index = 0;
            memset(sci_buffer, 0, sizeof(sci_buffer));
        }
        if(data == '\r'){
            sci_process_flag = 1;
        }
        // send the data to the VCOM port
        // UART_TxChar(VCOM, data);
        printf("recived %d : %c  \r\n", data);
    }
    NVIC_ClearPendingIRQ(USART1_IRQn);
}


/*************************** End of file ****************************/
