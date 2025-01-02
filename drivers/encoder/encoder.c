#include "encoder.h"
#include <stdbool.h>


void onRotaryChange(encoder_t *enc){
	enc->state = (int)gpinRead(&enc->clk);
	if(enc->state != enc->lastState){
		if ((int)gpinRead(&enc->dt) != enc->state){ 
			enc->counter++;
			enc->increment(enc->counter);
		 } else {
			enc->counter--;
			enc->decrement(enc->counter);
		 }
	}
	enc->lastState = enc->state;
}


// void onRotaryChange(encoder_t *enc){
// 	static uint8_t state = 0;
// 	bool CLKstate = gpinRead(&enc->clk);
// 	bool DTstate = gpinRead(&enc->dt);
// 	switch (state) {
// 		case 0:
// 			if (!CLKstate){
// 				state = 1;
// 			} else if (!DTstate) {	 
// 				state = 4;
// 			}
// 			break;
// 		// Clockwise rotation
// 		case 1:
// 			if (!DTstate) {
// 				state = 2;
// 			} 
// 			break;
// 		case 2:
// 			if (CLKstate) {
// 				state = 3;
// 			}
// 			break;
// 		case 3:
// 			if (CLKstate && DTstate) { 
// 				state = 0;
// 				// ++inputDelta;
// 				// printFlag = true;
// 				enc->counter++;
// 				enc->increment(enc->counter);
// 			}
// 			break;
// 		// Anticlockwise rotation
// 		case 4:
// 			if (!CLKstate) {
// 				state = 5;
// 			}
// 			break;
// 		case 5:
// 			if (DTstate) {
// 				state = 6;
// 			}
// 			break;
// 		case 6:
// 			if (CLKstate && DTstate) {
// 				state = 0;
// 				// --inputDelta;
// 				// printFlag = true;
// 				enc->counter--;
// 				enc->decrement(enc->counter);
// 			}
// 			break; 
// 	}
// }

void onRotaryCallback(void *context){
	encoder_t *encdr = (encoder_t *)context;
	onRotaryChange(encdr);
}


void encoderInit(encoder_t *encoder, GPIO_PINS clk_pin,
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
	encoder->clk = clk;

	encoder->state = 0;
	encoder->lastState = 0;

	gpinSetInterrupt(&btn, IRQ_FALLING, IRQ_HIGH_PRIORITY, btn_callback, NULL);

	// gpinSetInterrupt(&clk, IRQ_RISING_FALLING, IRQ_VERY_HIGH_PRIORITY, onRotaryCallback, (void *)encoder);
	gpinSetInterrupt(&clk, IRQ_RISING_FALLING, IRQ_VERY_HIGH_PRIORITY, onRotaryCallback, (void *)encoder);

}


uint32_t encoderGetCounter(encoder_t *encoder){
	return encoder->counter;
}

void encoderResetCounter(encoder_t *encoder){
	encoder->counter = 0;
}

