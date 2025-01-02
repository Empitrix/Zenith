// #include "board.h"
// #include "gpio.h"
// #include "encoder.h"
// 
// 
// 
// int counter = 0; 
// int aState;
// int aLastState;  
// 
// int main(){
// 	boardInit();
// 
// 	gpio_t led = gpinInit(C_13, GPIO_OUTPUT_MODE, GPIN_NO_PULL);
// 	// gpio_t btn = gpinInit(A_8, GPIO_INPUT_MODE, GPIN_PULL_UP);
// 
// 	gpio_t s1 = gpinInit(A_11, GPIO_INPUT_MODE, GPIN_NO_PULL);
// 	gpio_t s2 = gpinInit(A_12, GPIO_INPUT_MODE, GPIN_NO_PULL);
// 
// 
// 	while(1){
// 
// 		// // *Rotary Button* // //
// 		// GPIO_STATE state = gpinRead(&btn);
// 		// if(state == PIN_ON){
// 		// 	gpinSet(&led, PIN_ON);
// 		// } else {
// 		// 	gpinSet(&led, PIN_OFF);
// 		// }
// 
// 
// 		aState = (int)gpinRead(&s1);
// 		if(aState != aLastState){
// 			if ((int)gpinRead(&s2) != aState){ 
// 				counter++;
// 			 } else {
// 				counter--;
// 			 }
// 		}
// 		aLastState = aState;
// 
// 
// 		if((counter % 2) == 0){
// 			gpinSet(&led, PIN_ON);
// 		} else {
// 			gpinSet(&led, PIN_OFF);
// 		}
// 	}
// }
