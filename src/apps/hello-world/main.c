#include "board.h"


int main(void){
	boardInit();
	serialBegin(USB_SERIAL);
	while(1){
		serialPrint("Hello, World!\n");
		delayMs(200);
	}
}
