#ifndef _APPLICATION_H_
#define _APPLICATION_H_
#include "lua.h"
#include "lauxlib.h"

extern char* lua_string;
extern void rt_lua_thread_entry(void* parameter);
#endif //_APPLICATION_H_
