#include "board.h"
#include "gpio.h"

#define DELAY_VALUE 1000

int main(void){
	boardInit();

	GPIN led = gpinInit(GPIN_C13, GPIO_OUTPUT_MODE, GPIN_NO_PULL);

	while (1){
		gpinToggle(&led);
		delayMs(DELAY_VALUE);
	}
}

