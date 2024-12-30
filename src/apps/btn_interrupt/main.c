#include "timer.h"
#include "board.h"
#include "gpio.h"

GPIN led1;
GPIN c13;
GPIN btn;


void callback(){
	gpinToggle(&led1);
}

int main(void){
	boardInit();

	led1 = gpinInit(GPIN_A2,  GPIO_OUTPUT_MODE, GPIN_NO_PULL);
	c13 = gpinInit(GPIN_C13, GPIO_OUTPUT_MODE, GPIN_NO_PULL);

	btn = gpinInit (GPIN_A9,  GPIO_INPUT_MODE, GPIN_PULL_DOWN);
	gpinSetInterrupt(&btn, IRQ_RISING, IRQ_VERY_HIGH_PRIORITY, &callback);

	// BlUE-PILL c-13 blink
	while (1){
		gpinToggle(&c13);
		delayMs(200);
	}
}

