#include "board.h"
#include "uart.h"
#include <stdio.h>

int main(void){
	boardInit();
	uart_t vcom = uartInit(VIRTUAL_SERIAL, 115200);
	uartSetSTDOUT(&vcom);

	while(1){
		printf("Hello, World!\n");
		delayMs(100);
	}
}
