#include "timer.h"
#include "board.h"
#include "gpio.h"

#define DELAY_VALUE 1000

GPIN led1;
GPIN led2;
GPIN led3;

GPIN c13;

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

	gpinInit(&led1, GPIN_2, GPIO_TYPE_A, GPIO_OUTPUT_MODE, GPIN_NO_PULL);
	gpinInit(&led2, GPIN_3, GPIO_TYPE_A, GPIO_OUTPUT_MODE, GPIN_NO_PULL);
	gpinInit(&led3, GPIN_4, GPIO_TYPE_A, GPIO_OUTPUT_MODE, GPIN_NO_PULL);

	gpinInit(&c13, GPIN_13, GPIO_TYPE_C, GPIO_OUTPUT_MODE, GPIN_NO_PULL);

	timr1 = timerInit(TIMER_2, 200, &callback1, 1);
	timr2 = timerInit(TIMER_3, 400, &callback2, 1);
	timr3 = timerInit(TIMER_4, 800, &callback3, 1);


	// BlUE-PILL c-13 blink
	while (1){
		gpinToggle(&c13);
		delayMs(200);
	}

}

