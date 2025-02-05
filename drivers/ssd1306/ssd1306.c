#include "ssd1306.h"
#include "board.h"
#include <stdint.h>
#include <string.h>


char ssd1306Buffer[SCREEN_BUFFER_SIZE] = { 0 };


void writeCommand(i2c_t *conn, uint8_t data){
	i2cMemWrite(conn, SSD1306_I2C_DEV_ADDR, 0, 1, (uint8_t *)&data, 1);
}

void writeData(i2c_t *conn, uint8_t *data, int size){
	i2cMemWrite(conn, SSD1306_I2C_DEV_ADDR, 0x40, 1, data, size);
}


void ssd1306setDisplay(ssd1306_t *display, int status){
	if(status){
		display->displayOn = 1;
		writeCommand(display->conn, 0xAF);
	} else {
		display->displayOn = 0;
		writeCommand(display->conn, 0xAE);
	}
}

void ssd1306SetContrast(ssd1306_t ssd, const uint8_t value) {
	const uint8_t contrastRegister = 0x81;
	writeCommand(ssd.conn, contrastRegister);
	writeCommand(ssd.conn, value);
}


void ssd1306Flush(ssd1306_t *ssd){
	for(int i = 0; i < SSD1306_HEIGHT / 8 ; i++){
		writeCommand(ssd->conn, 0xB0 + i); // Set the current RAM page address.
		writeCommand(ssd->conn, 0x00 + 0);
		writeCommand(ssd->conn, 0x10 + 0);
		writeData(ssd->conn, (uint8_t *)&ssd1306Buffer[SSD1306_WIDTH*i], SSD1306_WIDTH);
	}
}


void ssd1306SetAll(void){
	memset(ssd1306Buffer, 0xFF, sizeof(ssd1306Buffer));
}

void ssd1306ClearAll(void){
	memset(ssd1306Buffer, 0x00, sizeof(ssd1306Buffer));
}


ssd1306_t ssd1306Init(i2c_t *i2c){
	ssd1306_t display;
	display.conn = i2c;

	delayMs(100);

	ssd1306setDisplay(&display, 0);


	// Set Memory Addressing Mode
	writeCommand(i2c, 0x20);
	// 0x00, Horizontal Addressing Mode; 01b, Vertical Addressing Mode; 10b,Page Addressing Mode (RESET); 11b, Invalid
	writeCommand(i2c, 0x00);
	// Set Page Start Address for Page Addressing Mode,0-7
	writeCommand(i2c, 0xB0);

#ifdef SSD1306_MIRROR_VERT
	// Mirror vertically
	writeCommand(i2c, 0xC0);
#else
	// Set COM Output Scan Direction
	writeCommand(i2c, 0xC8);
#endif


	// set low column address
	writeCommand(i2c, 0x00);
	// set high column address
	writeCommand(i2c, 0x10);
	// set start line address
	writeCommand(i2c, 0x40);

	ssd1306SetContrast(display, 0xFF);

#ifdef SSD1306_MIRROR_HORIZ
	// Mirror horizontally
	writeCommand(i2c, 0xA0);
#else
	// Set segment re-map 0 to 127 - CHECK
	writeCommand(i2c, 0xA1);
#endif


#ifdef SSD1306_INVERSE_COLOR
	// Set inverse color
	writeCommand(i2c, 0xA7);
#else
	// Set normal color
	writeCommand(i2c, 0xA6);
#endif



// Set multiplex ratio.
#if (SSD1306_HEIGHT == 128)
	// Found in the Luma Python lib for SH1106.
	writeCommand(i2c, 0xFF);
#else
	// Set multiplex ratio(1 to 64)
	writeCommand(i2c, 0xA8);
#endif


#if (SSD1306_HEIGHT == 32)
	writeCommand(i2c, 0x1F);
#elif (SSD1306_HEIGHT == 64)
	writeCommand(i2c, 0x3F);
#elif (SSD1306_HEIGHT == 128)
	writeCommand(i2c, 0x3F);
#else
#error "Only 32, 64, or 128 lines of height are supported!"
#endif


	writeCommand(i2c, 0xA4);  // 0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	writeCommand(i2c, 0xD3);  // Set display offset
	writeCommand(i2c, 0x00);  // Not offset
	writeCommand(i2c, 0xD5);  // Set display clock divide ratio/oscillator frequency
	writeCommand(i2c, 0xF0);  // Set divide ratio
	writeCommand(i2c, 0xD9);  // Set pre-charge period
	writeCommand(i2c, 0x22);
	writeCommand(i2c, 0xDA);  // Set com pins hardware configuration - CHECK

#if(SSD1306_HEIGHT == 32)
	writeCommand(i2c, 0x02);
#elif (SSD1306_HEIGHT == 64)
	writeCommand(i2c, 0x12);
#elif (SSD1306_HEIGHT == 128)
	writeCommand(i2c, 0x12);
#else
#error "Only 32, 64, or 128 lines of height are supported!"
#endif

	writeCommand(i2c, 0xDB); // Set vcomh
	writeCommand(i2c, 0x20); // 0x20,0.77xVcc
	writeCommand(i2c, 0x8D); // Set DC-DC enable
	writeCommand(i2c, 0x14);
	ssd1306setDisplay(&display, 1);


	memset(ssd1306Buffer, 0x00, sizeof(ssd1306Buffer));
	ssd1306Flush(&display);

	display.x = 0;
	display.y = 0;
	display.initialized = 1;

	return display;
}

