#include <board.h>
#include <gpio.h>


int main(void){
	boardInit();

	gpio_t pin = gpinInit(C_13, GPIO_OUTPUT_MODE, GPIN_NO_PULL);

	while(1){
		gpinToggle(&pin);
		delayMs(100);
	}
}
