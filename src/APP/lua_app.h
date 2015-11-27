#ifndef _LUA_APP_H_
#define _LUA_APP_H_
#include "stm32f4lib.h"
#include "app_interface.h"
#define  LUA_KEYBOARD_ENABLE_MASK   0x1
#define  LUA_MOUSE_ENABLE_MASK      0x2
#define  LUA_MACRO_PLAY_MASK      0x4
extern rt_mq_t mq_lua;
extern u32 lua_flag;//bit0:keyboard enable bit1:mouse enable 
void lua_init(void);
#endif//_LUA_APP_H_

