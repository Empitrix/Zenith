#include "board.h"

#define DELAY_VALUE 1000


void my_delay(int value){
	int wait = boardGetTick();
	wait = wait + value;
	while(boardGetTick() < wait){
		__asm__("nop");
	}
}

int main(void){
	boardInit();

	GPIN led;
	gpinInit(&led, GPIN_13, GPIO_TYPE_C, GPIO_OUTPUT_MODE, GPIN_NO_PULL);

	while (1){
		gpinToggle(&led);
		my_delay(DELAY_VALUE);
	}
}

