#include <stdint.h>

#ifndef __BOARD_BLUE_PILL__
#define __BOARD_BLUE_PILL__
// #include "board.c"


typedef enum {
	GPIN_0  = 0x0001,
	GPIN_1  = 0x0002,
	GPIN_2  = 0x0004,
	GPIN_3  = 0x0008,
	GPIN_4  = 0x0010,
	GPIN_5  = 0x0020,
	GPIN_6  = 0x0040,
	GPIN_7  = 0x0080,
	GPIN_8  = 0x0100,
	GPIN_9  = 0x0200,
	GPIN_10 = 0x0400,
	GPIN_11 = 0x0800,
	GPIN_12 = 0x1000,
	GPIN_13 = 0x2000,
	GPIN_14 = 0x4000,
	GPIN_15 = 0x8000,
} GPIO_PINS;

typedef enum {
	PIN_OFF = 0u,
	PIN_ON  = 1u,
} GPIO_STATE;



void BoardInit(void);
void DelayMs(uint32_t milliseconds);
void PinModeSet(GPIO_PINS pin, GPIO_STATE state);

#endif
