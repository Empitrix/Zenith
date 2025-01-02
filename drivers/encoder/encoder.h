#include "gpio.h"

#ifndef __ENCODER_DRIVER__
#define __ENCODER_DRIVER__


typedef void (*encoderCallback_t)(int);

typedef struct {
	GPIO_PINS clk_pin;  // Clock Pin
	GPIO_PINS dt_pin;   // Data Pin
	GPIO_PINS btn_pin;  // Button Pin

	uint32_t counter;   // Counter

	encoderCallback_t increment;
	encoderCallback_t decrement;
	gpio_t dt;

} Encoder_t;



/* encoderInit: Encoder Initialize */
void encoderInit(Encoder_t *encoder, GPIO_PINS clk_pin,
	GPIO_PINS dt_pin, GPIO_PINS btn_pin,
	callbackFunction_t btn_callback,
	encoderCallback_t increment_cb,
	encoderCallback_t decrement_cb
);



uint32_t encoderGetCounter(Encoder_t *encoder);
void encoderResetCounter(Encoder_t *encoder);

#endif

