#include "board.h"
#include "gpio.h"
#include "uart.h"
#include "timer.h"
#include <stdio.h>


void clkbk(timeHandle_t _){ }

void callback(timeHandle_t htim){
	// printf("I feel something! (%d)\n", (int)htim.Instance->CCR1);
	printf("Callback! Counter: %d, CCR: %d\n", (int)htim.Instance->CNT, (int)htim.Instance->CCR2);
}


int main(void){
	boardInit();
	uart_t uart = uartInit(USBCDC, 115200);
	uartSetSTDOUT(&uart);


	gpinInit(B_5, GPIO_INPUT_MODE, GPIN_NO_PULL);

	timer_t timer = timerInit(TIMER_3, 0, clkbk, 1);
	timerCaptureInit(&timer, TIMER_CH_2, CAPTURE_RISING, callback);

	while(1){

		// printf("%d\n", (int)TIM3->CCR1);
		// printf("SOMETHING (%d) (%d)\n", (int)TIM2->CCR1, (int)TIM3->CCR1);
	}
	return 0;
}

