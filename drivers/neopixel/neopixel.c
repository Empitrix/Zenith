#include "neopixel.h"
#include "timer.h"
#include "board.h"
#include <stdint.h>


/* NeopixelInit: Initialize neopixel with number of total LED's and PWD pin (TIMER & TIMER CHANNEL) that running */
neopixel_t NeopixelInit(int numLeds, timerNumber_t timerNumber, timerChannel_t timerChannel){
	neopixel_t npxl = { 0 };
	npxl.numLeds = numLeds;
	npxl.size = numLeds * NEOPIXEL_ROW_SIZE;
	for(int i = 0; i < NEOPIXEL_MAX_SIZE; i++){ npxl.data[i] = 0; }
	npxl.timerNumber = timerNumber;
	npxl.timerChannel = timerChannel;
	return npxl;
}


/* NeopixelShow: Flush All LEDS */
void NeopixelShow(neopixel_t *npxl){
	timerStartDMA(npxl->timerNumber, npxl->timerChannel, npxl->data, npxl->size * sizeof(uint16_t));
	delayMs(10);
	timerStopDMA(npxl->timerNumber, npxl->timerChannel);
}


/* NeopixelClearAll: Turn on all LED's (Set All)*/
void NeopixelSetAll(neopixel_t *npxl){
	for(int i = 0; i < npxl->size; i++){ npxl->data[i] = 2; }
}


/* NeopixelClearAll: Turn off all LED's (Clear All)*/
void NeopixelClearAll(neopixel_t *npxl){
	for(int i = 0; i < npxl->size; i++){ npxl->data[i] = 1; }
}


/* NeopixelSetColor: Set specified LED with given r,g,b */
void NeopixelSetColor(neopixel_t *npxl, int idx, uint8_t r, uint8_t g, uint8_t b){
	int index = idx * NEOPIXEL_ROW_SIZE;
	for (int i = 7; i >= 0; i--) {
		npxl->data[index++] = ((g >> i) & 1) + 1;
	}
	for (int i = 7; i >= 0; i--) {
		npxl->data[index++] = ((r >> i) & 1) + 1;
	}
	for (int i = 7; i >= 0; i--) {
		npxl->data[index++] = ((b >> i) & 1) + 1;
	}
}


/* NeopixelSetAllColor: Set All  LEDs with specified r,g,b */
void NeopixelSetAllColor(neopixel_t *npxl, uint8_t r, uint8_t g, uint8_t b){
	for(int i = 0; i < npxl->numLeds; i++){
		NeopixelSetColor(npxl, i, r, g, b);
	}
}
