#include "board.h"
#include "neopixel.h"
#include "timer.h"



int main(){
	boardInit();
	neopixel_t npxl = neopixelInit(1, B_6);

	int delay = 30;
	while(1){
		// Set all LEDs (white)
		neopixelSetAll(&npxl);
		// neopixelClearAll(&npxl);
		neopixelShow(&npxl);
		delayMs(delay);

		// Claer all LEDs
		neopixelClearAll(&npxl);
		neopixelShow(&npxl);
		delayMs(delay);
		// delayMs(1000);

		// neopixelSetColor(&npxl, 0, 0, 255, 0);
		// neopixelShow(&npxl);
		// delayMs(1000);

		// // Set specified LED to RED
		// neopixelSetColor(&npxl, 0, 255, 0, 0);
		// neopixelShow(&npxl);
		// delayMs(500);

		// // Set specified LED to GREEN
		// neopixelSetColor(&npxl, 0, 0, 255, 0);
		// neopixelShow(&npxl);
		// delayMs(500);

		// // Set specified LED to BLUE
		// neopixelSetColor(&npxl, 0, 0, 0, 255);
		// neopixelShow(&npxl);
		// delayMs(500);

		// // Set All LEDs to RED
		// neopixelSetAllColor(&npxl, 255, 0, 0);
		// neopixelShow(&npxl);
		// delayMs(500);

		// // Set All LEDs to GREEN
		// neopixelSetAllColor(&npxl, 0, 255, 0);
		// neopixelShow(&npxl);
		// delayMs(500);

		// // Set All LEDs to BLUE
		// neopixelSetAllColor(&npxl, 0, 0, 255);
		// neopixelShow(&npxl);
		// delayMs(500);

		// // Claer all LEDs
		// neopixelClearAll(&npxl);
		// neopixelShow(&npxl);
		// delayMs(1000);
	}
	return 0;
}
