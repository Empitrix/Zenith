#include "board.h"
#include "uart.h"
#include "gpio.h"
#include <stdio.h>


int main(void){
	boardInit();
	uart_t vcom = uartInit(VIRTUAL_SERIAL, 115200);
	uartSetSTDOUT(&vcom);
	uartSetSTDIN(&vcom);

	char echoStr[127] = { 0 };

	while(1){
		scanf("%s", echoStr);
		printf("%s\n", echoStr);
		delayMs(10);
	}
}

