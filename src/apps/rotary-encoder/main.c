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
	serialPrint("Counter: %d\n", counter);
	gpinToggle(&led3);
}

int count = 0;

int main(){
	boardInit();

	led = gpinInit(C_13, GPIO_OUTPUT_MODE, GPIN_NO_PULL);

	led2 = gpinInit(A_3, GPIO_OUTPUT_MODE, GPIN_NO_PULL);
	led3 = gpinInit(A_4, GPIO_OUTPUT_MODE, GPIN_NO_PULL);

	encoderInit(&enc, A_8, A_9, A_10, onPressCallback, onIncrement, onDecrement);

	while(1){
		serialPrint("Looping: %d\n", count++);
		delayMs(100);
	}
}
