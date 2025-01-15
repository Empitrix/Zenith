#include <stdarg.h>
#include <stdint.h>
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "uart.h"
#include "gpio.h"

#define STDOUT_FILENO   1
#define STDERR_FILENO   2

// serial_t serialType = NO_SERIAL;


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
		case UART1:
			break;
		case UART2:
			break;
		case UART3:
			MX_USART3_UART_Init(baudRate);
			break;
		case USBCDC:
			MX_USB_DEVICE_Init();
			break;
			default: break;
	}
	// serialType = type;
}

uart_t uartInit(serial_t type, int baudRate){
	uart_t uart;
	switch (type){
		case UART1:
			break;
		case UART2:
			break;
		case UART3:
			MX_USART3_UART_Init(baudRate);
			break;
		case USBCDC:
			MX_USB_DEVICE_Init();
			break;
			default: break;
	}

	uart.type = type;
	uart.baudRate = baudRate;

	return uart;
}


/*
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
*/


void uartSetSTDOUT(uart_t *uart){ uart_write = *uart; }
void uartSetSTDIN(uart_t *uart){ uart_read = *uart; }



// Update 'printf'
int _write(int file, char *ptr, int len){
	(void)file;
	switch (uart_write.type) {
		case UART1:
			break;
		case UART2:
			break;
		case UART3:
			HAL_UART_Transmit(&huart3, (uint8_t *)ptr, len, 1000);
			break;
		case USBCDC:
			CDC_Transmit_FS((uint8_t *)ptr, len);
			break;
		// case NO_SERIAL: break;
		default: break;
	}
	return 0;
}



#define RX_BUFFER_SIZE 256
static volatile uint8_t rxBuffer[RX_BUFFER_SIZE];
static volatile uint8_t rxLength = 0;


int _read(int file, char *ptr, int len) {
	(void)file;

	memset((char *)rxBuffer, '\0', sizeof(rxBuffer));
	rxLength = 0;

	if(uart_read.type == USBCDC){
		while(rxLength == 0){}  // Wait for input
		strcpy(ptr, (char *)rxBuffer);
	}

	return rxLength;
}

void USB_CDC_RxHandler(uint8_t* Buf, uint32_t Len){
	strcpy((char *)rxBuffer, (char *)Buf);
	rxLength = Len;
}

