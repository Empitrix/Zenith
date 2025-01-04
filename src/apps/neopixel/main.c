#include "board.h"
#include "neopixel.h"
#include "timer.h"



int main(){
	boardInit();

	timer_t t = timerInit(TIMER_4, 1, NULL, 1);

	while(1){
		NeopixelInit();
		delayMs(1000);
	}
	return 0;
}
