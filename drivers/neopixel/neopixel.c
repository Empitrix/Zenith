#include "neopixel.h"
#include "stm32f1xx_hal_tim.h"
#include "timer.h"
#include "board.h"
#include <stdint.h>


/* NeopixelInit: Initialize neopixel with number of total LED's and PWD pin (TIMER & TIMER CHANNEL) that running */
neopixel_t neopixelInit(int numLeds, GPIO_PINS dataIn){
	neopixel_t npxl = { 0 };
	npxl.numLeds = numLeds;
	npxl.size = numLeds * NEOPIXEL_ROW_SIZE;
	for(int i = 0; i < NEOPIXEL_MAX_SIZE; i++){ npxl.data[i] = 0; }
	npxl.dataIn = gpinInit(dataIn, GPIO_OUTPUT_MODE, GPIN_NO_PULL);
	return npxl;
}


/* NeopixelShow: Flush All LEDS */
void neopixelShow(neopixel_t *npxl){
	for(int i = 0; i < npxl->size; i++){
		if(npxl->data[i]){  // ON
			// ON 800ns
			((GPIO_TypeDef *)npxl->dataIn.type)->BSRR = npxl->dataIn.pin;
			for(int i = 0; i < 48; i++) __asm__("nop");
			// OFF 450ns
			((GPIO_TypeDef *)npxl->dataIn.type)->BSRR = (uint32_t)npxl->dataIn.pin << 16u;
			for(int i = 0; i < 28; i++) __asm__("nop");
		} else {  // OFF
			// ON 400ns
			((GPIO_TypeDef *)npxl->dataIn.type)->BSRR = npxl->dataIn.pin;
			for(int i = 0; i < 8; i++) __asm__("nop");
			// OFF 850ns
			((GPIO_TypeDef *)npxl->dataIn.type)->BSRR = (uint32_t)npxl->dataIn.pin << 16u;
			for(int i = 0; i < 52; i++) __asm__("nop");
		}
	}


	// timerStartDMA(TIMER_4, CH_1, npxl->data, 384 * 2);
	// delayMs(10);
	// timerStopDMA(TIMER_4, CH_1);
	// delayMs(10);
}


/* NeopixelClearAll: Turn on all LED's (Set All)*/
void neopixelSetAll(neopixel_t *npxl){
	for(int i = 0; i < npxl->size; i++){ npxl->data[i] = 1; }
}


/* NeopixelClearAll: Turn off all LED's (Clear All)*/
void neopixelClearAll(neopixel_t *npxl){
	for(int i = 0; i < npxl->size; i++){ npxl->data[i] = 0; }
}


/* NeopixelSetColor: Set specified LED with given r,g,b */
void neopixelSetColor(neopixel_t *npxl, int idx, uint8_t r, uint8_t g, uint8_t b){
	int index = idx * NEOPIXEL_ROW_SIZE;
	for (int i = 7; i >= 0; i--) {
		npxl->data[index++] = ((g >> i) & 1);
	}
	for (int i = 7; i >= 0; i--) {
		npxl->data[index++] = ((r >> i) & 1);
	}
	for (int i = 7; i >= 0; i--) {
		npxl->data[index++] = ((b >> i) & 1);
	}
}


/* NeopixelSetAllColor: Set All  LEDs with specified r,g,b */
void neopixelSetAllColor(neopixel_t *npxl, uint8_t r, uint8_t g, uint8_t b){
	for(int i = 0; i < npxl->numLeds; i++){
		neopixelSetColor(npxl, i, r, g, b);
	}
}
