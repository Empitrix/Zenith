#include "gpio.h"


GPIN gpinInit(GPIO_PINS pin, GPIO_TYPE type, GPIO_MODES mode, PullConfig pull_config){
	GPIN me;
	// Update me*
	me.pin = pin;
	me.type = type;
	me.mode = mode;
	me.pull_config = pull_config;

	// Initialize
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = pin;
	GPIO_InitStruct.Mode = mode;
	GPIO_InitStruct.Pull = pull_config;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init((GPIO_TypeDef *)type, &GPIO_InitStruct); 

	return me;
}


void gpinSet(GPIN * const me, GPIO_STATE state){
	HAL_GPIO_WritePin((GPIO_TypeDef *)me->type, me->pin, (GPIO_PinState)state);
}


void gpinToggle(GPIN * const me){
	HAL_GPIO_TogglePin((GPIO_TypeDef *)me->type, me->pin);
}


GPIO_STATE gpinRead(GPIN * const me){
	return (GPIO_STATE)HAL_GPIO_ReadPin((GPIO_TypeDef *)me->type, me->pin);
}


GPIO_Lock_Status gpinLock(GPIN * const me){
	return (GPIO_Lock_Status)HAL_GPIO_LockPin((GPIO_TypeDef *)me->type, me->pin);
}



void EXTI9_5_IRQHandler(void){
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
}


/*

GPIO EXTI interrupt init

HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

*/
