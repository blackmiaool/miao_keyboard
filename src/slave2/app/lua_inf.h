#ifndef _LUA_INF_H_
#define _LUA_INF_H_
#include "stm32lib.h"
int lua_handle(key_t* );
void lua_init(void);

extern u8 *key_index_lua_buf;
#endif //_LUA_INF_H_
