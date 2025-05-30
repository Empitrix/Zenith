#include "analog.h"
#include "stm32f103xb.h"
#include "stm32f1xx_hal_adc.h"


ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;

// DMA
DMA_HandleTypeDef hdma_adc1;


ADC_HandleTypeDef MX_ADC1_Init(int channel){
	ADC_ChannelConfTypeDef sConfig = {0};

	hadc1.Instance = ADC1;
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.ContinuousConvMode = ENABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	if(HAL_ADC_Init(&hadc1) != HAL_OK){
		Error_Handler();
	}
	sConfig.Channel = channel;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	if(HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK){
		Error_Handler();
	}
	return hadc1;
}


ADC_HandleTypeDef MX_ADC2_Init(int channel){
	ADC_ChannelConfTypeDef sConfig = {0};

	hadc2.Instance = ADC2;
	hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc2.Init.ContinuousConvMode = ENABLE;
	hadc2.Init.DiscontinuousConvMode = DISABLE;
	hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc2.Init.NbrOfConversion = 1;
	if(HAL_ADC_Init(&hadc2) != HAL_OK){
		Error_Handler();
	}

	sConfig.Channel = channel;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	if(HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK){
		Error_Handler();
	}
	return hadc2;
}

static void MX_DMA_Init(void){

	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Channel1_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}


/* analogInit: Initialize ADC */
analog_t analogInit(analogPin_t pin){
	analog_t analog;

	analog.pin = pin;
	analog.value = 0;

	int aPin = GET_ANALOG_PIN(pin);
	int channel = aPin > 8 ? aPin - 8: aPin;


	switch (GET_ANALOG_INSTANCE(pin)){
		case ADC_1: analog.adc = MX_ADC1_Init(channel); break; // analog.instance = ADC1; break;
		case ADC_2: analog.adc = MX_ADC2_Init(channel); break; // analog.instance = ADC2; break;
		default: break;
	}

	HAL_ADC_Start(&analog.adc);

	return analog;
}


/* analogDMAInit: Initialize ADC (NON-BLOCKING) */
void analogDMAInit(analog_t *analog, analogPin_t pin, analogCallback_t callback){
	analog->pin = pin;
	analog->value = 0;

	int aPin = GET_ANALOG_PIN(pin);
	int channel = aPin > 8 ? aPin - 8: aPin;

	MX_DMA_Init();

	switch (GET_ANALOG_INSTANCE(pin)) {
		case ADC_1: analog->adc = MX_ADC1_Init(channel); break; // analog->instance = ADC1; break;
		case ADC_2: analog->adc = MX_ADC2_Init(channel); break; // analog->instance = ADC2; break;
		default: break;
	}

	HAL_ADC_Start_DMA(&analog->adc, &analog->value, 1);
}


/* analogRead: Read the value of the given adc */
uint32_t analogRead(analog_t *analog){
	HAL_ADC_PollForConversion(&analog->adc, 1000);
	return HAL_ADC_GetValue(&analog->adc);
}


/** CALLBACK **/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
}


