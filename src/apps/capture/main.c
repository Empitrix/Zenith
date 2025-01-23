#include "board.h"
#include "gpio.h"
#include "uart.h"
#include "timer.h"
#include <stdio.h>

/*
Capture the input frequency at pin B5.
*/


gpio_t g;

void callback(timeHandle_t htim){}

void calbk(timeHandle_t htim){
	gpinToggle(&g);
}



int main(void){
	boardInit();
	uart_t uart = uartInit(UART3, 115200);
	uartSetSTDOUT(&uart);

	g = gpinInit(B_4, GPIO_OUTPUT_MODE, GPIN_NO_PULL);

	int tim = 20;

	timer_t timer;
	timerCaptureInit(&timer, B5_TIM3_CH2, CAPTURE_RISING, tim, calbk);

	int toggle = 0;
	int previus = 0;

	while(1){
		printf("CCR: %d\n", timer.capture.ccr);

		toggle++;
		int a = timer.capture.ccr - previus;
		if(toggle == 2){ printf("             %d (%d)\n", a, a * tim); toggle = 0;} else {
		 previus = timer.capture.ccr; 
		}
		delayMs(100);
	}

	return 0;
}

