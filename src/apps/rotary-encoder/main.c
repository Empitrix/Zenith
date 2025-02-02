#include "board.h"
#include "encoder.h"
#include "uart.h"
#include <stdio.h>

encoder_t enc;

/* Encoder's button callback */
void onPressCallback(void *_){
	// Reset the encoder's counter
	encoderResetCounter(&enc);
}

/* Increment callback */
void onIncrement(int counter){
	printf("Increment (%d)\n", counter);
}

/* Decrement callback */
void onDecrement(int counter){
	printf("Decrement (%d)\n", counter);
}

int main(){
	// Initialize the board
	boardInit();

	// Initialize UART & enable printf
	uart_t uart = uartInit(UART3, 115200);
	uartSetSTDOUT(&uart);

	// Initialize the encoder
	encoderInit(&enc, A_8, A_9, A_10, onPressCallback, onIncrement, onDecrement);

	while(1) __asm__ volatile ("nop");  // Keep the program running
}

