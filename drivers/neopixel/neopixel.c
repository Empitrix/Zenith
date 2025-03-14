#include "neopixel.h"
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


static inline void ws2812_send_bit(uint32_t pin, GPIO_TypeDef *port, uint8_t bit) {
	if (bit) {
		__asm__ volatile (
			"str %[set], [%[port], #0] \n\t"  // Set pin high
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			"nop \n\t nop \n\t"
			"str %[clr], [%[port], #0] \n\t"  // Set pin low
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			:
			: [port] "r" (&port->BSRR), [set] "r" (pin), [clr] "r" (pin << 16)
			: "memory"
		);
	} else {
		__asm__ volatile (
			"str %[set], [%[port], #0] \n\t"  // Set pin high
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			"nop \n\t nop \n\t nop \n\t"
			"str %[clr], [%[port], #0] \n\t"  // Set pin low
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			"nop \n\t nop \n\t nop \n\t nop \n\t"
			:
			: [port] "r" (&port->BSRR), [set] "r" (pin), [clr] "r" (pin << 16)
			: "memory"
		);
	}
}


/* NeopixelShow: Flush All LEDS */
void neopixelShow(neopixel_t *npxl){
	__disable_irq();
	for(int i = 0; i < npxl->size; i++){

		// if(npxl->data[i]){  // ON
		// 	// ON 800ns
		// 	((GPIO_TypeDef *)npxl->dataIn.type)->BSRR = npxl->dataIn.pin;
		// 	for(int i = 0; i < 48; i++) __asm__ volatile ("nop");
		// 	// OFF 450ns
		// 	((GPIO_TypeDef *)npxl->dataIn.type)->BSRR = (uint32_t)npxl->dataIn.pin << 16u;
		// 	for(int i = 0; i < 28; i++) __asm__ volatile ("nop");
		// } else {  // OFF
		// 	// ON 400ns
		// 	((GPIO_TypeDef *)npxl->dataIn.type)->BSRR = npxl->dataIn.pin;
		// 	for(int i = 0; i < 12; i++) __asm__ volatile ("nop");
		// 	// OFF 850ns
		// 	((GPIO_TypeDef *)npxl->dataIn.type)->BSRR = (uint32_t)npxl->dataIn.pin << 16u;
		// 	for(int i = 0; i < 52; i++) __asm__ volatile ("nop");
		// }

		ws2812_send_bit(GPIO_PIN_6, GPIOB, npxl->data[i]);

		// if(npxl->data[i]){  // ON
		// 	// ON 800ns
		// 	((GPIO_TypeDef *)npxl->dataIn.type)->BSRR = npxl->dataIn.pin;  // 42
		// 	for(volatile int i = 0; i < 3; i++) __asm__ volatile ("nop");
		// 	// OFF 450ns
		// 	((GPIO_TypeDef *)npxl->dataIn.type)->BSRR = (uint32_t)npxl->dataIn.pin << 16u;  // 292
		// 	for(volatile int i = 0; i < 1; i++) __asm__ volatile ("nop");

		// } else {  // OFF
		// 	// ON 400ns
		// 	((GPIO_TypeDef *)npxl->dataIn.type)->BSRR = npxl->dataIn.pin;
		// 	for(volatile int i = 0; i < 0; i++) __asm__ volatile ("nop");  // 333ns
		// 	// OFF 850ns
		// 	((GPIO_TypeDef *)npxl->dataIn.type)->BSRR = (uint32_t)npxl->dataIn.pin << 16u;
		// 	for(volatile int i = 0; i < 3; i++) __asm__ volatile ("nop");  // 917
		// }


		// if(npxl->data[i]){  // ON
		// 	// ON 800ns
		// 	((GPIO_TypeDef *)npxl->dataIn.type)->BSRR = npxl->dataIn.pin;  // 42
		// 	__asm__ volatile ("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n");  // 333
		// 	__asm__ volatile ("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n");  // 333
		// 	__asm__ volatile ("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n");  // 333
		// 	__asm__ volatile ("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n");  // 333
		// 	__asm__ volatile ("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n");  // 333

		// 	// for(volatile int i = 0; i < 3; i++) __asm__ volatile ("nop");
		// 	// OFF 450ns
		// 	((GPIO_TypeDef *)npxl->dataIn.type)->BSRR = (uint32_t)npxl->dataIn.pin << 16u;  // 292
		// 	__asm__ volatile ("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n");  // 333
		// 	// for(volatile int i = 0; i < 1; i++) __asm__ volatile ("nop");

		// } else {  // OFF
		// 	// ON 400ns
		// 	((GPIO_TypeDef *)npxl->dataIn.type)->BSRR = npxl->dataIn.pin;
		// 	// __asm__ volatile ("nop\nnop\nnop\nnop\nnop\nnop\n"); // 293
		// 	__asm__ volatile ("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n");  // 333

		// 	// >>> // for(volatile int i = 0; i < 0; i++) __asm__ volatile ("nop");  // 333ns

		// 	// for(volatile int i = 0; i < 1; i++) __asm__ volatile ("nop");  // 500
		// 	// for(volatile int i = 0; i < 2; i++) __asm__ volatile ("nop");  // 750

		// 	// OFF 850ns
		// 	((GPIO_TypeDef *)npxl->dataIn.type)->BSRR = (uint32_t)npxl->dataIn.pin << 16u;

		// 	__asm__ volatile ("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n");  // 333
		// 	__asm__ volatile ("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n");  // 333
		// 	__asm__ volatile ("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n");  // 333
		// 	__asm__ volatile ("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n");  // 333
		// 	// for(volatile int i = 0; i < 50; i++) __asm__ volatile ("nop");

		// 	// >>> // for(volatile int i = 0; i < 3; i++) __asm__ volatile ("nop");  // 917
		// }
	}

	
	__enable_irq();
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
	// swap
	int dummy = 0;
	dummy = r;
	r = g;
	g = dummy;

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
