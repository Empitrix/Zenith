#include "board.h"


int main(void){
	boardInit();
	serialBegin(UART_SERIAL);
	while(1){
		serialPrint("Hello, World!\n");
		delayMs(200);
	}
}
