#include "app.h"
#include "string.h"
#include "lua_inf.h"
#include "keyboard.h"
#include "lua.h"
#include "lapi.h"
#include "lauxlib.h"
#include "lstate.h"
#include <lualib.h>
#include <string.h>
FIL file; 
lua_State *L;
int lua_handle(u8* buf)
{
        int sum;
/*the function name*/
        lua_getglobal(L,"handle");
/*the first argument*/
//        lua_pushnumber(L, buf[0]);
/*the second argument*/
//        lua_pushnumber(L, buf[2]);
		for(int i=0;i<6;i++){
			lua_pushnumber(L, buf[i]);
		}
/*call the function with 2 arguments, return 1 result.*/
        lua_call(L, 6, 1);
/*get the result.*/
//        sum = (int)lua_tonumber(L, -1);
/*cleanup the return*/
        lua_pop(L,1);
        return sum;
}

void lua_init(){
	if(L)
		lua_close(L);
	L=(lua_State *) luaL_newstate();      
    luaopen_base(L); 
	char *entry_file="main.lua";
	printf("==========lua print==========\r\n");
	

	
    u32 cnt=0;

    if(!f_open(&file,entry_file,FA_OPEN_EXISTING|FA_WRITE|FA_READ|FA_OPEN_ALWAYS|FA__WRITTEN)){
		char *read_buf=(char *)malloc(file.fsize);
		f_read(&file,read_buf,file.fsize,&cnt);
		luaL_dostring(L,read_buf);
		f_close(&file);
		free(read_buf);
	}
}
