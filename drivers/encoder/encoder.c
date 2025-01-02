#include "encoder.h"


void onRotaryChange(uint32_t *coutner, gpio_t *dt, encoderCallback_t increment, encoderCallback_t decrement){
	if((int)gpinRead(dt) == 0){
		*coutner = (*coutner) + 1;
		increment(*coutner);
	} else {
		*coutner = (*coutner) - 1;
		decrement(*coutner);
	}
}

void onRotaryCallback(void *context){
	Encoder_t *encdr = (Encoder_t *)context;
	onRotaryChange(&encdr->counter, &encdr->dt, encdr->increment, encdr->decrement);
}


void encoderInit(Encoder_t *encoder, GPIO_PINS clk_pin,
	GPIO_PINS dt_pin, GPIO_PINS btn_pin,
	callbackFunction_t btn_callback,
	encoderCallback_t increment_cb,
	encoderCallback_t decrement_cb){

	encoder->clk_pin = clk_pin;
	encoder->btn_pin = btn_pin;
	encoder->dt_pin = dt_pin;
	encoder->counter = 0;

	gpio_t clk = gpinInit(clk_pin, GPIO_INPUT_MODE, GPIN_NO_PULL);
	gpio_t dt = gpinInit(dt_pin, GPIO_INPUT_MODE, GPIN_NO_PULL);


	gpio_t btn = gpinInit(btn_pin, GPIO_INPUT_MODE, GPIN_PULL_UP);

	encoder->increment = increment_cb;
	encoder->decrement = decrement_cb;
	encoder->dt = dt;

	gpinSetInterrupt(&btn, IRQ_FALLING, IRQ_HIGH_PRIORITY, btn_callback, NULL);

	// gpinSetInterrupt(&clk, IRQ_RISING_FALLING, IRQ_VERY_HIGH_PRIORITY, onRotaryCallback, (void *)encoder);
	gpinSetInterrupt(&clk, IRQ_RISING_FALLING, IRQ_VERY_HIGH_PRIORITY, onRotaryCallback, (void *)encoder);

}


uint32_t encoderGetCounter(Encoder_t *encoder){
	return encoder->counter;
}

void encoderResetCounter(Encoder_t *encoder){
	encoder->counter = 0;
}

