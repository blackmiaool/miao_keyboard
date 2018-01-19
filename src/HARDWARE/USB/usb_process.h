#ifndef _USB_PROCESS_H_
#define _USB_PROCESS_H_
#include "stm32lib.h"
#include "hw_config.h"
void mouse_process(u8* buf);
void keyboard_process(u8* buf);
#endif//_USB_PROCESS_H_
