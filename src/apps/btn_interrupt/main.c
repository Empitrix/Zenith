#include "timer.h"
#include "board.h"
#include "gpio.h"

#define DELAY_VALUE 1000

GPIN led1;
GPIN led2;
GPIN led3;
GPIN c13;
GPIN btn;

int main(void){
	boardInit();

	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);


	led1 = gpinInit(GPIN_2, GPIO_TYPE_A, GPIO_OUTPUT_MODE, GPIN_NO_PULL);
	led2 = gpinInit(GPIN_3, GPIO_TYPE_A, GPIO_OUTPUT_MODE, GPIN_NO_PULL);
	led3 = gpinInit(GPIN_4, GPIO_TYPE_A, GPIO_OUTPUT_MODE, GPIN_NO_PULL);
	c13 = gpinInit(GPIN_13, GPIO_TYPE_C, GPIO_OUTPUT_MODE, GPIN_NO_PULL);
	btn = gpinInit(GPIN_9, GPIO_TYPE_A, GPIO_MODE_IT_RISING, GPIN_PULL_DOWN);



	// BlUE-PILL c-13 blink
	while (1){
		gpinToggle(&c13);
		delayMs(200);
	}

}


// External Interrupt ISR Handler CallBackFun
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == GPIO_PIN_9){
		gpinToggle(&led1);
	}
}
