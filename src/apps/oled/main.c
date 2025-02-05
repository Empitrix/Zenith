#include "board.h"
#include "timer.h"
#include "uart.h"
#include "ssd1306.h"
#include "i2c.h"
#include "encoder.h"


char buff[128] = { 0 };

int main(void){
	boardInit();
	uart_t uart = uartInit(UART3, 115200);
	uartSetSTDOUT(&uart);


	i2c_t i2c = i2cInit(I2C1_B6_B7, 1000000);
	ssd1306_t ssd = ssd1306Init(&i2c);


	while(1){
		ssd1306SetAll();
		ssd1306Flush(&ssd);
		delayMs(100);
		ssd1306ClearAll();
		ssd1306Flush(&ssd);
		delayMs(100);
	}
	return 0;
}
