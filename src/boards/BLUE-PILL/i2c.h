#include "main.h"
#include "gpio.h"
#include "stm32f1xx_hal_i2c.h"


#ifndef __BLUE_PILL_I2C__
#define __BLUE_PILL_I2C__



typedef enum {
	I2C_1,
	I2C_2,
} i2c_instance;

typedef enum {
	//           CLK           SDA
	I2C1_B6_B7 = (I2C_1 << 16) | (B_6  << 8) | B_7,
	// I2C1_B8_B9 = (I2C_1 << 16) | (B_8  << 8) | B_9,
	I2C2_B6_B7 = (I2C_1 << 16) | (B_10 << 8) | B_11,
} i2cPins_t;


typedef struct {
	i2c_instance instance;
	I2C_HandleTypeDef *hi2c;
} i2c_t;

i2c_t i2cInit(i2cPins_t pins, int clkSpeed);
void i2cMemWrite(i2c_t *i2c, int devAddr, int memAddr, int memAddrSize, uint8_t *data, int size);

#endif
