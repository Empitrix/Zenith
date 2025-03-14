#include "board.h"
#include "gpio.h"
#include "uart.h"
#include "timer.h"
#include "neopixel.h"

#define MAX_BUF_SIZE 672


static int irq_flag = 0;
static int rpm = 0;
static int totalDegree = 0;

static int array[15] = { 0 };

static int adjustDelay = 0;


static timer_t timer;
static const int ticks = 13;





#define AVRG_WINDOW_SIZE 20

int moving_average_total(uint16_t new_value) {
	static uint16_t window[AVRG_WINDOW_SIZE] = {0};  // Circular buffer for ADC values
	static int index = 0;                            // Current index in the buffer
	static int count = 0;                            // Number of values added
	static uint32_t sum = 0;                         // Sum of window values
	sum -= window[index];                            // Remove the oldest value from the sum
	window[index] = new_value;
	sum += new_value;
	index = (index + 1) % AVRG_WINDOW_SIZE;          // Update index (circularly)
	if (count < AVRG_WINDOW_SIZE) { count++; }       // Increase count until the buffer is full
	return (int)sum / count;                         // Return the moving average
}






void calbk(timeHandle_t htim){
	if(timer.capture.ccr < 0){ return; }
	irq_flag = 1;
	rpm = 60 * (1 / ((timer.capture.ccr * ticks) * 0.000001));
	if(rpm > 2000){ rpm = 1617; }
	// if(rpm < 0){ return; }

	totalDegree = (rpm / 60) * 360;
	totalDegree = moving_average_total(totalDegree);

	array[0 ] = (((17.60 / totalDegree) / 16) * 1000000) + 12; 
	array[1 ] = (((18.21 / totalDegree) / 16) * 1000000) + 3;
	array[2 ] = (((18.87 / totalDegree) / 16) * 1000000) + 3;
	array[3 ] = (((19.57 / totalDegree) / 16) * 1000000) + 3;
	array[4 ] = (((20.33 / totalDegree) / 16) * 1000000) + 3;
	array[5 ] = (((21.15 / totalDegree) / 16) * 1000000) + 3;
	array[6 ] = (((22.04 / totalDegree) / 16) * 1000000) + 2;
	array[7 ] = (((23.01 / totalDegree) / 16) * 1000000) + 1;
	array[8 ] = (((24.07 / totalDegree) / 16) * 1000000) + 0;
	array[9 ] = (((25.23 / totalDegree) / 16) * 1000000) - 1 - 1;
	array[10] = (((26.51 / totalDegree) / 16) * 1000000) - 0 - 2;
	array[11] = (((27.93 / totalDegree) / 16) * 1000000) - 1 - 3;
	array[12] = (((29.51 / totalDegree) / 16) * 1000000) - 2 - 2;
	array[13] = (((31.29 / totalDegree) / 16) * 1000000) - 3 - 1;

	int pA = (34.60 / totalDegree) * 1000000;
	int pB = (17.60 / totalDegree) * 1000000;
	adjustDelay = pA - (pB / 2) - 690;
}


gpio_t rLED;
gpio_t gLED;
gpio_t bLED;


typedef struct {
	gpio_t red;
	gpio_t green;
	gpio_t blue;
} octaDisplay_t;


octaDisplay_t display;

void setColor(int red, int green, int blue){
	gpinSet(&display.red, red ? PIN_ON : PIN_OFF);
	gpinSet(&display.green, green ? PIN_ON : PIN_OFF);
	gpinSet(&display.blue, blue ? PIN_ON : PIN_OFF);
}

void allColorOff(){
	gpinSet(&display.red, PIN_OFF);
	gpinSet(&display.green, PIN_OFF);
	gpinSet(&display.blue, PIN_OFF);
}

static int displayBuffer[MAX_BUF_SIZE] =
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1
, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 
0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1,
 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0
, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 
1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


int main(void){
	boardInit();
	uart_t uart = uartInit(UART3, 115200);
	uartSetSTDOUT(&uart);

	timerCaptureInit(&timer, B5_TIM3_CH2, CAPTURE_FALLIGN, ticks, calbk);


	gpio_t pinOut = gpinInit(B_4, GPIO_OUTPUT_MODE, GPIN_NO_PULL);
	gpinSet(&pinOut, PIN_ON);

	display.blue = gpinInit(B_12, GPIO_OUTPUT_MODE, GPIN_PUSH_PULL);
	display.green = gpinInit(B_13, GPIO_OUTPUT_MODE, GPIN_PUSH_PULL);
	display.red = gpinInit(B_14, GPIO_OUTPUT_MODE, GPIN_PUSH_PULL);


	int j = 0;

	// for(int i = 0; i < MAX_BUF_SIZE; i++){ displayBuffer[i] = 0; }

	while(1){
		while(irq_flag == 0){ __asm__ volatile ("nop"); }

		delayUs(adjustDelay);

		j = 0;
		for(int i = 0; i < 14; i++){
			int delayVal = array[i] - 12;
			for (int k = 0; k < 48; k += 3) {
				setColor(displayBuffer[j + k], displayBuffer[j + k + 1], displayBuffer[j + k + 2]);
				delayUs(delayVal);
			}

			setColor(0, 0, 0);  // None
			j += 48;
		}

		irq_flag = 0;
	}

	return 0;
}

	// RPM:
	// rpm = 60 * (1 / ((timer.capture.ccr * ticks) * 0.000001));
	// printf("CCR: %d, RPM: %d\n", timer.capture.ccr, rpm);
	// delayMs(100);
