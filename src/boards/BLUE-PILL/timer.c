#include "timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>


// Global Timers
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
DMA_HandleTypeDef hdma_tim4_ch1;


static timer_callback_t timer_callbacks[MAX_TIMER_NUMBER] = { 0 };


static int get_timer_idx(timerNumber_t tn){
	switch (tn) {
		case TIMER_1: return 0;
		case TIMER_2: return 1;
		case TIMER_3: return 2;
		case TIMER_4: return 3;
	}
	return 1;
}

void save_callback(timerNumber_t tn, timer_callback_t clbk){
	int idx = get_timer_idx(tn);
	timer_callbacks[idx] = clbk;
}



TIM_HandleTypeDef MX_TIM1_Init(int prescaler, int period){
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = prescaler;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = period;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if(HAL_TIM_Base_Init(&htim1) != HAL_OK){
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if(HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK){
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if(HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK){
		Error_Handler();
	}
	return htim1;
}


TIM_HandleTypeDef MX_TIM2_Init(int prescaler, int period){
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = prescaler;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = period;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if(HAL_TIM_Base_Init(&htim2) != HAL_OK){
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if(HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK){
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if(HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK){
		Error_Handler();
	}
	return htim2;
}

TIM_HandleTypeDef MX_TIM3_Init(int prescaler, int period){
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = prescaler;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = period;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	// htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	return htim3;
}

TIM_HandleTypeDef MX_TIM4_Init(int prescaler, int period){
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	htim4.Instance = TIM4;
	htim4.Init.Prescaler = prescaler;
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim4.Init.Period = period;
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	return htim4;
}


static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}



// TIM_HandleTypeDef MX_TIM4_Init(int prescaler, int period){
// 
//   /* USER CODE BEGIN TIM4_Init 0 */
// 
//   /* USER CODE END TIM4_Init 0 */
// 
//   TIM_ClockConfigTypeDef sClockSourceConfig = {0};
//   TIM_MasterConfigTypeDef sMasterConfig = {0};
//   TIM_OC_InitTypeDef sConfigOC = {0};
// 
//   /* USER CODE BEGIN TIM4_Init 1 */
// 
//   /* USER CODE END TIM4_Init 1 */
//   htim4.Instance = TIM4;
//   htim4.Init.Prescaler = prescaler;
//   htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
//   htim4.Init.Period = period;
//   htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//   htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
//   if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
//   if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
//   sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//   if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   sConfigOC.OCMode = TIM_OCMODE_PWM1;
//   sConfigOC.Pulse = 0;
//   sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
//   sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
//   if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /* USER CODE BEGIN TIM4_Init 2 */
// 
//   /* USER CODE END TIM4_Init 2 */
//   HAL_TIM_MspPostInit(&htim4);
// 	return htim4;
// }


typedef struct {} interval_t;


void calc_interval(int interval_us, int *period, int *prescaler){
	int ticks = (interval_us * 72000000) / 1000000;
	*prescaler = (ticks / 65536);
	*period = (ticks / (*prescaler + 1)) - 1;
}



timer_t timerConfigure(timerNumber_t timerNumber, int prescaler, int period, timer_callback_t callback, int start){
	timer_t timer = { 0 };
	timer.capture = (capture_t){ 0 };

	timer.interval = period;
	timer.timerNumber = timerNumber;
	timer.callback = callback;

	save_callback(timer.timerNumber, callback);

	MX_DMA_Init();


	switch (timer.timerNumber) {
		case TIMER_1: timer.htim = MX_TIM1_Init(prescaler, period); break;
		case TIMER_2: timer.htim = MX_TIM2_Init(prescaler, period); break;
		case TIMER_3: timer.htim = MX_TIM3_Init(prescaler, period); break;
		case TIMER_4: timer.htim = MX_TIM4_Init(prescaler, period); break;
	}

	timer.running = 0;

	if(start){
		HAL_TIM_Base_Start_IT(&timer.htim);
		timer.running = 1;
	}
	
	return timer;
}


timer_t timerInit(timerNumber_t timerNumber, time_t interval_us, timer_callback_t callback, int start){
	int pre = 0, pri = 0;
	calc_interval(interval_us, &pri, &pre);
	return timerConfigure(timerNumber, pre, pri, callback, start);
}

/*
timer_t timerInit(timerNumber_t timerNumber, time_t interval_us, timer_callback_t callback, int start){
	timer_t timer = { 0 };
	timer.capture = (capture_t){ 0 };

	timer.interval = interval_us;
	timer.timerNumber = timerNumber;
	timer.callback = callback;

	save_callback(timer.timerNumber, callback);

	MX_DMA_Init();


	int init_prescaler = 0;
	int init_period = 0;
	calc_interval(interval_us, &init_period, &init_prescaler);


	switch (timer.timerNumber) {
		case TIMER_1: break;
		case TIMER_2: timer.htim = MX_TIM2_Init(init_prescaler, init_period); break;
		case TIMER_3: timer.htim = MX_TIM3_Init(init_prescaler, init_period); break;
		case TIMER_4: timer.htim = MX_TIM4_Init(init_prescaler, init_period); break;
	}

	timer.running = 0;

	if(start){
		HAL_TIM_Base_Start_IT(&timer.htim);
		timer.running = 1;
	}
	
	return timer;
}
*/


/* timerStart: start timer if not running */
void timerStart(timer_t *timer){
	if(timer->running == 0){
		HAL_TIM_Base_Start_IT(&timer->htim);
		timer->running = 1;
	}
}

/* timerStop: stop timer if running */
void timerStop(timer_t *timer){
	if(timer->running){
		HAL_TIM_Base_Stop_IT(&timer->htim);
		timer->running = 0;
	}
}



void setTimerAutoRelease(timer_t *timr, int enable){
	timerStop(timr);
	htim3.Init.AutoReloadPreload = enable ? TIM_AUTORELOAD_PRELOAD_ENABLE : TIM_AUTORELOAD_PRELOAD_DISABLE;
	timerStart(timr);
}


timerNumber_t *idx2timer(int idx){
	switch (idx) {
		case 0:  return (timerNumber_t*)TIM1;
		case 1:  return (timerNumber_t*)TIM2;
		case 2:  return (timerNumber_t*)TIM3;
		case 3:  return (timerNumber_t*)TIM4;
		default: return (timerNumber_t*)TIM2;
	}
}


/* timerSetInterval: update interval */
void timerSetInterval(timer_t *timer, time_t interval){
	int period = 0;
	int prescaler = 0;

	calc_interval(interval, &period, &prescaler);

	switch (timer->timerNumber) {
		case TIMER_1: break;
		case TIMER_2: timer->htim = MX_TIM2_Init(prescaler, period); break;
		case TIMER_3: timer->htim = MX_TIM3_Init(prescaler, period); break;
		case TIMER_4: timer->htim = MX_TIM4_Init(prescaler, period); break;
	}

	HAL_TIM_Base_Start_IT(&timer->htim);
}



time_t timerGetInterval(const timer_t *timer){
	return (time_t)timer->interval / 10;
}


bool timerIsRunning(const timer_t *timer){
	return timer->running;
}

void timerSetCallback(timer_t *timer, timer_callback_t callback){
	save_callback(timer->timerNumber, callback);
}


void timerReset(timer_t *timer, int restart){
	((TIM_TypeDef *)timer->timerNumber)->CNT = 0;
}


void timerPause(timer_t *timer){
	if(timer->running == 0){ return; }
	switch (timer->timerNumber) {
		case TIMER_1:
			__HAL_RCC_TIM1_CLK_DISABLE();
			break;
		case TIMER_2:
			__HAL_RCC_TIM2_CLK_DISABLE();
			break;
		case TIMER_3:
			__HAL_RCC_TIM3_CLK_DISABLE();
			break;
		case TIMER_4:
			__HAL_RCC_TIM4_CLK_DISABLE();
			break;
		default: break;
	}
	timer->running = 0;
}

void timerResume(timer_t *timer){
	if(timer->running){ return; }
	switch (timer->timerNumber) {
		case TIMER_1:
			__HAL_RCC_TIM1_CLK_ENABLE();
			break;
		case TIMER_2:
			__HAL_RCC_TIM2_CLK_ENABLE();
			break;
		case TIMER_3:
			__HAL_RCC_TIM3_CLK_ENABLE();
			break;
		case TIMER_4:
			__HAL_RCC_TIM4_CLK_ENABLE();
			break;
		default: break;
	}
	timer->running = 1;
}


time_t timerGetRemainingTime(const timer_t *timer){
	// int interval = ((timer->interval) * (7200-1)) - __HAL_TIM_GetCounter(&timer->htim);
	int interval = (timer->interval * ((int)HAL_GetTickFreq())) - __HAL_TIM_GetCounter(&timer->htim);
	return interval;
}


void timerStartDMA(timerNumber_t timerNumber, timerChannel_t channel, uint32_t *data, uint16_t length){
	switch (timerNumber){
		case TIMER_1:
			break;
		case TIMER_2:
			HAL_TIM_PWM_Start_DMA(&htim2, channel, data, length);
			break;
		case TIMER_3:
			HAL_TIM_PWM_Start_DMA(&htim3, channel, data, length);
			break;
		case TIMER_4:
			HAL_TIM_PWM_Start_DMA(&htim4, channel, data, length);
			break;
		default: break;
	}
}

void timerStopDMA(timerNumber_t timerNumber, timerChannel_t channel){
	switch (timerNumber){
		case TIMER_1:
			break;
		case TIMER_2:
			HAL_TIM_PWM_Stop_DMA(&htim2, channel);
			break;
		case TIMER_3:
			HAL_TIM_PWM_Stop_DMA(&htim3, channel);
			break;
		case TIMER_4:
			HAL_TIM_PWM_Stop_DMA(&htim4, channel);
			break;
		default: break;
	}
}



static timer_callback_t capture_callbacks[MAX_TIMER_CHANNEL_IRQ] = { 0 };
static capture_t *capture_timers[MAX_TIMER_CHANNEL_IRQ] = { 0 };


void timerCaptureInit(timer_t *timer, tiemrCaptureConfig_t config, capturePolarity_t polarity, int tickTime, timer_callback_t callback){
	TIM_IC_InitTypeDef sConfigIC = {0};
	sConfigIC.ICPolarity = polarity;
	sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
	sConfigIC.ICFilter = 0;



	int timerNum = 0;
	int channelNum = 0;

	
	GPIO_PINS pin = (GPIO_PINS)(config >> 16);
	timerNumber_t tnum = (timerNumber_t)((config >> 8) & 0xFF);
	timerChannel_t channel = (timerChannel_t)(config & 0xFF);


	int prescaler = (tickTime * 72000000) / 1000000;
	*timer = timerConfigure(tnum, prescaler, 65535, NULL, 1);
	gpinInit(pin, GPIO_INPUT_MODE, GPIN_NO_PULL);

	// Set to Auto-Reload
	// setTimerAutoRelease(t, 1);

	switch(timer->timerNumber){
		case TIMER_1:
			HAL_TIM_Base_Start_IT(&htim1);
			HAL_TIM_IC_Start_IT(&htim1, channel);
			timerNum = 0;
			break;
		case TIMER_2:
			if(HAL_TIM_IC_Init(&htim2) != HAL_OK){ Error_Handler(); }
			HAL_TIM_Base_Start_IT(&htim2);
			HAL_TIM_IC_Start_IT(&htim2, channel);
			if(HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, channel) != HAL_OK){
				Error_Handler();
			}
			timerNum = 1;
			break;
		case TIMER_3:
			if(HAL_TIM_IC_Init(&htim3) != HAL_OK){ Error_Handler(); }
			HAL_TIM_Base_Start_IT(&htim3);
			HAL_TIM_IC_Start_IT(&htim3, channel);
			if(HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, channel) != HAL_OK){
				Error_Handler();
			}
			timerNum = 2;
			break;
		case TIMER_4:
			if(HAL_TIM_IC_Init(&htim4) != HAL_OK){ Error_Handler(); }
			HAL_TIM_Base_Start_IT(&htim4);
			HAL_TIM_IC_Start_IT(&htim4, channel);
			if(HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, channel) != HAL_OK){
				Error_Handler();
			}
			timerNum = 3;
			break;
		default: break;
	}
	
	switch(channel){
		case CH_1: channelNum = 0; break;
		case CH_2: channelNum = 1; break;
		case CH_3: channelNum = 2; break;
		case CH_4: channelNum = 3; break;
		default: break;
	}

	int idx = (timerNum * MAX_TIMER_NUMBER) + channelNum;
	capture_callbacks[idx] = callback;
	capture_timers[idx] = &timer->capture;
}



