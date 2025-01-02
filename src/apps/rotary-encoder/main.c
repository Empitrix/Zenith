#include "board.h"
#include "gpio.h"
#include "encoder.h"

gpio_t led, led2, led3;
encoder_t enc;

void onPressCallback(void *x){
	gpinToggle(&led);
}

void onIncrement(int counter){
	gpinToggle(&led2);
}

void onDecrement(int counter){
	gpinToggle(&led3);
}

int main(){
	boardInit();

	led = gpinInit(C_13, GPIO_OUTPUT_MODE, GPIN_NO_PULL);

	led2 = gpinInit(A_3, GPIO_OUTPUT_MODE, GPIN_NO_PULL);
	led3 = gpinInit(A_4, GPIO_OUTPUT_MODE, GPIN_NO_PULL);

	encoderInit(&enc, A_11, A_12, A_8, onPressCallback, onIncrement, onDecrement);

	while(1){
		delayMs(100);
	}
}
