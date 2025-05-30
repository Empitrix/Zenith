#include <stdint.h>
#include "main.h"

#ifndef __BOARD_UART__
#define __BOARD_UART__

typedef enum {
	UART1,
	UART2,
	UART3,
	USBCDC,
} serial_t;


typedef struct {
	serial_t type;
	int baudRate;
} uart_t; 


void serialBegin(serial_t type, int baudRate);
void serialPrint(const char * frmt, ...);


uart_t uartInit(serial_t type, int baudRate);

static volatile uart_t uart_write, uart_read;

void uartSetSTDOUT(uart_t *uart);
void uartSetSTDIN(uart_t *uart);


#endif
