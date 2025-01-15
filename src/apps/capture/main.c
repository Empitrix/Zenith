#include "board.h"
#include "gpio.h"
#include "timer.h"

gpio_t led1;
gpio_t led2;

void led1Toggle(timeHandle_t _){ gpinToggle(&led1); }
void led2Toggle(timeHandle_t _){ gpinToggle(&led2); }

int main(void){
	boardInit();

	led1 = gpinInit(A_2, GPIO_OUTPUT_MODE, GPIN_NO_PULL);
	led2 = gpinInit(A_3, GPIO_OUTPUT_MODE, GPIN_NO_PULL);
	
	timerInit(TIMER_2, MS(200), led1Toggle, 1);
	timerInit(TIMER_3, US(400), led2Toggle, 1);

	while(1){}
	return 0;
}

