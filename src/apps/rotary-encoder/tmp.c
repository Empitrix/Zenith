// #include "board.h"
// #include "timer.h"
// #include "gpio.h"
// #include "encoder.h"
// #include "uart.h"
// #include <stdio.h>
// 
// 
// static int v = 0;
// static int pos = 0;
// static int pos_before = 0;
// static int counter = 0;
// 
// void timCallback(timeHandle_t _){
// 	// printf("V: %d (Pos: %d, PosB: %d)\t\t(%d)\n", v, pos, pos_before, pos == pos_before);
// 	printf("Counter: %d\n", counter / 4);
// }
// 
// // gpio_t led, led2, led3;
// encoder_t enc;
// 
// // void onPressCallback(void *x){
// // 	gpinToggle(&led);
// // }
// // 
// // void onIncrement(int counter){
// // 	gpinToggle(&led2);
// // }
// // 
// // void onDecrement(int counter){
// // 	printf("Counter: %d\n", counter);
// // 	gpinToggle(&led3);
// // }
// 
// int count = 0;
// 
// static int sA = 0;
// static int sB = 0;
// 
// 
// int g2b(){
// 	if(sA == PIN_OFF && sB == PIN_OFF){
// 		return 0;
// 	} else if (sA == PIN_OFF && sB == PIN_ON){
// 		return 1;
// 	} else if (sA == PIN_ON && sB == PIN_ON){
// 		return 2;
// 	} else {
// 		return 3;
// 	}
// 	return 0;
// }
// 
// 
// int main(){
// 	boardInit();
// 	uart_t uart = uartInit(UART3, 115200);
// 	uartSetSTDOUT(&uart);
// 
// 	gpio_t ledA = gpinInit(A_4, GPIO_OUTPUT_MODE, GPIN_NO_PULL);
// 	gpio_t ledB = gpinInit(A_6, GPIO_OUTPUT_MODE, GPIN_NO_PULL);
// 	gpio_t ledC = gpinInit(A_5, GPIO_OUTPUT_MODE, GPIN_NO_PULL);
// 	gpio_t ledD = gpinInit(A_7, GPIO_OUTPUT_MODE, GPIN_NO_PULL);
// 
// 	gpinSet(&ledA, PIN_OFF);
// 	gpinSet(&ledB, PIN_OFF);
// 	gpinSet(&ledC, PIN_OFF);
// 	gpinSet(&ledD, PIN_OFF);
// 
// 	// led = gpinInit(C_13, GPIO_OUTPUT_MODE, GPIN_NO_PULL);
// 
// 	// led2 = gpinInit(A_3, GPIO_OUTPUT_MODE, GPIN_NO_PULL);
// 	// led3 = gpinInit(A_4, GPIO_OUTPUT_MODE, GPIN_NO_PULL);
// 
// 	// encoderInit(&enc, A_8, A_9, A_10, onPressCallback, onIncrement, onDecrement);
// 
// 	gpio_t pA = gpinInit(A_8, GPIO_INPUT_MODE, GPIN_PULL_UP);
// 	gpio_t pB = gpinInit(A_9, GPIO_INPUT_MODE, GPIN_PULL_UP);
// 
// 	timerInit(TIMER_2, MS(100), timCallback, 1); // just check for _V_
// 
// 	while(1){
// 		// printf("Looping: %d\n", count++);
// 		// delayMs(100);
// 		sB = gpinRead(&pB);
// 		sA = gpinRead(&pA);
// 		pos = g2b();
// 		v = pos_before - pos;
// 
// 		if(v == -1 || v == 3){ counter--; }
// 		if(v == 1 || v == -3){ counter++; }
// 
// 
// 
// 		gpinSet(&ledA, PIN_OFF);
// 		gpinSet(&ledB, PIN_OFF);
// 		gpinSet(&ledC, PIN_OFF);
// 		gpinSet(&ledD, PIN_OFF);
// 
// 		switch (counter / 4){
// 			case 1:
// 				gpinSet(&ledA, PIN_ON);
// 				break;
// 			case 3:
// 				gpinSet(&ledB, PIN_ON);
// 				break;
// 			case 2:
// 				gpinSet(&ledC, PIN_ON);
// 				break;
// 			case 4:
// 				gpinSet(&ledD, PIN_ON);
// 				break;
// 			default: break;
// 		}
// 
// 		pos_before = pos;
// 
// 		// __asm__ volatile ("nop");
// 	}
// }
