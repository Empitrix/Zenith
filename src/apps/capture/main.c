#include "board.h"
#include "gpio.h"
#include "uart.h"
#include "timer.h"
#include <stdio.h>
#include <stdlib.h>

/*
Capture the input frequency at pin B5.
*/


void callback(timeHandle_t htim){}

void calbk(timeHandle_t htim){}



int main(void){
	boardInit();
	uart_t uart = uartInit(UART3, 115200);
	uartSetSTDOUT(&uart);

	int ticks = 13;

	timer_t timer;
	timerCaptureInit(&timer, B5_TIM3_CH2, CAPTURE_RISING, ticks, calbk);

	int rpm = 0;
	while(1){
		rpm = 60 * (1 / ((timer.capture.ccr * ticks) * 0.000001));
		printf("%d (%d) RPM: %d\n", abs(timer.capture.ccr), abs(timer.capture.ccr * ticks), rpm);
		delayMs(100);
	}

	return 0;
}

