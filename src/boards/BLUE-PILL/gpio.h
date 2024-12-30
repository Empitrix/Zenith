#include <stdint.h>

#include "main.h"

#ifndef __BOARD_GPIO__
#define __BOARD_GPIO__


#define MAX_IRQ_HANDLER 16

typedef enum {
	GPIN_A0,  GPIN_A1,  GPIN_A2,  GPIN_A3,  GPIN_A4,  GPIN_A5,  GPIN_A6, GPIN_A7,
	GPIN_A8, GPIN_A9, GPIN_A10, GPIN_A11, GPIN_A12, GPIN_A13, GPIN_A14, GPIN_A15,

	GPIN_B0,  GPIN_B1,  GPIN_B2,  GPIN_B3,  GPIN_B4,  GPIN_B5,  GPIN_B6, GPIN_B7,
	GPIN_B8, GPIN_B9, GPIN_B10, GPIN_B11, GPIN_B12, GPIN_B13, GPIN_B14, GPIN_B15,

	GPIN_C0,  GPIN_C1,  GPIN_C2,  GPIN_C3,  GPIN_C4,  GPIN_C5,  GPIN_C6, GPIN_C7,
	GPIN_C8, GPIN_C9, GPIN_C10, GPIN_C11, GPIN_C12, GPIN_C13, GPIN_C14, GPIN_C15,
} GPIO_PINS;

typedef enum {
	GPIO_TYPE_A = GPIOA_BASE,
	GPIO_TYPE_B = GPIOB_BASE,
	GPIO_TYPE_C = GPIOC_BASE,
} GPIO_TYPE;

typedef enum {
	GPIO_INPUT_MODE = GPIO_MODE_INPUT,
	GPIO_OUTPUT_MODE = GPIO_MODE_OUTPUT_PP,
} GPIO_MODES;


typedef enum {
	PIN_OFF = 0u,
	PIN_ON  = 1u,
} GPIO_STATE;



typedef enum {
	GLS_OK = HAL_OK,
	GLS_ERROR = HAL_ERROR,
	GLS_BUSY = HAL_BUSY,
	GLS_TIMEOUT = HAL_TIMEOUT,
} GPIO_Lock_Status;


typedef enum {
	GPIN_PULL_DOWN = GPIO_PULLDOWN,
	GPIN_PULL_UP = GPIO_PULLUP,
	GPIN_NO_PULL = GPIO_NOPULL,
} PullConfig;


typedef struct {
	int number;
	int pin;
	GPIO_TYPE type;
	GPIO_MODES mode;
	PullConfig pull_config;
} GPIN;

GPIN gpinInit(GPIO_PINS pin, GPIO_MODES mode, PullConfig pull_config);
void gpinSet(GPIN * const me, GPIO_STATE state);
void gpinToggle(GPIN * const me);
GPIO_STATE gpinRead(GPIN * const me);
GPIO_Lock_Status gpinLock(GPIN * const me);

typedef enum {
	IRQ_RISING = GPIO_MODE_IT_RISING,
	IRQ_FALLING = GPIO_MODE_IT_FALLING,
	IRQ_RISING_FALLING = GPIO_MODE_IT_RISING_FALLING,
} irqModes_t;


typedef enum {
	IRQ_VERY_LOW_PRIORITY,
	IRQ_LOW_PRIORITY,
	IRQ_MEDIUM_PRIORITY,
	IRQ_HIGH_PRIORITY,
	IRQ_VERY_HIGH_PRIORITY,
} irqPriorities_t;


typedef void (*callbackFunction_t)(void);

void gpinSetInterrupt(GPIN *pin, irqModes_t irqMode, irqPriorities_t irqPriority, callbackFunction_t irqHandler);
void gpinRemoveInterrupt(GPIN *pin);

#endif
