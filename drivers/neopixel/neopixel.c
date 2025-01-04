#include "neopixel.h"
#include "timer.h"



void NeopixelInit(void){
	uint16_t pwmData[384]; //16*24=384 (No of LEDs 16 and 24 Pulses for each LED)
	for (int i=0; i<384; i++) pwmData[i] = 2;
	timerStartDMA(TIMER_4, TIMER_CH_1, (uint32_t *)pwmData, 384);
	HAL_Delay (10);
	timerStopDMA(TIMER_4, TIMER_CH_1);
	HAL_Delay (500);
	for (int i=0; i<384; i++) pwmData[i] = 1;
	timerStartDMA(TIMER_4, TIMER_CH_1, (uint32_t *)pwmData, 384);
	HAL_Delay (10);
	timerStopDMA(TIMER_4, TIMER_CH_1);
	HAL_Delay (500);
}

