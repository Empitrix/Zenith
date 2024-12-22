#include "board.h"


int main(void){
	BoardInit();

	while (1){
		PinModeSet(GPIN_13, PIN_ON);
		DelayMs(1000);
		PinModeSet(GPIN_13, PIN_OFF);
		DelayMs(1000);
	}
}

