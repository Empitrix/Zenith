#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "board.h"
#include <stdint.h>
#include <string.h>


#define ABS(x) ((x) < 0 ? -(x) : (x))


void writeCommand(ssd1306_t *display, uint8_t data){
	i2cMemWrite(display->conn, display->devAddr, 0, 1, (uint8_t *)&data, 1);
}

void writeData(ssd1306_t *display, uint8_t *data, int size){
	i2cMemWrite(display->conn, display->devAddr, 0x40, 1, data, size);
}


void ssd1306setDisplay(ssd1306_t *display, int status){
	if(status){
		display->displayOn = 1;
		writeCommand(display, 0xAF);
	} else {
		display->displayOn = 0;
		writeCommand(display, 0xAE);
	}
}

void ssd1306SetContrast(ssd1306_t *display, const uint8_t value){
	const uint8_t contrastRegister = 0x81;
	writeCommand(display, contrastRegister);
	writeCommand(display, value);
}


void ssd1306Flush(ssd1306_t *display){
	for(int i = 0; i < display->height / 8 ; i++){
		writeCommand(display, 0xB0 + i); // Set the current RAM page address.
		writeCommand(display, 0x00 + 0);
		writeCommand(display, 0x10 + 0);
		writeData(display, (uint8_t *)&display->buffer[display->width * i], display->width);
	}
}


void ssd1306SetAll(ssd1306_t *display){
	memset(display->buffer, 0xFF, display->bufferSize);
}

void ssd1306ClearAll(ssd1306_t *display){
	memset(display->buffer, 0x00, display->bufferSize);
}


ssd1306_t ssd1306Init(i2c_t *i2c, int devAddr, int width, int height, uint8_t *buffer){
	ssd1306_t display;

	display.conn = i2c;
	display.devAddr = devAddr;
	display.width = width;
	display.height = height;
	display.bufferSize = (height * width) / 8;
	display.buffer = buffer;

	delayMs(100);

	ssd1306setDisplay(&display, 0);


	// Set Memory Addressing Mode
	writeCommand(&display, 0x20);
	// 0x00, Horizontal Addressing Mode; 01b, Vertical Addressing Mode; 10b,Page Addressing Mode (RESET); 11b, Invalid
	writeCommand(&display, 0x00);
	// Set Page Start Address for Page Addressing Mode,0-7
	writeCommand(&display, 0xB0);

#ifdef SSD1306_MIRROR_VERT
	// Mirror vertically
	writeCommand(&display, 0xC0);
#else
	// Set COM Output Scan Direction
	writeCommand(&display, 0xC8);
#endif


	// set low column address
	writeCommand(&display, 0x00);
	// set high column address
	writeCommand(&display, 0x10);
	// set start line address
	writeCommand(&display, 0x40);

	ssd1306SetContrast(&display, 0xFF);

#ifdef SSD1306_MIRROR_HORIZ
	// Mirror horizontally
	writeCommand(&display, 0xA0);
#else
	// Set segment re-map 0 to 127 - CHECK
	writeCommand(&display, 0xA1);
#endif


#ifdef SSD1306_INVERSE_COLOR
	// Set inverse color
	writeCommand(&display, 0xA7);
#else
	// Set normal color
	writeCommand(&display, 0xA6);
#endif



	// Set multiplex ratio.
	if(display.height == 128){
		// Found in the Luma Python lib for SH1106.
		writeCommand(&display, 0xFF);
	} else {
		// Set multiplex ratio(1 to 64)
		writeCommand(&display, 0xA8);
	}


	if(display.height == 32){
		writeCommand(&display, 0x1F);
	} else if (display.height == 64){
		writeCommand(&display, 0x3F);
	} else if (display.height == 128){
		writeCommand(&display, 0x3F);
	} else {
		// "Only 32, 64, or 128 lines of height are supported!";
	}

	writeCommand(&display, 0xA4);  // 0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	writeCommand(&display, 0xD3);  // Set display offset
	writeCommand(&display, 0x00);  // Not offset
	writeCommand(&display, 0xD5);  // Set display clock divide ratio/oscillator frequency
	writeCommand(&display, 0xF0);  // Set divide ratio
	writeCommand(&display, 0xD9);  // Set pre-charge period
	writeCommand(&display, 0x22);
	writeCommand(&display, 0xDA);  // Set com pins hardware configuration - CHECK

	if(display.height == 32){
		writeCommand(&display, 0x02);
	} else if(display.height == 64){
		writeCommand(&display, 0x12);
	} else if(display.height == 128){
		writeCommand(&display, 0x12);
	} else {
		// #error "Only 32, 64, or 128 lines of height are supported!"
	}

	writeCommand(&display, 0xDB); // Set vcomh
	writeCommand(&display, 0x20); // 0x20,0.77xVcc
	writeCommand(&display, 0x8D); // Set DC-DC enable
	writeCommand(&display, 0x14);
	ssd1306setDisplay(&display, 1);


	memset(display.buffer, 0x00, display.bufferSize);
	ssd1306Flush(&display);

	display.x = 0;
	display.y = 0;
	display.initialized = 1;

	return display;
}


