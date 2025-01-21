#include "board.h"
#include "gpio.h"
#include "uart.h"
#include "timer.h"
#include <stdio.h>

/*
Capture the input frequency at pin B5.
*/


void callback(timeHandle_t htim){}


int main(void){
	boardInit();
	uart_t uart = uartInit(UART3, 115200);
	uartSetSTDOUT(&uart);

	timer_t timer;
	timerCaptureInit(&timer, B5_TIM3_CH2, CAPTURE_RISING, 0, callback);

	while(1){
		printf("Frequency: %d\n", timer.capture.frequency);
		delayMs(100);
	}
	return 0;
}

