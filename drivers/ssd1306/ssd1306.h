#include "gpio.h"
#include "timer.h"
#include "i2c.h"
#include <stdint.h>

#ifndef __SSD1306_DRIVER__
#define __SSD1306_DRIVER__


#define SSD1306_WIDTH   128
#define SSD1306_HEIGHT  64


#define SCREEN_BUFFER_SIZE (SSD1306_WIDTH * SSD1306_HEIGHT) / 8

#define SSD1306_I2C_DEV_ADDR        (0x3C << 1)


typedef struct {
	i2c_t *conn;    // I2C connectivity
	int displayOn;  // Status of display (ON=1, OFF=0)
	int x;
	int y;
	int initialized;
} ssd1306_t;


// #ifdef SSD1306_X_OFFSET
// #define SSD1306_X_OFFSET_LOWER (SSD1306_X_OFFSET & 0x0F)
// #define SSD1306_X_OFFSET_UPPER ((SSD1306_X_OFFSET >> 4) & 0x07)
// #else
// #endif
// #define SSD1306_X_OFFSET_LOWER 0
// #define SSD1306_X_OFFSET_UPPER 0

ssd1306_t ssd1306Init(i2c_t *i2c);
void ssd1306SetContrast(ssd1306_t ssd, const uint8_t value);
void ssd1306Flush(ssd1306_t *ssd);
void ssd1306SetAll(void);
void ssd1306ClearAll(void);

#endif
