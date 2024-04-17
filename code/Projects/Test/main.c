//////////////////////////////////////////////////////////////////////////////////////
// Project: SCI_LIGHT_FIRMWARE
//       
//       
// Author: Brandon Carpenter 
//       
//       
//       
// updates: 
//       created: 03/17/2024
//       
//Instructions:
//
//       Coding Exercise
//       --------------------------------------------------------
//        For this exercise the goal is not to generate compiled and fully functional code but rather to demonstrate coding structure,
//        style, and design process.  Thus, it is not expected that the code be entirely bug free.
//        The code should be written in C using what you consider to be good coding practices for code reuse and maintainability.
//        Be prepared to explain your methodology.  You do not need to complete all the code,
//        but if time constrained can instead provide an explanation of what the code needs to do,
//        best practices for implementation, and any potential pitfalls to avoid.
//
//        Design Requirements:
//        --------------------------------------------------------
//        Design firmware that would be used to control two dimmable DC light fixtures 
//        and controlled by commands over a serial UART interface.  
//        The serial commands indicate which light is being controlled and the target brightness level of the 
//        light in 10% increments.  When a command is received, the light should be ramped up or down to the 
//        target brightness at a rate of 100 milliseconds for every 10% change.  
//        The lights should be controlled with a PWM frequency of 100 Hz.
//        It can be assumed that the brightness varies linearly with duty cycle.
//        Once the light reaches the target brightness, 
//        it should send a light status message over the UART interface indicating the new brightness and which light is reporting.
//
//        Assumptions:
//        This exercise is not specific to any one microcontroller, so low level or peripheral initialization can be shown
//        as function calls with whatever parameters might be required.  For instance, 
//
//        you can show the UART initialization with a function call:
//        UartInit( <add initialization parameters> )
//
//        You can assume that there are three interrupt service routines available for use by the code:
//          SystemTickIsr() – called once per millisecond
//          UartRxIsr() – called when one byte has been received by the UART interface
//          UartTxIsr() – called when there is room to write one byte to the UART interface
//
//        You can assume that there are functions to enable and disable the interrupts, for instance:
//          InterruptEnable( UART_TX_INTERRUPT )
//
//        You can assume there are functions to read and write a byte to the UART interface:
//          UartReadByte()
//          UartWriteByte( <Data> )
//
//        You can use the following macros to control the GPIO:
//          GPIO_SET( OUTPUTx )
//          GPIO_CLR( OUTPUTx )
//
//        For this exercise, assume that hardware based PWM is not available.
//
//        You can use the following packet structure for the command and status messages:
//        --------------------------------------------------------
//        Byte 0: frame sync, always 0xA5
//        Byte 1: packet type, 0xF0 for command, 0xF1 for status
//        Byte 2: Light Instance number being controlled or reporting status
//        Byte 3: Light brightness (0-100%)
//        Byte 4: Checksum, simple twos complement checksum on bytes 0-3, modulus 256
//
//       
//////////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include "memory.h"
#include files...


#define PWM_Incremnt  10
#define FRAME_SYNC 0xA5

/////////////////////////////////////////// GLOBALS ///////////////////////////////////////////

// max char in buffer
const char BUFF_MAX = 5;
//buffer to receive the SCI bytes
char buffer[BUFF_MAX] = "";

//Increment for the SCI to keep track of end
//this is needed because there is no end byte being sent in the instructions
char buff_length = 0;

// this is when the buffer is full
char buff_handle_flag = 0; 




//counts 1ms counter increments
char isr_counter = 0;
const char ISR_CNT_MAX = 100;

// I would like to store the recived data from sci into a struct
// 
typedef struct _sci_data {
    char sync; // frame sync, always 0xA5
    char packet_type;//packet type, 0xF0 for command, 0xF1 for status
    char light_num_reporting; // Light Instance number being controlled or reporting status         
    char light_brightness_set; //Light brightness (0-100%)             
    char checksum;// Checksum, simple twos complement checksum on bytes 0-3, modulus 256
} sci_data;

// struct that holds received data
sci_data recivedSCI;



// Fake PWM Values
const char PWM_PERIOD = 10; // 10ms
char pwm_cycle = 0;// the current position in the pwm cycle ie 0-9 || 10-100%

// this is flaged when the duty needs to change
char pwm_isr_flag = 0;

// duty is in % incase there is a future change from 10% increments
char lightOne_Duty = 50; // current duty in %
char lightOne_desired_Duty =50; // SCI specified duty too become

char lightTwo_Duty = 50;// current duty
char lightTwo_desired_Duty =50;// sci specified duty in %

//PWM Flags change flags
char lightOne_flag = 0;
char lightTwo_flag = 0;

//signal change done flags
char lightOne_done = 0;
char lightTwo_done = 0;


void changeDuty(char desired_duty, char* current_duty, char increment, char pwm);


