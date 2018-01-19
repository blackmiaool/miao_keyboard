#include "app.h"
#include "string.h"
#include "lua_inf.h"
#include "keyboard.h"
#include "usb_process.h"
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
	lua_getglobal(L,"key_input");
	lua_pushnumber(L, buf->control);
	lua_pushnumber(L,buf->cnt);
	for(int i=0;i<buf->cnt;i++){
		lua_pushnumber(L, (buf->key[i].pos[0]<<4)+buf->key[i].pos[1]);
	}
	int result=lua_pcall(L, buf->cnt+2, 1,0);
 
	if(result==0){
		ret=lua_toboolean(L, -1); 
		lua_pop(L,1);
	}else{
		ret=0;
		int t = lua_type(L, -1);  
    const char* err = lua_tostring(L,-1);  
    printf("Error: %s\n", err);
	}

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
    int ms=lua_tointeger(L,1);
    for(int i=0;i<ms/1000;i++)
        delay_ms(1000);
    delay_ms(ms%1000);
    return 0;
    
}
extern void mouse_process(u8* buf);
static int lua_mouse_output(lua_State* L){
	u8 buf[5];
    for(u8 i=0;i<5;i++){
        buf[i]=lua_tointeger(L,i+1);
    }
	mouse_process(buf);
	return 0;
}
static int lua_output(lua_State* L){
    u8 buf[8];
    for(u8 i=0;i<8;i++){
        buf[i]=lua_tointeger(L,i+1);
    }
    keyboard_process(buf);
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
		trim=lua_tointeger(L,2);
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
extern u8 led_buf[14][5];
extern u8 led_mode;
static int led_set_bit(lua_State *L){
	u8 x=13-lua_tointeger(L,1);
	u8 y=lua_tointeger(L,2);
	u8 v=lua_tointeger(L,3);
	led_buf[x][y]=!v;
	return 0;
}
static int led_clear(lua_State *L){
	for(u8 i=0;i<14;i++){
		for(u8 j=0;j<5;j++){
			led_buf[i][j]=1;
		}
	}
	return 0;
}
static int led_set_mode(lua_State *L){
	led_mode=lua_tointeger(L,1);
	return 0;
}
static int led_fill(lua_State *L){
	for(u8 i=0;i<14;i++){
		for(u8 j=0;j<5;j++){
			led_buf[i][j]=0;
		}
	}
	return 0;
}
int handle_datasheet(char *buf,int size,u8 width){
	int cnt=size/(width+1);
	u8 *data_base=(u8*)malloc(cnt);
	for(int i=0;i<cnt;i++){
		*(data_base+i)=atoi((buf+i*(width+1)));
	}
	return (int)data_base;
}
static int read_datasheet(lua_State *L){
	u8 *arr=(u8 *)lua_tointeger(L,1);
	int index=lua_tointeger(L,2);
	lua_pushinteger(L,arr[index]);
	return 1;
}
u8 *key_index_lua_buf;
static int init_datasheet(lua_State *L){
	FIL file;

	const char *file_name=lua_tostring(L,1);
	u8 width=lua_tointeger(L,2);
	
	if(!f_open(&file,file_name,FA_OPEN_EXISTING|FA_WRITE|FA_READ|FA__WRITTEN)){
		char *read_buf=(char *)malloc(file.fsize+1);
		u32 cnt=0;
		f_read(&file,read_buf,file.fsize,&cnt);
		f_close(&file);
		str_trim(read_buf);
		int buf_p=handle_datasheet(read_buf,cnt,width);
		if(!strcmp(file_name,"config/key_index.txt")){
			key_index_lua_buf=(u8 *)buf_p;
		}
		lua_pushinteger(L,buf_p);
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
	lua_register(L, "mouse_output",lua_mouse_output);
    lua_register(L, "delay",lua_delay_ms);
    lua_register(L, "get_key_index",get_key_index);
	lua_register(L, "read_file",read_file);
	lua_register(L, "restart_keyboard",restart_keyboard);
	lua_register(L, "led_set_bit",led_set_bit);
	lua_register(L, "led_clear",led_clear);
	lua_register(L, "led_set_mode",led_set_mode);
	lua_register(L, "led_fill",led_fill);
	lua_register(L, "read_datasheet",read_datasheet);
	lua_register(L, "init_datasheet",init_datasheet);
	
    lua_pop(L, 1);  // remove _PRELOAD table
	char *entry_file="main.lua";
	printf("==========lua print==========\r\n");
	
    u32 cnt=0;

    if(!f_open(&file,entry_file,FA_OPEN_EXISTING|FA_WRITE|FA_READ|FA__WRITTEN)){
        
		char *read_buf=(char *)malloc(file.fsize);
		f_read(&file,read_buf,file.fsize,&cnt);
		int result=luaL_dostring(L,read_buf);
		if ( result != 0 )  
        {  
          int t = lua_type(L, -1);  
          const char* err = lua_tostring(L,-1);  
          printf("Error: %s\n", err); 
        }
		printf("==========lua init result %d==========\r\n",result);
		f_close(&file);
		free(read_buf);
		use_lua=true;
	}
}
