#include "board.h"
#include "timer.h"
#include "uart.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "i2c.h"
#include "encoder.h"

uint8_t displayBuffer[(128 * 64) / 8] = { 0 };

int main(void){
	boardInit();

	i2c_t i2c = i2cInit(I2C1_B6_B7, 1000000);
	ssd1306_t display = ssd1306Init(&i2c, SSD1306_I2C_DEV_ADDR, 128, 64, displayBuffer);

	while(1){

		// Draw Letter 'A' at [0, 0]
		ssd1306SetCursor(&display, 0, 0);
		ssd1306DrawChar(&display, 'A', FONT_16X26, ssd1306White);

		// Draw Letter 'e' at [100, 10]
		ssd1306SetCursor(&display, 100, 10);
		ssd1306DrawChar(&display, 'e', FONT_16X26, ssd1306White);

		// Draw a rectangle
		ssd1306DrawRectangle(&display, 8, 16, 24, 32, ssd1306White);

		// Draw an string
		ssd1306SetCursor(&display, 30, 40);
		ssd1306DrawString(&display, "This is Awesome", FONT_6X8, ssd1306White);

		// Draw a line under the string
		ssd1306DrawLine(&display, 78, 50, 118, 50, ssd1306White);

		// Draw a filled rectangle
		ssd1306DrawFilledRectangle(&display, 10, 50, 20, 60, ssd1306White);
		ssd1306Flush(&display);
	}
	return 0;
}

