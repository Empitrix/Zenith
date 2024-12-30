#include "timer.h"


// Global Timers
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;


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




TIM_HandleTypeDef MX_TIM2_Init(int period){
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 7200-1;
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

TIM_HandleTypeDef MX_TIM3_Init(int period){
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 7200-1;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = period;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
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

TIM_HandleTypeDef MX_TIM4_Init(int period){
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	htim4.Instance = TIM4;
	htim4.Init.Prescaler = 7200-1;
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

timer_t timerInit(timerNumber_t timerNumber, time_t interval, timer_callback_t callback, int start){
	timer_t timer = { 0 };

	timer.interval = (int)interval * 10;
	timer.timerNumber = timerNumber;
	timer.callback = callback;

	save_callback(timer.timerNumber, callback);


	switch (timer.timerNumber) {
		case TIMER_1: break;
		case TIMER_2: timer.htim = MX_TIM2_Init(timer.interval); break;
		case TIMER_3: timer.htim = MX_TIM3_Init(timer.interval); break;
		case TIMER_4: timer.htim = MX_TIM4_Init(timer.interval); break;
	}

	if(start){
		HAL_TIM_Base_Start_IT(&timer.htim);
	}
	
	return timer;
}


timerNumber_t idx2timer(int idx){
	switch (idx) {
		case 0: return TIMER_1;
		case 1: return TIMER_2;
		case 2: return TIMER_3;
		case 3: return TIMER_4;
		default: return TIMER_2;
	}
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	for(int i = 0; i < MAX_TIMER_NUMBER; i++){
		if(htim->Instance == (TIM_TypeDef *)idx2timer(i) && timer_callbacks[i] != NULL){
			timer_callbacks[i]((timeHandle_t)*htim);
		}
	}
}
