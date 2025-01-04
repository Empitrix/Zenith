#include "board.h"
#include "neopixel.h"
#include "timer.h"



int main(){
	boardInit();
	neopixel_t npxl;

	timerInit(TIMER_4, 1, NULL, 1);
	npxl = NeopixelInit(25, TIMER_4, TIMER_CH_1);

	while(1){
		// Set all LEDs (white)
		NeopixelSetAll(&npxl);
		NeopixelShow(&npxl);
		delayMs(1000);

		// Claer all LEDs
		NeopixelClearAll(&npxl);
		NeopixelShow(&npxl);
		delayMs(1000);

		// Set specified LED to RED
		NeopixelSetColor(&npxl, 0, 255, 0, 0);
		NeopixelShow(&npxl);
		delayMs(500);

		// Set specified LED to GREEN
		NeopixelSetColor(&npxl, 0, 0, 255, 0);
		NeopixelShow(&npxl);
		delayMs(500);

		// Set specified LED to BLUE
		NeopixelSetColor(&npxl, 0, 0, 0, 255);
		NeopixelShow(&npxl);
		delayMs(500);

		// Set All LEDs to RED
		NeopixelSetAllColor(&npxl, 255, 0, 0);
		NeopixelShow(&npxl);
		delayMs(500);

		// Set All LEDs to GREEN
		NeopixelSetAllColor(&npxl, 0, 255, 0);
		NeopixelShow(&npxl);
		delayMs(500);

		// Set All LEDs to BLUE
		NeopixelSetAllColor(&npxl, 0, 0, 255);
		NeopixelShow(&npxl);
		delayMs(500);

		// Claer all LEDs
		NeopixelClearAll(&npxl);
		NeopixelShow(&npxl);
		delayMs(1000);
	}
	return 0;
}
