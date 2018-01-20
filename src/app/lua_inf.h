#ifndef _LUA_INF_H_
#define _LUA_INF_H_
#include "stm32lib.h"
#include "keyboard_base.h"

extern u8 lua_handle(key_t* );
extern void lua_init(void);

extern u8 use_lua;
extern u8 *key_index_lua_buf;
#endif //_LUA_INF_H_
