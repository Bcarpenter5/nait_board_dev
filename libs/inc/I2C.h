
#include "stm32g031xx.h"

typedef enum send_Stop {
    I2C_CONTINUE = 0,
    I2C_STOP = 1
} i2c_Stop;


typedef enum {
    I2C_READ = 1,
    I2C_WRITE = 0
} i2c_RW;


//*************************************************************************
//  @Method: void I2C_Init(void);
//  @Purpose: Initialize I2C module
//*************************************************************************
void I2C_init(void);


//*************************************************************************
//  @Method: void I2C_MemWrite(I2C_TypeDef *I2Cx, uint8_t adress, uint8_t memadress, uint8_t data, i2c_Stop stop){
//  @Purpose: send data to a specific memory adress of a slave
//  @Args: I2C_TypeDef *I2Cx -- I2C1 or I2C2
//       @ uint8_t adress -- adress of the slave
//       @ uint8_t memadress -- the memory adress of the slave
//       @ uint8_t data -- data to send
//       @ i2c_Stop stop -- I2C_CONTINUE or I2C_STOP
//  @returns: void
//*************************************************************************
void I2C_CommandWrite(I2C_TypeDef *I2Cx, uint8_t adress, uint8_t memadress, uint8_t data);

//*************************************************************************
//  @Method: void I2C_MemWrite(I2C_TypeDef *I2Cx, uint8_t adress, uint8_t memadress, uint8_t *data, uint8_t length){
//  @Purpose: send data to a specific memory adress of a slave
//  @Args: I2C_TypeDef *I2Cx -- I2C1 or I2C2
//       @ uint8_t adress -- adress of the slave
//       @ uint8_t memadress -- the memory adress of the slave
//       @ uint8_t *data -- data to send
//       @ uint8_t length -- length of the data
//  @returns: void
//*************************************************************************
void I2C_MemWrite(I2C_TypeDef *I2Cx, uint8_t adress, uint8_t memadress, uint8_t *data, uint8_t length);

//*************************************************************************
//  @Method: void I2C_address(I2C_TypeDef *I2Cx, uint8_t adress, i2c_RW rw){
//  @Purpose: set the adress of the slavej
//  @Args: I2C_TypeDef *I2Cx -- I2C1 or I2C2
//       @ uint8_t adress -- adress of the slave
//       @ i2c_RW rw -- I2C_READ or I2C_WRITE
//  @returns: void
//*************************************************************************
void I2C_address(I2C_TypeDef *I2Cx, uint8_t adress, i2c_RW rw);

//*************************************************************************
//  @Method: void I2C_write(I2C_TypeDef *I2Cx, uint8_t adress, uint8_t data, i2c_Stop stop);
//  @Purpose: write data to I2C
//  @Args: I2C_TypeDef *I2Cx -- I2C1 or I2C2
//       @ uint8_t adress -- adress of the slave
//       @ uint8_t data -- data to send
//       @ i2c_Stop stop -- I2C_CONTINUE or I2C_STOP
//  @returns: void
//*************************************************************************
void I2C_write(I2C_TypeDef *I2Cx, uint8_t adress, uint8_t data, i2c_Stop stop);

//*************************************************************************
//  @Method: void I2C_AutoWrite(I2C_TypeDef *I2Cx, uint8_t adress,uint8_t *data, uint8_t length);
//  @Purpose: 
//  @Args: I2C_TypeDef *I2Cx -- 
//       @ uint8_t adress -- 
//       @uint8_t *data -- 
//       @ uint8_t length -- 
//  @returns: void
//*************************************************************************
void I2C_AutoWrite(I2C_TypeDef *I2Cx, uint8_t adress,uint8_t *data, uint8_t length);

