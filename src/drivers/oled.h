#ifndef _OLED_H_
#define _OLED_H_
#include "stm32f4lib.h"
#include "rtdef.h"
#include <rthw.h>
#include <rtthread.h>
#include <stm32f4xx.h>
struct OLED_IO_info
{
  u16 clk[5];
  GPIO_TypeDef* base[5];
  u32 PIN[5];//CS  MISO MOSI SCK  DC
};
rt_err_t rt_hw_OLED_register(rt_device_t device, const char* name,
                              rt_uint32_t flag, struct OLED_IO_info *info);
void rt_hw_OLED_init(void);
void OLED_Init(void);


#endif//_OLED_H_

