#include <stdint.h>

#include "main.h"
#include "gpio.h"

#ifndef __ANALOG_BLUE_PILL__
#define __ANALOG_BLUE_PILL__

#define MAX_ANALOG_CHANNELS 10

#define GET_ANALOG_INSTANCE(pin)   ((pin >> 8)  & 0xFF)
#define GET_ANALOG_PIN(pin)        (pin & 0xFF)


typedef enum {
	ADC_1,
	ADC_2,
} analogInstance_t;


typedef enum {
	// ADC 1
	ADC1_A1 = (ADC_1 << 8) | A_1,
	ADC1_A2 = (ADC_1 << 8) | A_2,
	ADC1_A3 = (ADC_1 << 8) | A_3,
	ADC1_A4 = (ADC_1 << 8) | A_4,
	ADC1_A5 = (ADC_1 << 8) | A_5,
	ADC1_A6 = (ADC_1 << 8) | A_6,
	ADC1_A7 = (ADC_1 << 8) | A_7,
	ADC1_B0 = (ADC_1 << 8) | B_0,
	ADC1_B1 = (ADC_1 << 8) | B_1,

	// ADC 2
	ADC2_A1 = (ADC_2 << 8) | A_1,
	ADC2_A2 = (ADC_2 << 8) | A_2,
	ADC2_A3 = (ADC_2 << 8) | A_3,
	ADC2_A4 = (ADC_2 << 8) | A_4,
	ADC2_A5 = (ADC_2 << 8) | A_5,
	ADC2_A6 = (ADC_2 << 8) | A_6,
	ADC2_A7 = (ADC_2 << 8) | A_7,
	ADC2_B0 = (ADC_2 << 8) | B_0,
	ADC2_B1 = (ADC_2 << 8) | B_1,
} analogPin_t;


typedef void (*analogCallback_t)(int);

typedef struct {
	analogPin_t pin;
	ADC_HandleTypeDef adc;
	uint32_t value;
} analog_t;


analog_t analogInit(analogPin_t pin);
void analogDMAInit(analog_t *analog, analogPin_t pin, analogCallback_t callback);
uint32_t analogRead(analog_t *analog);


#endif
