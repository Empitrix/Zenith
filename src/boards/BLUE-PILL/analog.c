#include "analog.h"


ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;


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
	hadc2.Init.ContinuousConvMode = DISABLE;
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



/* analogInit: Initialize ADC */
analog_t analogInit(analogPin_t pin){
	analog_t analog;

	analog.pin = pin;
	analog.value = 0;

	int aPin = GET_ANALOG_PIN(pin);
	int channel = aPin > 8 ? aPin - 8: aPin;

	switch (GET_ANALOG_INSTANCE(pin)) {
		case ADC_1: analog.adc = MX_ADC1_Init(channel); break;
		case ADC_2: analog.adc = MX_ADC2_Init(channel); break;
		default: break;
	}

	HAL_ADC_Start(&analog.adc);

	return analog;
}


/* analogInit: Initialize ADC */
analog_t analogDMAInit(analogPin_t pin, analogDMACallback callback){
	analog_t analog;

	analog.pin = pin;
	analog.value = 0;

	int aPin = GET_ANALOG_PIN(pin);
	int channel = aPin > 8 ? aPin - 8: aPin;

	switch (GET_ANALOG_INSTANCE(pin)) {
		case ADC_1: analog.adc = MX_ADC1_Init(channel); break;
		case ADC_2: analog.adc = MX_ADC2_Init(channel); break;
		default: break;
	}

	HAL_ADC_Start(&analog.adc);

	return analog;
}


/* analogRead: Read the value of the given adc */
uint32_t analogRead(analog_t *analog){
	HAL_ADC_PollForConversion(&analog->adc, 1000);
	return HAL_ADC_GetValue(&analog->adc);
}

