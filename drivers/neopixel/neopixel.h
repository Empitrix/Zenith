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
	gpio_t dataIn;
} neopixel_t;


/* NeopixelInit: Initialize neopixel with number of total LED's and PWD pin (TIMER & TIMER CHANNEL) that running */
neopixel_t neopixelInit(int numLeds, GPIO_PINS dataIn);

/* NeopixelClearAll: Turn off all LED's (Clear All)*/
void neopixelClearAll(neopixel_t *npxl);

/* NeopixelClearAll: Turn on all LED's (Set All)*/
void neopixelSetAll(neopixel_t *npxl);

/* NeopixelSetColor: Set specified LED with given r,g,b */
void neopixelSetColor(neopixel_t *npxl, int idx, uint8_t r, uint8_t g, uint8_t b);

/* NeopixelShow: Flush All LEDS */
void neopixelShow(neopixel_t *npxl);

/* NeopixelSetAllColor: Set All  LEDs with specified r,g,b */
void neopixelSetAllColor(neopixel_t *npxl, uint8_t r, uint8_t g, uint8_t b);

#endif

