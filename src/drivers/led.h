#ifndef _LED_H_
#define _LED_H_
#include "rtdef.h"
#include <rthw.h>
#include <rtthread.h>
#include <stm32f4xx.h>
#include "stm32f4lib.h"
#define LED_CNT 4
struct LED_IO_info
{
  u16 clk[LED_CNT];
  GPIO_TypeDef* base[LED_CNT];
  u32 PIN[LED_CNT];
};
rt_err_t rt_hw_LED_register(rt_device_t device, const char* name,
                              rt_uint32_t flag, struct LED_IO_info *info);
void rt_hw_LED_init(void);
#endif//_LED_H_
