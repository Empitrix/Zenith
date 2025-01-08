#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "uart.h"

#define STDOUT_FILENO   1
#define STDERR_FILENO   2

serial_t serialType = NO_SERIAL;


UART_HandleTypeDef huart3;

static void MX_USART3_UART_Init(int baudRate){
	huart3.Instance = USART3;
	huart3.Init.BaudRate = baudRate;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart3) != HAL_OK){
		Error_Handler();
	}
}

void serialBegin(serial_t type, int baudRate){
	switch (type){
		case UART1_SERIAL:
			break;
		case UART2_SERIAL:
			break;
		case UART3_SERIAL:
			MX_USART3_UART_Init(baudRate);
			break;
		case VIRTUAL_SERIAL:
			MX_USB_DEVICE_Init();
			break;
			default: break;
	}
	serialType = type;
}

uart_t uartInit(serial_t type, int baudRate){
	uart_t uart;
	switch (type){
		case UART1_SERIAL:
			break;
		case UART2_SERIAL:
			break;
		case UART3_SERIAL:
			MX_USART3_UART_Init(baudRate);
			break;
		case VIRTUAL_SERIAL:
			MX_USB_DEVICE_Init();
			break;
			default: break;
	}

	uart.type = type;
	uart.baudRate = baudRate;

	return uart;
}


void serialPrint(const char * frmt, ...){
	if(serialType == NO_SERIAL){ return; }
	char tmp[1024] = { 0 };
	va_list arglist;
	va_start(arglist, frmt);
	vsprintf(tmp, frmt, arglist);
	va_end(arglist);
	if(serialType == VIRTUAL_SERIAL){
		CDC_Transmit_FS((uint8_t *)tmp, strlen(tmp) * sizeof(uint8_t));
	} else {
		HAL_UART_Transmit(&huart3, (uint8_t *)tmp, strlen(tmp) * sizeof(uint8_t), 1000);
	}
}


void uartSetSTDOUT(uart_t *uart){ uart_write = *uart; }
void uartSetSTDOIN(uart_t *uart){ uart_read = *uart; }


// Update 'printf'
int _write(int file, char *ptr, int len){
	(void)file;
	switch (uart_write.type) {
		case UART1_SERIAL:
			break;
		case UART2_SERIAL:
			break;
		case UART3_SERIAL:
			HAL_UART_Transmit(&huart3, (uint8_t *)ptr, len, 1000);
			break;
		case VIRTUAL_SERIAL:
			CDC_Transmit_FS((uint8_t *)ptr, len);
			break;
		case NO_SERIAL: break;
		default: break;
	}
	return 0;
}