void ssd1306DrawPixel(ssd1306_t *dispaly, uint8_t x, uint8_t y, ssd1306Color_t color) {
	if(x >= dispaly->width || y >= dispaly->height) { return; }
	if(color == ssd1306White) {
		dispaly->buffer[x + (y / 8) * dispaly->width] |= 1 << (y % 8);
	} else { 
		dispaly->buffer[x + (y / 8) * dispaly->width] &= ~(1 << (y % 8));
	}
}


/* Position the cursor */
void ssd1306SetCursor(ssd1306_t *display, uint8_t x, uint8_t y){
	display->x = x;
	display->y = y;
}


void ssd1306DrawLine(ssd1306_t *display, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, ssd1306Color_t color){
	int32_t deltaX = ABS(x2 - x1);
	int32_t deltaY = ABS(y2 - y1);
	int32_t signX = ((x1 < x2) ? 1 : -1);
	int32_t signY = ((y1 < y2) ? 1 : -1);
	int32_t error = deltaX - deltaY;
	int32_t error2;
	
	ssd1306DrawPixel(display, x2, y2, color);

	while((x1 != x2) || (y1 != y2)) {
		ssd1306DrawPixel(display, x1, y1, color);
		error2 = error * 2;
		if(error2 > -deltaY) {
			error -= deltaY;
			x1 += signX;
		}
		
		if(error2 < deltaX) {
			error += deltaX;
			y1 += signY;
		}
	}
	return;
}


void ssd1306DrawRectangle(ssd1306_t *display, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, ssd1306Color_t color){
	ssd1306DrawLine(display, x1,y1,x2,y1,color);
	ssd1306DrawLine(display, x2,y1,x2,y2,color);
	ssd1306DrawLine(display, x2,y2,x1,y2,color);
	ssd1306DrawLine(display, x1,y2,x1,y1,color);
	return;
}


void ssd1306DrawFilledRectangle(ssd1306_t *dispaly, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, ssd1306Color_t color) {
	uint8_t x_start = ((x1<=x2) ? x1 : x2);
	uint8_t x_end   = ((x1<=x2) ? x2 : x1);
	uint8_t y_start = ((y1<=y2) ? y1 : y2);
	uint8_t y_end   = ((y1<=y2) ? y2 : y1);
	for (uint8_t y= y_start; (y<= y_end)&&(y < dispaly->height); y++) {
		for (uint8_t x= x_start; (x<= x_end)&&(x < dispaly->width); x++) {
			ssd1306DrawPixel(dispaly, x, y, color);
		}
	}
	return;
}


char ssd1306DrawChar(ssd1306_t *display, char character, ssd1306Font_t font, ssd1306Color_t color){
	uint32_t i, b, j;
	
	// Check if character is valid
	if(character < 32 || character > 126){ return 0; }
	
	// Check remaining space on current line
	if (display->width < (display->x + font.width) ||
		display->height < (display->y + font.height))
	{
		// Not enough space on current line
		return 0;
	}
	
	// Use the font to write
	for(i = 0; i < font.height; i++) {
		b = font.data[(character - 32) * font.height + i];
		for(j = 0; j < font.width; j++) {
			if((b << j) & 0x8000)  {
				ssd1306DrawPixel(display, display->x + j, (display->y + i), (ssd1306Color_t)color);
			} else {
				ssd1306DrawPixel(display, display->x + j, (display->y + i), (ssd1306Color_t)!color);
			}
		}
	}
	
	// The current space is now taken
	display->x += font.size ? font.size[character - 32] : font.width;
	
	// Return written char for validation
	return character;
}

/* Write full string to screenbuffer */
char ssd1306DrawString(ssd1306_t *display, char* str, ssd1306Font_t font, ssd1306Color_t color){
	while (*str) {
		if (ssd1306DrawChar(display, *str, font, color) != *str) {
			// Char could not be written
			return *str;
		}
		str++;
	}
	
	// Everything ok
	return *str;
}
