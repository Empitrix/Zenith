#include "main.h"
#include <stdint.h>

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
typedef void (*callback_t)(timeHandle_t);


typedef struct {
	callback_t callback;
	timerNumber_t timerNumber;
	time_t interval;
	TIM_HandleTypeDef htim;
} timer_t;

timer_t timerInit(timerNumber_t timerNumber, time_t interval, callback_t callback, int start);


// void timerStart(timer_t *timer);
// void timerStop(timer_t *timer);
// void timerSetInterval(timer_t *timer, time_t interval);
// time_t timerGetInterval(const timer_t *timer);
// bool timerIsRunning(const timer_t *timer);
// void timerReset(timer_t *timer, int restart);
// void timerSetCallback(timer_t *timer, callback_t callback);
// void timerPause(timer_t *timer);
// void timerResume(timer_t *timer);
// time_t timerGetRemainingTime(const timer_t *timer);


#endif
