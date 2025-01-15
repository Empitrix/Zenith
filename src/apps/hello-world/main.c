#include "board.h"
#include "uart.h"
#include "gpio.h"
#include <stdio.h>


int main(void){
	boardInit();
	uart_t usbCDC = uartInit(USBCDC, 115200);
	uart_t uart1 = uartInit(UART1, 115200);
	uart_t uart2 = uartInit(UART2, 115200);
	uart_t uart3 = uartInit(UART3, 115200);


	while(1){

		uartSetSTDOUT(&usbCDC);
		printf("Hello, from USB CDC\n");

		uartSetSTDOUT(&uart1);
		printf("Hello, from UART 1\n");

		uartSetSTDOUT(&uart2);
		printf("Hello, from UART 2\n");

		uartSetSTDOUT(&uart3);
		printf("Hello, from UART 3\n");

	}
}

