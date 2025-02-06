#include "gpio.h"
#include "timer.h"
#include "i2c.h"
#include <stdint.h>

#ifndef __SSD1306_DRIVER__
#define __SSD1306_DRIVER__


// #define SSD1306_WIDTH   128
// #define SSD1306_HEIGHT  64


#define SCREEN_BUFFER_SIZE (SSD1306_WIDTH * SSD1306_HEIGHT) / 8

#define SSD1306_I2C_DEV_ADDR (0x3C << 1)



typedef struct {
	i2c_t *conn;
	int displayOn;
	int x;
	int y;
	int width, height;
	uint8_t *buffer;
	int bufferSize;
	int initialized;
	int devAddr;
} ssd1306_t;


typedef enum {
	ssd1306Black = 0x00,
	ssd1306White = 0x01,
} ssd1306Color_t;



typedef struct {
	const uint8_t width;
	const uint8_t height;
	const uint16_t *const data;
	const uint8_t *const size;
} ssd1306Font_t;


// #ifdef SSD1306_X_OFFSET
// #define SSD1306_X_OFFSET_LOWER (SSD1306_X_OFFSET & 0x0F)
// #define SSD1306_X_OFFSET_UPPER ((SSD1306_X_OFFSET >> 4) & 0x07)
// #else
// #endif
// #define SSD1306_X_OFFSET_LOWER 0
// #define SSD1306_X_OFFSET_UPPER 0

// ssd1306_t ssd1306Init(i2c_t *i2c, int devAddr);
ssd1306_t ssd1306Init(i2c_t *i2c, int devAddr, int widht, int height, uint8_t *buffer);

void ssd1306SetContrast(ssd1306_t *display, const uint8_t value);
void ssd1306Flush(ssd1306_t *display);
void ssd1306SetAll(ssd1306_t *display);
void ssd1306ClearAll(ssd1306_t *display);
void ssd1306DrawPixel(ssd1306_t *dispaly, uint8_t x, uint8_t y, ssd1306Color_t color);
void ssd1306SetCursor(ssd1306_t *display, uint8_t x, uint8_t y);
void ssd1306DrawLine(ssd1306_t *display, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, ssd1306Color_t color);
void ssd1306DrawRectangle(ssd1306_t *display, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, ssd1306Color_t color);
void ssd1306DrawFilledRectangle(ssd1306_t *dispaly, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, ssd1306Color_t color);
char ssd1306DrawChar(ssd1306_t *display, char character, ssd1306Font_t font, ssd1306Color_t color);
char ssd1306DrawString(ssd1306_t *display, char* str, ssd1306Font_t font, ssd1306Color_t color);


#endif