/////////////////////////////////////////// MAIN ///////////////////////////////////////////
int main()
{
    //set clock to  desired speed
    ClockSet(20Mhz);

    // Initialize the UART
    UartInit(uart1,115200,Interupt_Endabled);

    // Initialize the GPIO
    GpioInit(1);
    GpioInit(2);

    // set a 1ms interrupt
    SetTick1ms();

    // Enable the UART RX interrupt
    InterruptEnable( UART_RX_INTERRUPT );

    // Enable the system tick interrupt
    InterruptEnable( SYSTEM_TICK_INTERRUPT );

    // Main loop
    while(1)
    {
        // Handle PWM Duty Change
        if(pwm_isr_flag){
            pwm_isr_flag = 0; // clear flag

            if(lightOne_flag)
                // this changes current duty by PWM_Increment
                // then changes the current duty too it
                // if both are == because of change  in function, trigger flag
                changeDuty(lightOne_desired_Duty,&lightOne_Duty , PWM_Incremnt,1);

            if(lightTwo_flag)
                changeDuty(lightTwo_desired_Duty,&lightTwo_Duty , PWM_Incremnt,2);
        }

        // process the buffer
        if(buff_handle_flag){
            buff_handle_flag =0;
            // copy too struct
            memcpy(buffer, &recivedSCI, buff_length*sizeof(buffer[0]));
            //clear buffer
            memset(buffer, 0, buff_length*sizeof(buffer[0]));
            //reset buff length
            buff_length = 0;

            // check for checksum
            char checksum = (~(recivedSCI.sync + recivedSCI.packet_type+recivedSCI.light_num_reporting)+1);// same as bottom VVVV

            //check that both checksum is write and it is getting a Command
            if(checksum == recivedSCI.checksum && recivedSCI.packet_type == 0xF0){
                // set the duty and flags
                switch (recivedSCI.light_num_reporting) {
                    case 1:
                        lightOne_Duty = recivedSCI.light_brightness_set;
                        lightOne_flag =1;
                        break;
                    case 2:
                        lightTwo_Duty = recivedSCI.light_brightness_set;
                        lightTwo_flag = 1;
                        break;
                    default:
                        break;
                }
            }
        }
        // check for done flags
        if(lightOne_done){
            lightOne_done = 0; 

        }
        if(lightTwo_Duty){
            lightTwo_done = 0; 
        }

    }
}
//*************************************************************************
//  @Method: char changeDuty(int pwm ,char desired_duty, char current_duty){
//  @Purpose: change the duty by 10% and set done flag if the change makes it equal
//  @Note: if both are == coming in return
//  @Args:  
//       @char desired_duty --  duty desired
//       @ char current_duty -- current duty
//       @ char increment -- amount to increment by
//       @ char pwm -- the fake pwm in use
//  @returns: void
//*************************************************************************
void changeDuty(char desired_duty, char* current_duty, char increment, char pwm){

    // check that duty is different
    if(*current_duty == desired_duty)
        return;

    // change duty by increment
    if(*current_duty > desired_duty)
        *current_duty -= increment;
    else
        *current_duty += increment;

    //check if current duty is now == 
    if(!(*current_duty = desired_duty))
        return;

    // set send msg flag two true
    // clear Change flag
    if(pwm == 1){
        lightOne_done = 1;
        lightOne_flag = 0;
    }
    else {
        lightTwo_done = 1;
        lightTwo_flag = 0;
    }
}


//*************************************************************************
//  @Method: void send_Done(char light_num, char duty){
//  @Purpose: send a UART response Showing the state
//  @Args: char light_num -- light number
//       @ char duty -- duty currently active
//  @returns: void
//*************************************************************************
void send_Done(char light_num, char duty){
    /* char sync; // frame sync, always 0xA5
       char packet_type;//packet type, 0xF0 for command, 0xF1 for status
       char light_num_reporting; // Light Instance number being controlled or reporting status         
       char light_brightness_set; //Light brightness (0-100%)             
       char checksum;// checksum, simple twos complement checksum on bytes 0-3, modulus 256 */


    //calc checksum
    char checksum = (~(FRAME_SYNC + 0xF1 + light_num)+1);// same as bottom VVVV
    //int checksum = (~(FRAME_SYNC + 0xF1 + light_num)+1) & 0xFF;// same as % 256


    UartWriteByte(FRAME_SYNC);// always  0xA5
    UartWriteByte(0xF1);// type = status
    UartWriteByte(light_num); // light nummber
    UartWriteByte(duty);
    UartWriteByte(checksum);
}



void SystemTickIsr(){
    // clear isr
    systyck_clear();

    // Period of 100hz is 10ms
    // duty change occurs every 100ms 
    //increment ISR_counter 
    isr_counter == isr_counter >= ISR_CNT_MAX ? 0 : isr_counter++;

    //increment pwn cycle
    pwm_cycle == pwm_cycle >= PWM_PERIOD ? 0 : pwm_cycle++;

    // check to change pwm when counter is 0
    if(!isr_counter){ 
        //set the pwn isr routine flag
        //run this when the main loop resumes
        pwm_isr_flag = 1;
    }

    /// this is operating similar to an output compare timer 

    // PWM Handling
    if(!pwm_cycle){
        GPIO_2_BYTE  ^= 1;
        GPIO_1_BYTE ^= 1; // flip the bit
    }

    // PWM 1
    // the /10 is  be casue pwm cycle is out of 10 and duty is a percentage
    if(lightOne_Duty /10 == pwm_cycle)
        // I could do this a realy dumb way by keeping track of some states, 
        // but this is the fastest and goes right to the hardware
        GPIO_1_BYTE ^= 1; // flip the bit


    // same but with light two
    if(lightTwo_Duty /10 == pwm_cycle)
        GPIO_2_BYTE  ^= 1;

    // there will be a glitch where when it will not stay on forevery unless this code is here
    // if it is zero it will flip and flip again on zero
    if(lightOne_Duty == 100)
        GPIO_1_BYTE |= 1; // flip the bit

    if(lightTwo_Duty == 100)
        GPIO_2_BYTE |= 1;
}


void UartRxIsr(){
    //clear ISR
    clearUARTIsr();

    // set the buffer latest too latest byte
    buffer[buff_length++] = UartReadByte();

    // when the buffer is full flag it down
    if(buff_length >= BUFF_MAX){
        buff_handle_flag =1;
    }
}



        