//****** HAL callback's ******//

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	for(int i = 0; i < MAX_TIMER_NUMBER; i++){
		if(htim->Instance == (TIM_TypeDef *)idx2timer(i) && timer_callbacks[i] != NULL){
			timer_callbacks[i]((timeHandle_t)*htim);
		}
	}

	for(int i = 0; i < MAX_TIMER_CHANNEL_IRQ; i++){
		if(capture_timers[i] != NULL){
			capture_timers[i]->tim_ovc++;
		}
	}
}


static int previusTicks = 0;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim){
	int timerNum = 0;
	int channelNum = 0;

	if(htim->Instance == TIM1){
		timerNum = 0;
	} else if(htim->Instance == TIM2){
		timerNum = 1;
	} else if(htim->Instance == TIM3){
		timerNum = 2;
	} else{
		timerNum = 3;
	}


	int ccr = 0;
	switch (htim->Channel) {
		case HAL_TIM_ACTIVE_CHANNEL_1:
			channelNum = 0;
			ccr = htim->Instance->CCR1;
			break;
		case HAL_TIM_ACTIVE_CHANNEL_2:
			channelNum = 1;
			ccr = htim->Instance->CCR2;
			break;
		case HAL_TIM_ACTIVE_CHANNEL_3:
			channelNum = 2;
			ccr = htim->Instance->CCR3;
			break;
		case HAL_TIM_ACTIVE_CHANNEL_4:
			channelNum = 3;
			ccr = htim->Instance->CCR4;
			break;
		default: break;
	}


	int idx = (timerNum * MAX_TIMER_NUMBER) + channelNum;
	capture_callbacks[idx](*htim);

	capture_timers[idx]->ccr = ccr - previusTicks;
	previusTicks = ccr;
}

// void delayUs(uint16_t us){
// 	__HAL_TIM_SET_COUNTER(&htim1, 0);
// 	while(__HAL_TIM_GET_COUNTER(&htim1) < us){}
// }
