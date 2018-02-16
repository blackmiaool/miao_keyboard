#ifndef _USB_PROCESS_H_
#define _USB_PROCESS_H_
#include "stm32lib.h"
#include "hw_config.h"
extern void mouse_process(u8 *buf);
extern void keyboard_process(u8 *buf);
extern u8 SD_ReadDisk(u8 *buf, u32 addr, u32 cnt);
extern u8 SD_WriteDisk(u8 *buf, u32 addr, u32 cnt);
#endif //_USB_PROCESS_H_
