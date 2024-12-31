#include "timer.h"
#include "board.h"
#include "gpio.h"

#define DELAY_VALUE 1000

gpio_t led1;
gpio_t led2;
gpio_t led3;

gpio_t c13;

void callback1(timeHandle_t htim){
	gpinToggle(&led1);
}

void callback2(timeHandle_t htim){
	gpinToggle(&led2);
}

void callback3(timeHandle_t htim){
	gpinToggle(&led3);
}

timer_t timr1;
timer_t timr2;
timer_t timr3;

int main(void){
	boardInit();

	led1 = gpinInit(A_2, GPIO_OUTPUT_MODE, GPIN_NO_PULL);
	led2 = gpinInit(A_3, GPIO_OUTPUT_MODE, GPIN_NO_PULL);
	led3 = gpinInit(A_4, GPIO_OUTPUT_MODE, GPIN_NO_PULL);
	c13  = gpinInit(C_13, GPIO_OUTPUT_MODE, GPIN_NO_PULL);

	timr1 = timerInit(TIMER_2, 200, &callback1, 1);
	timr2 = timerInit(TIMER_3, 400, &callback2, 1);
	timr3 = timerInit(TIMER_4, 800, &callback3, 1);

	// BlUE-PILL c-13 blink
	while (1){
		gpinToggle(&c13);
		delayMs(200);
	}

}

