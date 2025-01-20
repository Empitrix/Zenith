#include "main.h"
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
	TIMER_1 = TIM1_BASE,
	TIMER_2 = TIM2_BASE,
	TIMER_3 = TIM3_BASE,
	TIMER_4 = TIM4_BASE,
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
	uint32_t frequency;
} capture_t;


typedef struct {
	timer_callback_t callback;
	timerNumber_t timerNumber;
	time_t interval;
	TIM_HandleTypeDef htim;
	bool running;

	capture_t capture;
} timer_t;


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
	TIMER_CH_1 = TIM_CHANNEL_1,
	TIMER_CH_2 = TIM_CHANNEL_2,
	TIMER_CH_3 = TIM_CHANNEL_3,
	TIMER_CH_4 = TIM_CHANNEL_4,
	TIMER_CH_A = TIM_CHANNEL_ALL,
} timerChannel_t;


void timerStartDMA(timerNumber_t timerNumber, timerChannel_t channel, uint32_t *data, uint16_t length);
void timerStopDMA(timerNumber_t timerNumber, timerChannel_t channel);




//** CAPTURE **//

typedef enum {
	CAPTURE_RISING = TIM_INPUTCHANNELPOLARITY_RISING,
	CAPTURE_FALLIGN = TIM_INPUTCHANNELPOLARITY_FALLING,
	CAPTURE_BOTHEDGE = TIM_INPUTCHANNELPOLARITY_BOTHEDGE,
} capturePolarity_t;

void timerCaptureInit(timer_t *timer, timerChannel_t channel, capturePolarity_t polarity, timer_callback_t callback);


#define maxinterval 910

#endif
