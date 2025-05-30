#include "i2c.h"
#include "stm32f1xx_hal_i2c.h"

#define GET_I2C_INSTANCE(pin) ((pin >> 16) & 0xFF)  // Extract I2C instance
#define GET_SCL(pin)          ((pin >> 8)  & 0xFF)  // Extract SCL pin
#define GET_SDA(pin)          (pin & 0xFF)          // Extract SDA pin


I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;


I2C_HandleTypeDef* MX_I2C1_Init(int clockSpeed){
	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 100000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if(HAL_I2C_Init(&hi2c1) != HAL_OK){
		Error_Handler();
	}
	return &hi2c1;
}


I2C_HandleTypeDef *MX_I2C2_Init(int clockSpeed){
	hi2c2.Instance = I2C2;
	hi2c2.Init.ClockSpeed = clockSpeed;
	hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c2.Init.OwnAddress1 = 0;
	hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c2.Init.OwnAddress2 = 0;
	hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if(HAL_I2C_Init(&hi2c2) != HAL_OK){
		Error_Handler();
	}
	return &hi2c2;
}


i2c_t i2cInit(i2cPins_t pin, int clkSpeed){
	i2c_t i2c;
	i2c_instance instance = GET_I2C_INSTANCE(pin);
	switch(instance){
		case I2C_1: i2c.hi2c = MX_I2C1_Init(clkSpeed); break;
		case I2C_2: i2c.hi2c = MX_I2C2_Init(clkSpeed); break;
		default: break;
	}

	i2c.instance = instance;

	return i2c;
}


void i2cMemWrite(i2c_t *i2c, int devAddr, int memAddr, int memAddrSize, uint8_t *data, int size){
	HAL_I2C_Mem_Write(i2c->hi2c, devAddr, memAddr, memAddrSize, data, size, HAL_MAX_DELAY);
}

