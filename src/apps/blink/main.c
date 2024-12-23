#include "board.h"


#define FLASHER_DELAY 100

int main(void){
	BoardInit();

	PinMode_Init(GPIN_4 | GPIN_5 | GPIN_6 | GPIN_7, GPIO_TYPE_A, GPIO_OUTPUT_MODE);

	while (1){
		GPIO_PINS pins[] = {GPIN_4, GPIN_5, GPIN_6, GPIN_7};
		int len = sizeof(pins) / sizeof(pins[0]);

		for(int i = 0; i < len; i++){
			PinMode_Set(pins[i], GPIO_TYPE_A, PIN_ON);
			DelayMs(FLASHER_DELAY);
			PinMode_Set(pins[i], GPIO_TYPE_A, PIN_OFF);
			DelayMs(FLASHER_DELAY);
		}

	}
}

