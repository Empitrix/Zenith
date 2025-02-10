#include "board.h"
#include "uart.h"
#include "analog.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include <stdio.h>


static uint8_t displayBuffer[1024] = { 0 };


void callback(int _){}


/* oledShowValue: display the value on the OLED screen */
void oledShowValue(ssd1306_t *display, int value){
	char valBuf[128] = { 0 };
	sprintf(valBuf, "V: %d", value);
	ssd1306ClearAll(display);
	ssd1306SetCursor(display, 0, 0);
	ssd1306DrawString(display, valBuf, FONT_16X26, ssd1306White);
	ssd1306Flush(display);
}

int main(void){
	boardInit();
	uart_t uart = uartInit(UART3, 115200);
	uartSetSTDOUT(&uart);

	i2c_t i2c = i2cInit(I2C1_B6_B7, 1000000);
	ssd1306_t display = ssd1306Init(&i2c, SSD1306_I2C_DEV_ADDR, 128, 64, displayBuffer);


	// analog_t analog = analogInit(ADC1_B1);
	// analog_t analog = analogDMAInit(ADC1_B1, callback);

	analog_t analog;
	analogDMAInit(&analog, ADC1_B1, callback);

	while(1){
		oledShowValue(&display, analog.value);
		// delayMs(500);
	}
	return 0;
}


