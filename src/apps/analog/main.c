#include "board.h"
#include "uart.h"
#include "analog.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include <stdio.h>


#define VOL_SCALE 0.319727891156

#define GAIN  0.9998888
#define OFFSET (+2.999)


static uint8_t displayBuffer[1024] = { 0 };


void callback(int x){
	// printf("Is it working? (%d)\n", _);
}


#define WINDOW_SIZE 10  // Number of samples for moving average

// Function to compute the moving average of ADC readings
float moving_average(uint16_t new_value) {
    static uint16_t window[WINDOW_SIZE] = {0};  // Circular buffer for ADC values
    static int index = 0;                       // Current index in the buffer
    static int count = 0;                       // Number of values added
    static uint32_t sum = 0;                    // Sum of window values

    // Remove the oldest value from the sum
    sum -= window[index];

    // Store the new ADC value
    window[index] = new_value;
    sum += new_value;

    // Update index (circularly)
    index = (index + 1) % WINDOW_SIZE;

    // Increase count until the buffer is full
    if (count < WINDOW_SIZE) {
        count++;
    }

    // Return the moving average
    return (float)sum / count;
}


/* oledShowValue: display the value on the OLED screen */
void oledShowValue(ssd1306_t *display, float num, int adc){
	int int_part = (int)num;
	int frac_part = (int)((num - int_part) * 100);
	if (frac_part < 0) frac_part = -frac_part;

	char valBuf[128] = { 0 };
	char adcBuf[128] = { 0 };

	sprintf(valBuf, "%d.%02dv", int_part, frac_part);
	sprintf(adcBuf, "%d", adc);
	ssd1306ClearAll(display);
	ssd1306SetCursor(display, 0, 0);
	ssd1306DrawString(display, valBuf, FONT_16X26, ssd1306White);

	ssd1306SetCursor(display, 0, 30);
	ssd1306DrawString(display, adcBuf, FONT_16X26, ssd1306White);
	ssd1306Flush(display);
}

int main(void){
	boardInit();
	uart_t uart = uartInit(UART3, 115200);
	uartSetSTDOUT(&uart);

	i2c_t i2c = i2cInit(I2C1_B6_B7, 1000000);
	ssd1306_t display = ssd1306Init(&i2c, SSD1306_I2C_DEV_ADDR, 128, 64, displayBuffer);

	analog_t volAnlg;
	analogDMAInit(&volAnlg, ADC1_A5, callback);

	while(1){
		float val = volAnlg.value;
		val *= GAIN;
		val += OFFSET;
		float average = moving_average(val);
		oledShowValue(&display, average / 380.1886394, average);
	}
	return 0;
}

