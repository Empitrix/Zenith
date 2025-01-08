#include <stdint.h>

#include "main.h"

#ifndef __BOARD_BLUE_PILL__
#define __BOARD_BLUE_PILL__

void boardInit(void);
int boardGetTick(void);
void delayMs(uint32_t milliseconds);

#endif
