#include "keyboard.h"
void keyboard_init(void){
	TM_GPIO_Init(GPIOC, GPIO_PIN_4, TM_GPIO_Mode_IN, TM_GPIO_OType_PP, TM_GPIO_PuPd_DOWN, TM_GPIO_Speed_Low);
	TM_GPIO_Init(GPIOC, GPIO_PIN_5, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Low);
//	TM_GPIO_SetPullResistor(GPIOC, GPIO_PIN_1, TM_GPIO_PuPd_UP);
	TM_GPIO_SetPinHigh(GPIOC,GPIO_PIN_5);
}
