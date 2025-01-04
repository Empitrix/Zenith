#include "gpio.h"
#include "timer.h"
#include <stdint.h>

#ifndef __NEOPIXEL_DRIVER__
#define __NEOPIXEL_DRIVER__

// LED buffer size
#define NEOPIXEL_MAX_SIZE 700

// Datasheet: https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf
// Number of LED bits, (8 G, 8 R, 8 B) (24 in total)
#define NEOPIXEL_ROW_SIZE 24

typedef struct {
	int size;                          // Total bits
	int numLeds;                       // Number of all LEDs
	uint32_t data[NEOPIXEL_MAX_SIZE];  // LED buffer

	timerNumber_t timerNumber;         // Timer number (running PWM)
	timerChannel_t timerChannel;       // Timer channel (running PWM)
} neopixel_t;


/* NeopixelInit: Initialize neopixel with number of total LED's and PWD pin (TIMER & TIMER CHANNEL) that running */
neopixel_t NeopixelInit(int numLeds, timerNumber_t timerNumber, timerChannel_t timerChannel);

/* NeopixelClearAll: Turn off all LED's (Clear All)*/
void NeopixelClearAll(neopixel_t *npxl);

/* NeopixelClearAll: Turn on all LED's (Set All)*/
void NeopixelSetAll(neopixel_t *npxl);

/* NeopixelSetColor: Set specified LED with given r,g,b */
void NeopixelSetColor(neopixel_t *npxl, int idx, uint8_t r, uint8_t g, uint8_t b);

/* NeopixelShow: Flush All LEDS */
void NeopixelShow(neopixel_t *npxl);

/* NeopixelSetAllColor: Set All  LEDs with specified r,g,b */
void NeopixelSetAllColor(neopixel_t *npxl, uint8_t r, uint8_t g, uint8_t b);

#endif

