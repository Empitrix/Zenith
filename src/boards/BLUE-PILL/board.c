#include "board.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include <stdarg.h>
#include <stdio.h>
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include <string.h>



void SystemClock_Config(void){
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/** Initializes the RCC Oscillators according to the specified parameters
	* in the RCC_OscInitTypeDef structure.
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK){
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
															|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK){
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
	PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK){
		Error_Handler();
	}
}





// MX_GPIO_Init: GPIO Initialization Function
void MX_GPIO_Init(void){
	// GPIO Ports Clock Enable
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();


	//// Enable GPIO intera
	//HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
	//HAL_NVIC_EnableIRQ(EXTI0_IRQn);

	//HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
	//HAL_NVIC_EnableIRQ(EXTI1_IRQn);

	//HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
	//HAL_NVIC_EnableIRQ(EXTI2_IRQn);

	//HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
	//HAL_NVIC_EnableIRQ(EXTI3_IRQn);

	//HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
	//HAL_NVIC_EnableIRQ(EXTI4_IRQn);

	//HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
	//HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

	//HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	//HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}


// This function is executed in case of error occurrence.
void Error_Handler(void){
	__disable_irq();
	while(1){}
}


UART_HandleTypeDef huart3;

static void MX_USART3_UART_Init(void){
	huart3.Instance = USART3;
	huart3.Init.BaudRate = 115200;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart3) != HAL_OK){
		Error_Handler();
	}
}


/// ***************************** BOARD ***************************** ///

/* BoardInit: Initializes HAL, System Clock, GPIO pins */
void boardInit(void){
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
}


serial_t serialType = NO_SERIAL;


void serialBegin(serial_t type){
	if(type == USB_SERIAL){
		MX_USB_DEVICE_Init();
	} else {
		MX_USART3_UART_Init();
	}
	serialType = type;
}


void serialPrint(const char * frmt, ...){
	if(serialType == NO_SERIAL){ return; }

	char tmp[1024] = { 0 };
	// char *tmp = calloc(1024, sizeof(char));
	va_list arglist;
	va_start(arglist, frmt);
	vsprintf(tmp, frmt, arglist);
	va_end(arglist);

	if(serialType == UART_SERIAL){
		HAL_UART_Transmit(&huart3, (uint8_t *)tmp, strlen(tmp) * sizeof(uint8_t), 1000);
	} else {
		CDC_Transmit_FS((uint8_t *)tmp, strlen(tmp) * sizeof(uint8_t));
	}
}


/// ***************************** TIMER ***************************** ///

/* HAL_Delay */
void delayMs(uint32_t milliseconds){
	HAL_Delay(milliseconds);
}

// void delayUs(uint32_t us){
// 	__HAL_TIM_SET_COUNTER(&htim1,0);
// 	while (__HAL_TIM_GET_COUNTER(&htim1) < us);
// }

int boardGetTick(void){
	return (uint32_t)HAL_GetTick();
}



