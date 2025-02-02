#include <stdint.h>

#include "main.h"

#ifndef __BOARD_GPIO__
#define __BOARD_GPIO__


#define MAX_IRQ_HANDLER 16

typedef enum {
	A_0,  A_1,  A_2,  A_3,  A_4,  A_5,  A_6, A_7,
	A_8, A_9, A_10, A_11, A_12, A_13, A_14, A_15,

	B_0,  B_1,  B_2,  B_3,  B_4,  B_5,  B_6, B_7,
	B_8, B_9, B_10, B_11, B_12, B_13, B_14, B_15,

	C_0,  C_1,  C_2,  C_3,  C_4,  C_5,  C_6, C_7,
	C_8, C_9, C_10, C_11, C_12, C_13, C_14, C_15,
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
	// PIN_OFF = GPIO_PIN_SET,
	// PIN_ON  = GPIO_PIN_RESET,

	PIN_ON = GPIO_PIN_SET,
	PIN_OFF  = GPIO_PIN_RESET,
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
} gpio_t;

gpio_t gpinInit(GPIO_PINS pin, GPIO_MODES mode, PullConfig pull_config);
void gpinSet(gpio_t * const pin, GPIO_STATE state);
void gpinToggle(gpio_t * const pin);
GPIO_STATE gpinRead(gpio_t * const pin);
GPIO_Lock_Status gpinLock(gpio_t * const pin);

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


typedef void (*callbackFunction_t)(void*);
// typedef void (*irqFunction_t)(void *);

// void gpinSetInterrupt(gpio_t *pin, irqModes_t irqMode, irqPriorities_t irqPriority, callbackFunction_t irqHandler);
void gpinSetInterrupt(gpio_t *pin, irqModes_t irqMode, irqPriorities_t irqPriority, callbackFunction_t irqHandler, void *context);
void gpinRemoveInterrupt(gpio_t *pin);


typedef struct {
	callbackFunction_t handler;
	void *context;
} gpioContext_t;

#endif
