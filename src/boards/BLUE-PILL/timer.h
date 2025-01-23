#include "main.h"
#include "gpio.h"
#include "stm32f1xx_hal_tim.h"
#include <stdint.h>
#include <stdbool.h>

#ifndef __BOARD_TIMER__
#define __BOARD_TIMER__


#define MAX_TIMER_NUMBER 4

#define TIMER_CLOCK 72000000
#define S(interval) (interval * 1000000)
#define MS(interval) (interval * 1000)
#define US(interval) (interval)

#define MAX_TIMER_NUMBER 4
#define MAX_CHANNEL_NUMBER 4

#define MAX_TIMER_CHANNEL_IRQ (MAX_TIMER_NUMBER * MAX_CHANNEL_NUMBER)


typedef enum {
	TIMER_1,
	TIMER_2,
	TIMER_3,
	TIMER_4,
} timerNumber_t;


typedef uint32_t time_t;

typedef TIM_HandleTypeDef timeHandle_t;
typedef void (*timer_callback_t)(timeHandle_t);


#define F_CLK  72000000UL
#define CAPTURE_FREQUENCY 910


typedef struct {
	uint8_t state;
	uint32_t t1;
	uint32_t t2;
	uint32_t ticks;
	uint16_t tim_ovc;
	int frequency;

	int ccr;
} capture_t;


typedef struct {
	timer_callback_t callback;
	timerNumber_t timerNumber;
	time_t interval;
	TIM_HandleTypeDef htim;
	bool running;

	capture_t capture;
} timer_t;


timer_t timerConfigure(timerNumber_t timerNumber, int prescaler, int period, timer_callback_t callback, int start);
timer_t timerInit(timerNumber_t timerNumber, time_t interval, timer_callback_t callback, int start);
void timerStart(timer_t *timer);
void timerStop(timer_t *timer);

void timerSetInterval(timer_t *timer, time_t interval);
time_t timerGetInterval(const timer_t *timer);
bool timerIsRunning(const timer_t *timer);
void timerSetCallback(timer_t *timer, timer_callback_t callback);
void timerReset(timer_t *timer, int restart);

void timerPause(timer_t *timer);
void timerResume(timer_t *timer);

time_t timerGetRemainingTime(const timer_t *timer);


typedef enum {
	CH_1 = TIM_CHANNEL_1,
	CH_2 = TIM_CHANNEL_2,
	CH_3 = TIM_CHANNEL_3,
	CH_4 = TIM_CHANNEL_4,
	CH_A = TIM_CHANNEL_ALL,
} timerChannel_t;


void timerStartDMA(timerNumber_t timerNumber, timerChannel_t channel, uint32_t *data, uint16_t length);
void timerStopDMA(timerNumber_t timerNumber, timerChannel_t channel);




//** CAPTURE **//

typedef enum {
	CAPTURE_RISING = TIM_INPUTCHANNELPOLARITY_RISING,
	CAPTURE_FALLIGN = TIM_INPUTCHANNELPOLARITY_FALLING,
	CAPTURE_BOTHEDGE = TIM_INPUTCHANNELPOLARITY_BOTHEDGE,
} capturePolarity_t;




typedef enum {
	B9_TIM4_CH4  =  (B_9 << 16)  | (TIMER_4 << 8) | CH_4,
	B8_TIM4_CH3  =  ( B_8 << 16) | (TIMER_4 << 8) | CH_3,
	B7_TIM4_CH2  =  ( B_7 << 16) | (TIMER_4 << 8) | CH_2,
	B6_TIM4_CH1  =  ( B_6 << 16) | (TIMER_4 << 8) | CH_1,
	B1_TIM3_CH4  =  ( B_1 << 16) | (TIMER_3 << 8) | CH_4,
	B0_TIM3_CH3  =  ( B_0 << 16) | (TIMER_3 << 8) | CH_3,

	B5_TIM3_CH2  =  ( A_7 << 16) | (TIMER_3 << 8) | CH_2,
	// A7_TIM3_CH2  =  ( A_7 << 16) | (TIMER_3 << 8) | CH_2,

	A6_TIM3_CH1  =  ( A_6 << 16) | (TIMER_3 << 8) | CH_1,
	A3_TIM2_CH4  =  ( A_3 << 16) | (TIMER_2 << 8) | CH_4,
	A2_TIM2_CH3  =  ( A_2 << 16) | (TIMER_2 << 8) | CH_3,
	A1_TIM2_CH2  =  ( A_1 << 16) | (TIMER_2 << 8) | CH_2,
	A0_TIM2_CH1  =  ( A_0 << 16) | (TIMER_2 << 8) | CH_1,
	A11_TIM1_CH4 =  (A_11 << 16) | (TIMER_1 << 8) | CH_4,
	A10_TIM1_CH3 =  (A_10 << 16) | (TIMER_1 << 8) | CH_3,
	A9_TIM1_CH2  =  ( A_9 << 16) | (TIMER_1 << 8) | CH_2,
	A8_TIM1_CH1  =  ( A_8 << 16) | (TIMER_1 << 8) | CH_1,
} tiemrCaptureConfig_t;


// void timerCaptureInit(timer_t *timer, timerChannel_t channel, capturePolarity_t polarity, timer_callback_t callback);
// void timerCaptureInit(tiemrCaptureConfig_t config, capturePolarity_t polarity, timer_callback_t callback);
// void timerCaptureInit(timer_t *timer, tiemrCaptureConfig_t config, capturePolarity_t polarity, timer_callback_t callback);

void timerCaptureInit(timer_t *timer, tiemrCaptureConfig_t config, capturePolarity_t polarity, int tickTime, timer_callback_t callback);


#define maxinterval 910

#endif
