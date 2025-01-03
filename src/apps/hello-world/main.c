#include "board.h"


int main(void){
	boardInit();

	while(1){
		serialPrint("Hello, World!\n");
		delayMs(200);
	}
}
