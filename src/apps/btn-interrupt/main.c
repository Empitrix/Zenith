#include "timer.h"
#include "board.h"
#include "gpio.h"

gpio_t led1;
gpio_t c13;
gpio_t btn;


void callback(void *ctx){
	gpinToggle(&led1);
}

int main(void){
	boardInit();

	led1 = gpinInit(A_2,  GPIO_OUTPUT_MODE, GPIN_NO_PULL);
	c13 = gpinInit(C_13, GPIO_OUTPUT_MODE, GPIN_NO_PULL);

	btn = gpinInit (A_9,  GPIO_INPUT_MODE, GPIN_PULL_DOWN);
	gpinSetInterrupt(&btn, IRQ_RISING, IRQ_VERY_HIGH_PRIORITY, &callback, NULL);

	// BlUE-PILL c-13 blink
	while (1){
		gpinToggle(&c13);
		delayMs(200);
	}
}

