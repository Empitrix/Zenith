#include "board.h"
#include "gpio.h"
#include "timer.h"

gpio_t led;
gpio_t led2;
timer_t tim1;
int running = 0;

void onTimer1Callback(){
	gpinToggle(&led);
}


void onGpioCallback(){
	if(running){
		timerPause(&tim1);
	} else {
		timerResume(&tim1);
	}
	running = running == 0;
	gpinToggle(&led2);
}

int counter = 0;

int main(void){
	boardInit();

	gpio_t c13 = gpinInit(C_13, GPIO_OUTPUT_MODE, GPIN_PULL_DOWN);
	gpio_t btn = gpinInit(A_9, GPIO_INPUT_MODE, GPIN_PULL_DOWN);
	gpinSetInterrupt(&btn, IRQ_RISING, IRQ_VERY_HIGH_PRIORITY, &onGpioCallback);

	led = gpinInit(A_3, GPIO_OUTPUT_MODE, GPIN_PULL_DOWN);
	led2 = gpinInit(A_4, GPIO_OUTPUT_MODE, GPIN_PULL_DOWN);

	tim1 = timerInit(TIMER_2, 200, &onTimer1Callback, 1);

	while(1){
		if(counter == 5){
			timerStop(&tim1);
		}

		if(counter == 10){
			timerStart(&tim1);
			timerSetInterval(&tim1, 200);
		}

		if(counter == 3){
			timerSetInterval(&tim1, 80);
		}

		if(counter == 15){
			timerReset(&tim1, 1);
		}

		// Blink
		gpinSet(&c13, PIN_ON);
		delayMs(50);
		gpinSet(&c13, PIN_OFF);

		// delayMs(1000);
		delayMs(timerGetRemainingTime(&tim1));

		counter++;
	}
}
