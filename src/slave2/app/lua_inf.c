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
u8 use_lua=false;
int lua_handle(key_t* buf)
{
        int ret;
/*the function name*/
        lua_getglobal(L,"key_input");
/*the first argument*/
//        lua_pushnumber(L, buf[0]);
/*the second argument*/
//        lua_pushnumber(L, buf[2]);
        lua_pushnumber(L, buf->control);
        lua_pushnumber(L,buf->key_cnt);
		for(int i=0;i<buf->key_cnt;i++){
			lua_pushnumber(L, (buf->key[i].pos[0]<<4)+buf->key[i].pos[1]);
		}
//        for(int i=0;i<6-buf->key_cnt;i++){
//			lua_pushnumber(L, 0);
//		}
/*call the function with 2 arguments, return 1 result.*/
        lua_call(L, buf->key_cnt+2, 1);


        ret=lua_toboolean(L, -1);        /*get the result.*/
        lua_pop(L,1);/*cleanup the return*/
        return ret;
}


extern const u8 general_key_value[33];
extern const u8* general_key_map[33];
    
static int get_key_index(lua_State *L){
    const char *str=lua_tostring(L,1);
    u8 cnt=32;
    for(int i=1;i<cnt;i++){
        if(strcmp((const char*)general_key_map[i],str)==0)
        {
             lua_pushnumber(L, general_key_value[i]);
            return 1;
        }
    }
     lua_pushnumber(L, 0);
    return 1;
}

static int lua_delay_ms(lua_State *L)
{
    int ms=lua_tonumber(L,1);
    for(int i=0;i<ms/1000;i++)
        delay_ms(1000);
    delay_ms(ms%1000);
    return 0;
    
}
static int lua_output(lua_State* L){
    u8 buf[8];
    for(u8 i=0;i<8;i++){
        buf[i]=lua_tonumber(L,i+1);
    }
    app_press(buf);
    return 0;
}
void lua_init(){
	if(L)
		lua_close(L);
	L=(lua_State *) luaL_newstate();      
    luaL_openlibs(L);
    lua_register(L, "output",lua_output);
    lua_register(L, "delay",lua_delay_ms);
    lua_register(L, "get_key_index",get_key_index);

    lua_pop(L, 1);  // remove _PRELOAD table
	char *entry_file="main.lua";
	printf("==========lua print==========\r\n");
	
    u32 cnt=0;

    if(!f_open(&file,entry_file,FA_OPEN_EXISTING|FA_WRITE|FA_READ|FA_OPEN_ALWAYS|FA__WRITTEN)){
        use_lua=true;
		char *read_buf=(char *)malloc(file.fsize);
		f_read(&file,read_buf,file.fsize,&cnt);
		luaL_dostring(L,read_buf);
		f_close(&file);
		free(read_buf);
	}
}
