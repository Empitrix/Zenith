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
	gpinInit(B_5, GPIO_INPUT_MODE, GPIN_NO_PULL);  // Set B_5 as input

	// Initialize timer
	timer_t timer = timerInit(TIMER_3, CAPTURE_FREQUENCY, NULL, 1);
	// Initialize capture mode
	timerCaptureInit(&timer, TIMER_CH_2, CAPTURE_RISING, callback);

	while(1){
		printf("Frequency = %u Hz\n", timer.capture.frequency);
	}
	return 0;
}

