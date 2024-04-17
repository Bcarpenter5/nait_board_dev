//////////////////////////////////////////////////////////////////////////////////////
// Project: UASRT Lib
//       
//       
// Author: Brandon Carpenter 
//       
//       
// updates: 
//       created: 01/25/2024
//       
//       
//////////////////////////////////////////////////////////////////////////////////////
#ifndef UART_H
#define UART_H

#define VCOM (USART_TypeDef *)USART2

#include "stm32g031xx.h"


typedef enum {
    USART_Interupt_Enable = 0x00000001,
    USART_Interupt_Disable = 0x00000000,
} UART_Interupt;



//*************************************************************************
//  @Method: void Vcom_Init(uint32_t baudrate, UART_Interupt interupt);
//  @Purpose: initializes VCOM 
//  @Args: uint32_t baudrate -- baudrate to be used
//       @ UART_Interupt interupt -- enables or disables interupts
//  @returns: void
//*************************************************************************
void Vcom_Init(uint32_t baudrate, UART_Interupt interupt);


//*************************************************************************
//  @Method: void UART_Init(USART_TypeDef *USARTx, uint32_t baudrate, USART_Interupt interupt);
//  @Purpose: initializes UART
//  @Args: USART_TypeDef *USARTx -- selects which UART to use
//       @ uint32_t baudrate -- baudrate to be used
//       @ USART_Interupt interupt -- enables or disables interupts
//  @returns: void
//*************************************************************************
void UART_Init(USART_TypeDef *USARTx, uint32_t baudrate, UART_Interupt interupt);


//*************************************************************************
//  @Method: void UART_TxChar(USART_TypeDef *USARTx, uint8_t data);
//  @Purpose: sends a single byte over UART
//  @Args: USART_TypeDef *USARTx -- selects which UART to use
//       @ uint8_t data -- data to be sent
//  @returns: void 
//*************************************************************************
void UART_TxChar(USART_TypeDef *USARTx, uint8_t data);

//*************************************************************************
//  @Method: void UART_TxString(USART_TypeDef *USARTx, uint8_t *data);
//  @Purpose: sends a string over UART
//  @Args: USART_TypeDef *USARTx -- selects which UART to use
//       @ uint8_t *data -- pointer to string to be sent
//  @returns: void
//*************************************************************************
void UART_TxStr(USART_TypeDef *USARTx, char *data);


//*************************************************************************
//  @Method: void UART_Packet(USART_TypeDef *usart, uint8_t *data, uint8_t size);
//  @Purpose: sends a packet over UART
//  @Args: USART_TypeDef *usart -- selects which UART to use
//       @ uint8_t *data -- pointer to data to be sent
//       @ uint8_t size -- size of data to be sent
//  @returns: void
//*************************************************************************
void UART_Packet(USART_TypeDef *usart, uint8_t *data, uint8_t size);
//*************************************************************************
//  @Method: void UART_RxByte(USART_TypeDef *, uint8_t *);
//  @Purpose: recieves a single byte over UART
//  @Args: USART_TypeDef * -- selects which UART to use
//       @ uint8_t * -- pointer to where recieved data will be stored
//  @returns: char
//*************************************************************************
char UART_RxByte(USART_TypeDef *, uint8_t *data);



#endif // UART_H
