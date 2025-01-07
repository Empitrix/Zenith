#include <stdint.h>

#include "main.h"

#ifndef __BOARD_BLUE_PILL__
#define __BOARD_BLUE_PILL__

void boardInit(void);
int boardGetTick(void);
void serialPrint(const char * frmt, ...);


void delayMs(uint32_t milliseconds);


typedef enum {
	UART_SERIAL,
	USB_SERIAL,
	NO_SERIAL
} serial_t;

void serialBegin(serial_t type);

#endif
