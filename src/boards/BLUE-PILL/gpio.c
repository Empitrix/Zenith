#include "gpio.h"
#include "stm32f103xb.h"

int gpio_bases[] = { GPIOA_BASE, GPIOB_BASE, GPIOC_BASE };

callbackFunction_t __irq_handlers[MAX_IRQ_HANDLER] = { NULL };


gpio_t gpinInit(GPIO_PINS pin, GPIO_MODES mode, PullConfig pull_config){
	gpio_t gpin;
	
	int type = gpio_bases[(pin / 16)];

	gpin.number = (pin % 16);
	gpin.pin = (1 << gpin.number);
	gpin.type = (GPIO_TYPE)type;
	gpin.mode = mode;
	gpin.pull_config = pull_config;

	// Initialize
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = gpin.pin;
	GPIO_InitStruct.Mode = gpin.mode;
	GPIO_InitStruct.Pull = gpin.pull_config;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init((GPIO_TypeDef *)gpin.type, &GPIO_InitStruct); 

	return gpin;
}


void gpinSet(gpio_t * const pin, GPIO_STATE state){
	HAL_GPIO_WritePin((GPIO_TypeDef *)pin->type, pin->pin, (GPIO_PinState)state);
}


void gpinToggle(gpio_t * const pin){
	HAL_GPIO_TogglePin((GPIO_TypeDef *)pin->type, pin->pin);
}


GPIO_STATE gpinRead(gpio_t * const pin){
	return (GPIO_STATE)HAL_GPIO_ReadPin((GPIO_TypeDef *)pin->type, pin->pin);
}


GPIO_Lock_Status gpinLock(gpio_t * const me){
	return (GPIO_Lock_Status)HAL_GPIO_LockPin((GPIO_TypeDef *)me->type, me->pin);
}



void gpinSetInterrupt(gpio_t *pin, irqModes_t irqMode, irqPriorities_t irqPriority, callbackFunction_t irqHandler){

	int priority = 0;
	switch (irqPriority){
		case IRQ_VERY_LOW_PRIORITY: case IRQ_LOW_PRIORITY:
			priority = 3;
			break;
		case IRQ_MEDIUM_PRIORITY:
			priority = 2;
			break;
		case IRQ_HIGH_PRIORITY:
			priority = 1;
			break;
		case IRQ_VERY_HIGH_PRIORITY:
			priority = 0;
			break;
	}

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = pin->pin;
	GPIO_InitStruct.Mode = irqMode;
	GPIO_InitStruct.Pull = pin->pull_config;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init((GPIO_TypeDef *)pin->type, &GPIO_InitStruct); 


	IRQn_Type IRQnb = EXTI0_IRQn;

	switch (pin->number) {
		case 0: IRQnb = EXTI0_IRQn; break;
		case 1: IRQnb = EXTI1_IRQn; break;
		case 2: IRQnb = EXTI2_IRQn; break;
		case 3: IRQnb = EXTI3_IRQn; break;
		case 4: IRQnb = EXTI4_IRQn; break;
		case 5: case 6: case 7: case 8: case 9:
			IRQnb = EXTI9_5_IRQn;
			break;
		case 10: case 11: case 12: case 13: case 14: case 15:
			IRQnb = EXTI15_10_IRQn;
			break;
	}

	// save callback
	if(irqHandler != NULL){
		__irq_handlers[pin->number] = irqHandler;
	}

	HAL_NVIC_SetPriority(IRQnb, priority, 0);
	HAL_NVIC_EnableIRQ(IRQnb);
}


void gpinRemoveInterrupt(gpio_t *pin){
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = pin->pin;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	HAL_GPIO_Init((GPIO_TypeDef *)pin->type, &GPIO_InitStruct); 

	// Remove the handler
	__irq_handlers[pin->number] = NULL;
}



// External Interrupt ISR Handler CallBackFun
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	for(int i = 0; i < MAX_IRQ_HANDLER; i++){
		if(GPIO_Pin == (1 << i) && __irq_handlers[i] != NULL){
			__irq_handlers[i]();
		}
	}
}

