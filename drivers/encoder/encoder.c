#include "encoder.h"
#include <stdbool.h>


/* gray2bin: Convert gray code to binary */
int gray2bin(GPIO_STATE sA, GPIO_STATE sB){
	if(sA == PIN_OFF && sB == PIN_OFF){
		return 0;
	} else if (sA == PIN_OFF && sB == PIN_ON){
		return 1;
	} else if (sA == PIN_ON && sB == PIN_ON){
		return 2;
	} else {
		return 3;
	}
	return 0;
}



/* onRotaryChange: This function is responsible for calling the callbacks at the appropriate time */
void onRotaryChange(encoder_t *enc){
	int s1 = gpinRead(&enc->clk);
	int s2 = gpinRead(&enc->dt);

	enc->pos = gray2bin(s1, s2);
	int value = enc->lastPos - enc->pos;

	int direction = 0;

	if(value == 1 || value == -3){
		if(enc->first){enc->value=2;}
		enc->value--;
		direction = 2;
	} else if(value == -1 || value == 3){
		if(enc->first){enc->value=-2;}
		enc->value++;
		direction = 1;
	}

	enc->lastPos = enc->pos;
	enc->first = 0;
	enc->counter = enc->value / 4;
	enc->charge++;

	if(direction == 1 && enc->charge == 3){
		enc->increment(enc->counter);
		enc->charge = -1;
	} else if (direction == 2 && enc->charge == 3){
		enc->decrement(enc->counter);
		enc->charge = -1;
	}
}


void onRotaryCallback(void *context){
	encoder_t *encdr = (encoder_t *)context;
	onRotaryChange(encdr);
}


/* encoderInit: Initialize the encoder */
void encoderInit(encoder_t *encoder, GPIO_PINS clk_pin,
	GPIO_PINS dt_pin, GPIO_PINS btn_pin,
	callbackFunction_t btn_callback,
	encoderCallback_t increment_cb,
	encoderCallback_t decrement_cb){

	encoder->clk_pin = clk_pin;
	encoder->btn_pin = btn_pin;
	encoder->dt_pin = dt_pin;

	// Initialize the encoder's pins
	gpio_t clk = gpinInit(clk_pin, GPIO_INPUT_MODE, GPIN_NO_PULL);
	gpio_t dt = gpinInit(dt_pin, GPIO_INPUT_MODE, GPIN_NO_PULL);
	gpio_t btn = gpinInit(btn_pin, GPIO_INPUT_MODE, GPIN_NO_PULL);

	encoder->increment = increment_cb;
	encoder->decrement = decrement_cb;
	encoder->dt = dt;
	encoder->clk = clk;

	encoder->pos = 0;
	encoder->lastPos = 0;
	encoder->counter = 0;
	encoder->first = 1;
	encoder->value = 0;
	encoder->charge = 0;

	gpinSetInterrupt(&clk, IRQ_RISING_FALLING, IRQ_VERY_HIGH_PRIORITY, onRotaryCallback, (void *)encoder);
	gpinSetInterrupt(&dt, IRQ_RISING_FALLING, IRQ_VERY_HIGH_PRIORITY, onRotaryCallback, (void *)encoder);
	gpinSetInterrupt(&btn, IRQ_FALLING, IRQ_HIGH_PRIORITY, btn_callback, NULL);

}


/* encoderGetCounter: Return the value of the encoder's counter */
uint32_t encoderGetCounter(encoder_t *encoder){
	return encoder->counter;
}

/* encoderResetCounter: Reset the encoder's counter */
void encoderResetCounter(encoder_t *encoder){
	encoder->pos = 0;
	encoder->lastPos = 0;
	encoder->counter = 0;
	encoder->first = 1;
	encoder->value = 0;
	encoder->charge = 0;
}

