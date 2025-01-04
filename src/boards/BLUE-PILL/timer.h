#include "main.h"
#include "stm32f1xx_hal_tim.h"
#include <stdint.h>
#include <stdbool.h>

#ifndef __BOARD_TIMER__
#define __BOARD_TIMER__


#define MAX_TIMER_NUMBER 4

typedef enum {
	TIMER_1 = TIM1_BASE,
	TIMER_2 = TIM2_BASE,
	TIMER_3 = TIM3_BASE,
	TIMER_4 = TIM4_BASE,
} timerNumber_t;


typedef uint32_t time_t;

typedef TIM_HandleTypeDef timeHandle_t;
typedef void (*timer_callback_t)(timeHandle_t);


typedef struct {
	timer_callback_t callback;
	timerNumber_t timerNumber;
	time_t interval;
	TIM_HandleTypeDef htim;
	bool running;
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


#endif
