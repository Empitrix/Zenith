#include <stdint.h>
#include "main.h"

#ifndef __BOARD_GPIO__
#define __BOARD_GPIO__

typedef enum {
	UART1_SERIAL,
	UART2_SERIAL,
	UART3_SERIAL,
	VIRTUAL_SERIAL,
	NO_SERIAL
} serial_t;


typedef struct {
	serial_t type;
	int baudRate;
} uart_t; 


void serialBegin(serial_t type, int baudRate);
void serialPrint(const char * frmt, ...);


uart_t uartInit(serial_t type, int baudRate);

static volatile uart_t uart_write, uart_read;

// void uartInit(serial_t type, int baudrate);
void uartSetSTDOUT(uart_t *uart);
void uartSetSTDOIN(uart_t *uart);


#endif
