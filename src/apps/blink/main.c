// #include "board.h"
// 
// 
// #define FLASHER_DELAY 100
// 
// int main(void){
// 	BoardInit();
// 
// 	GPIO_Init(GPIN_13, GPIO_TYPE_C, GPIO_OUTPUT_MODE);
// 
// 	while (1){
// 		GPIO_Toggle(GPIN_13, GPIO_TYPE_C);
// 		DelayMs(FLASHER_DELAY);
// 	}
// }
// 


#include "board.h"


#define FLASHER_DELAY 100

int main(void){
	BoardInit();

	GPIN led;
	GPIN_init(&led, GPIN_13, GPIO_TYPE_C, GPIO_OUTPUT_MODE, GPIN_NO_PULL);

	while (1){
		GPIN_toggle(&led);
		DelayMs(FLASHER_DELAY);
	}
}

