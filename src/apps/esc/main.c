#include "board.h"
#include "gpio.h"
#include "stm32f1xx_hal.h"
#include "uart.h"
#include "timer.h"
#include <stdio.h>
#include <stdlib.h>
#include "neopixel.h"

/*
Capture the input frequency at pin B5.
*/

#define NEOPIXEL_MATRIX_SIZE 25



static int irq_flag = 0;

void calbk(timeHandle_t htim){
	irq_flag = 1;
}

int main(void){
	boardInit();
	uart_t uart = uartInit(UART3, 115200);
	uartSetSTDOUT(&uart);

	int ticks = 13;
	timer_t timer;
	timerCaptureInit(&timer, B5_TIM3_CH2, CAPTURE_FALLIGN, ticks, calbk);


	gpio_t pinOut = gpinInit(B_4, GPIO_OUTPUT_MODE, GPIN_NO_PULL);
	gpinSet(&pinOut, PIN_ON);

	neopixel_t npxl = neopixelInit(NEOPIXEL_MATRIX_SIZE, B_6);
	neopixelClearAll(&npxl);
	neopixelShow(&npxl);

	// int rpm = 0;
	while(1){

		// while(irq_flag == 0){ delayMs(10); }
		while(irq_flag == 0){ __asm__ volatile ("nop"); }

		delayMs(3);
		// delay_us(3240);
		// for(int i = 0; i < 500; i++) __asm__ volatile ("nop");

		gpinSet(&pinOut, PIN_OFF);
		// neopixelSetAll(&npxl);
		// neopixelShow(&npxl);

		// for(int i = 0; i < 6650; i++) __asm__ volatile ("nop");

		// delayMs(1000);
		// neopixelClearAll(&npxl);
		// neopixelShow(&npxl);
		gpinSet(&pinOut, PIN_ON);
		irq_flag = 0;

	}

	return 0;
}

		// rpm = 60 * (1 / ((timer.capture.ccr * ticks) * 0.000001));
		// printf("CCR: %d, RPM: %d\n", timer.capture.ccr, rpm);
