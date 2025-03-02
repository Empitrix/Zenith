#include <board.h>
#include <ssd1306.h>
#include <ssd1306_fonts.h>
#include <analog.h>
#include <stdio.h>
#include <uart.h>

uint8_t displayBuffer[(128 * 64) / 8] = { 0 };


// #define VOLT_GAIN  0.9535
#define VOLT_GAIN  0.962
#define VOLT_OFFSET (+1.92)

#define AMP_GAIN  0.81
#define AMP_OFFSET (+0.57)


#define VOLT_WINDOW_SIZE 20
#define AMP_WINDOW_SIZE 50


float voltage_moving_average(uint16_t new_value) {
	static uint16_t window[VOLT_WINDOW_SIZE] = {0};  // Circular buffer for ADC values
	static int index = 0;                       // Current index in the buffer
	static int count = 0;                       // Number of values added
	static uint32_t sum = 0;                    // Sum of window values
	sum -= window[index];                       // Remove the oldest value from the sum
	// Store the new ADC value
	window[index] = new_value;
	sum += new_value;
	index = (index + 1) % VOLT_WINDOW_SIZE;          // Update index (circularly)
	if (count < VOLT_WINDOW_SIZE) { count++; }       // Increase count until the buffer is full
	return (float)sum / count;                  // Return the moving average
}


float current_moving_average(uint16_t new_value) {
	static uint16_t window[AMP_WINDOW_SIZE] = {0};  // Circular buffer for ADC values
	static int index = 0;                       // Current index in the buffer
	static int count = 0;                       // Number of values added
	static uint32_t sum = 0;                    // Sum of window values
	sum -= window[index];                       // Remove the oldest value from the sum
	// Store the new ADC value
	window[index] = new_value;
	sum += new_value;
	index = (index + 1) % AMP_WINDOW_SIZE;          // Update index (circularly)
	if (count < AMP_WINDOW_SIZE) { count++; }       // Increase count until the buffer is full
	return (float)sum / count;                  // Return the moving average
}



void floatStr(char *buffer, float value, char end){
	int int_part = (int)value;
	int frac_part = (int)((value - int_part) * 100);
	if (frac_part < 0) frac_part = -frac_part;
	sprintf(buffer, "%d.%02d%c", int_part, frac_part, end);
}

gpio_t relay;

static analog_t ampAnlg;
static float current;

static int initDelaySec = 0;

timer_t initDelayTim;

static int relayState = 0;

void initDelay(){
	if(initDelaySec == 5){
		timerStop(&initDelayTim);
		gpinSet(&relay, PIN_ON);
		relayState = 1;
		initDelaySec++;
	} else {
		initDelaySec++;
	}
}

void calcCurrent(){
	int val = analogRead(&ampAnlg);
	int readValue = current_moving_average(val);
	float rawVoltage = (readValue * 3.3) / 4095;
	current = (rawVoltage - 2.5) / 0.185;
	current *= AMP_GAIN;
	current -= AMP_OFFSET;
}

void clbk(int _){}

gpio_t btnA;
gpio_t btnB;


// Button A callback (self calibration)
void btnACallback(void *ctx){
}

// Button B callback (relay toggle)
void btnBCallback(void *ctx){
	relayState = relayState == 0;
	gpinToggle(&relay);
}


int main(void){
	boardInit();

	// initializing UART
	uart_t uart = uartInit(UART3, 115200);
	uartSetSTDOUT(&uart);

	// OLED (I2C) SKL: B6, SDA: B7
	i2c_t i2c = i2cInit(I2C1_B6_B7, 1000000);
	ssd1306_t display = ssd1306Init(&i2c, SSD1306_I2C_DEV_ADDR, 128, 64, displayBuffer);

	// Relay pin
	relay = gpinInit(A_0, GPIO_OUTPUT_MODE, GPIN_PULL_DOWN);

	// Button A (Internal PULL-UP, Falling edge)
	btnA = gpinInit(A_2, GPIO_INPUT_MODE, GPIN_PULL_UP);
	gpinSetInterrupt(&btnA, IRQ_FALLING, IRQ_VERY_HIGH_PRIORITY, &btnACallback, NULL);

	// Button B (Internal PULL-UP, Falling edge)
	btnB = gpinInit(A_1, GPIO_INPUT_MODE, GPIN_PULL_UP);
	gpinSetInterrupt(&btnB, IRQ_FALLING, IRQ_VERY_HIGH_PRIORITY, &btnBCallback, NULL);

	analog_t volAnlg;
	analogDMAInit(&volAnlg, ADC1_A5, clbk);
	ampAnlg = analogInit(ADC2_B1);
	timerInit(TIMER_2, MS(400), calcCurrent, 1);
	initDelayTim = timerInit(TIMER_3, S(1), initDelay, 1);

	while(1){
		char ampBuff[10] = { 0 };
		floatStr(ampBuff, current, 'A');
		// sprintf(ampBuff, "%d", analogRead(&ampAnlg));

		float val = volAnlg.value;
		val *= VOLT_GAIN;
		val += VOLT_OFFSET;
		float average = voltage_moving_average(val);
		char voltBuff[10] = { 0 };

		floatStr(voltBuff, average * 0.00540935672515, 'v');

		char *relayBuff = relayState ? " ON" : "OFF";
		ssd1306ClearAll(&display);

		ssd1306SetCursor(&display, 20, 5);
		ssd1306DrawString(&display, voltBuff, FONT_16X26, ssd1306White);

		ssd1306SetCursor(&display, 20, 38);
		ssd1306DrawString(&display, ampBuff, FONT_16X26, ssd1306White);

		if(initDelaySec != 6){
			char initDelayBuff[10];
			sprintf(initDelayBuff, "%d", initDelaySec);
			ssd1306SetCursor(&display, 115, 0);
			ssd1306DrawString(&display, initDelayBuff, FONT_6X8, ssd1306White);
		}

		ssd1306SetCursor(&display, 110, 54);
		ssd1306DrawString(&display, relayBuff, FONT_6X8, ssd1306White);

		ssd1306Flush(&display);
	}

	return 0;
}
