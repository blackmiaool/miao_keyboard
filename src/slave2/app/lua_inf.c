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
        if(lua_pcall(L, buf->key_cnt+2, 1,0)==0){
			ret=lua_toboolean(L, -1); 
			lua_pop(L,1);
		}else{
			ret=0;
		}


               /*get the result.*/
        /*cleanup the return*/
        return ret;
}


extern const u8 general_key_value[33];
extern const u8* general_key_map[33];
const u8 general_key_value[]={0,
                              40,41,42,43,
                              44,57,58,59,
                              60,61,62,63,
                              64,65,66,67,

                              68,69,70,71,
                              72,73,74,75,
                              76,77,78,79,
                              80,81,82,83
                             };
 const u8* general_key_map[]={"",
                             "enter",            "esc",      "delete",       "tab",
                             "space",            "capsLock","f1",           "f2",
                             "f3",               "f4",       "f5",           "f6",
                             "f7",               "f8",       "f9",           "f10",

                             "f11",              "f12",      "printscreen",  "scrollLock",
                             "pause",            "insert",   "home",         "pageup",
                             "deleteforward",   "end",      "pagedown",     "right",
                             "left",             "down",     "up",           "numLock"
                            };
static int restart_keyboard(lua_State *L)
{
	printf("resetting\r\n");
	SCB->AIRCR = 0x05FA0000| (u32)0x04;
	return 0;
}
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
static void str_trim(char*pStr)  
{  
    char *pTmp = pStr;  
      
    while (*pStr != '\0')   
    {  
        if (*pStr != ' '&&*pStr != '\n'&&*pStr != '\r')  
        {  
            *pTmp++ = *pStr;  
        }  
        ++pStr;  
    }  
    *pTmp = '\0';  
} 
static int read_file(lua_State *L){
	FIL file;
	int n = lua_gettop(L);	
	const char *file_name=lua_tostring(L,1);
	u8 trim=0;
	if(n==2)
		trim=lua_tonumber(L,2);
	 if(!f_open(&file,file_name,FA_OPEN_EXISTING|FA_WRITE|FA_READ|FA__WRITTEN)){
		char *read_buf=(char *)malloc(file.fsize+1);
		u32 cnt=0;
		f_read(&file,read_buf,file.fsize,&cnt);
		f_close(&file);
		if(trim){
			str_trim(read_buf);
		}
		lua_pushstring(L, read_buf);
		free(read_buf);
		return 1;
	}
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
	lua_register(L, "read_file",read_file);
	lua_register(L, "restart_keyboard",restart_keyboard);
	
    lua_pop(L, 1);  // remove _PRELOAD table
	char *entry_file="main.lua";
	printf("==========lua print==========\r\n");
	
    u32 cnt=0;

    if(!f_open(&file,entry_file,FA_OPEN_EXISTING|FA_WRITE|FA_READ|FA__WRITTEN)){
        
		char *read_buf=(char *)malloc(file.fsize);
		f_read(&file,read_buf,file.fsize,&cnt);
		luaL_dostring(L,read_buf);
		f_close(&file);
		free(read_buf);
		use_lua=true;
	}
}
