#include "board.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"


void SystemClock_Config(void){
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

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

	// Initializes the CPU, AHB and APB buses clocks
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK){
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
}


// This function is executed in case of error occurrence.
void Error_Handler(void){
	__disable_irq();
	while(1){}
}


// /// ***************************** BOARD ***************************** ///
// 
// /* BoardInit: Initializes HAL, System Clock, GPIO pins */
// void BoardInit(void){
// 	HAL_Init();
// 	SystemClock_Config();
// 	MX_GPIO_Init();
// }
// 
// 
// void GPIO_Init(GPIO_PINS pin, GPIO_TYPE type, GPIO_MODES mode){
// 	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
// 	GPIO_InitStruct.Pin = pin;
// 	GPIO_InitStruct.Mode = mode;
// 	GPIO_InitStruct.Pull = mode == GPIO_INPUT_MODE ? GPIO_PULLDOWN : GPIO_PULLUP;
// 	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
// 	HAL_GPIO_Init((GPIO_TypeDef *)type, &GPIO_InitStruct); 
// }
// 
// 
// void GPIO_Set(GPIO_PINS pin, GPIO_TYPE type, GPIO_STATE state){
// 	HAL_GPIO_WritePin((GPIO_TypeDef *)type, pin, (GPIO_PinState)state);
// }
// 
// 
// void GPIO_Toggle(GPIO_PINS pin, GPIO_TYPE type){
// 	HAL_GPIO_TogglePin((GPIO_TypeDef *)type, pin);
// }
// 
// 
// GPIO_STATE GPIO_Read(GPIO_PINS pin, GPIO_TYPE type){
// 	return (GPIO_STATE)HAL_GPIO_ReadPin((GPIO_TypeDef *)type, pin);
// }
// 
// 
// GPIO_Lock_Status GPIO_Lock(GPIO_PINS pin, GPIO_TYPE type){
// 	return (GPIO_Lock_Status)HAL_GPIO_LockPin((GPIO_TypeDef *)type, pin);
// }
// 
// 
// /// ***************************** TIMER ***************************** ///
// 
// /* HAL_Delay */
// void DelayMs(uint32_t milliseconds){
// 	HAL_Delay(milliseconds);
// }



/// ***************************** BOARD ***************************** ///

/* BoardInit: Initializes HAL, System Clock, GPIO pins */
void BoardInit(void){
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
}


GPIN *GPIN_init(GPIN * const me, GPIO_PINS pin, GPIO_TYPE type, GPIO_MODES mode, PullConfig pull_config){
	// Update me*
	me->pin = pin;
	me->type = type;
	me->mode = mode;
	me->pull_config = pull_config;

	// Initialize
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = pin;
	GPIO_InitStruct.Mode = mode;
	GPIO_InitStruct.Pull = pull_config;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init((GPIO_TypeDef *)type, &GPIO_InitStruct); 

	return me;
}


void GPIN_set(GPIN * const me, GPIO_STATE state){
	HAL_GPIO_WritePin((GPIO_TypeDef *)me->type, me->pin, (GPIO_PinState)state);
}


void GPIN_toggle(GPIN * const me){
	HAL_GPIO_TogglePin((GPIO_TypeDef *)me->type, me->pin);
}


GPIO_STATE GPIN_read(GPIN * const me){
	return (GPIO_STATE)HAL_GPIO_ReadPin((GPIO_TypeDef *)me->type, me->pin);
}


GPIO_Lock_Status GPIN_lock(GPIN * const me){
	return (GPIO_Lock_Status)HAL_GPIO_LockPin((GPIO_TypeDef *)me->type, me->pin);
}


/// ***************************** TIMER ***************************** ///

/* HAL_Delay */
void DelayMs(uint32_t milliseconds){
	HAL_Delay(milliseconds);
}

int Board_GetTick(void){
	return (uint32_t)HAL_GetTick();
}
