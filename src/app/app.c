#include "app.h"
#include "string.h"
#include "ahk.h"
#include "keyboard.h"
#include "lua_inf.h"
/******************macro area******************/

/******************macro area end******************/
u8 *read_buf;
u8 buf_out[10];

void buf_clear(void);

u16 key_cap_cnt=0;
void reset_system(struct st_key_cap* a);

#define min(a,b) (a>b?b:a)

void buf_clear()
{
    u8 i=0;
    for(i=2;i<9;i++)
    {
        buf_out[i]=0;
    }
    buf_out[0]=6;
    press;

}



void reset_system(struct st_key_cap* a)
{
//NVIC_SystemReset();
//    u16 output=0;
//    press_string_pure(&output,1);
//    press_string_pure(&output,1);
	printf("resetting\r\n");
	SCB->AIRCR = 0x05FA0000| (u32)0x04;
}


u8 system_init=false;


extern u8 use_lua;
u8 key_capture(u8 *buf,key_t* bufp)
{
    int through2=0;
    if(use_lua){
        through2|=lua_handle(bufp);
    }
    return through2;
    
}
FATFS fs;
int fgetc(FILE *f){
	return 1;
}
//int fputc (int ch, FILE *f) {

//  return (ch);
//}


void app_init()
{
	
	if(clean_mode)
		return;

	u8 result=f_mount(&fs,"/",1);
    if(result)
    {
        DBG("FS mount failed!!\r\n%d",result);
		return;
    }

	lua_init();
}
extern void keyborad_process(u8* buf);
void app_press(u8 *buf){
	keyborad_process(buf);
}
void app_handle(u8 *buf,key_t* bufp){
//	DBG("CNT=%d\r\n",key_cap_cnt);
//	for(u8 i=0;i<8;i++)
//	{
//		DBG("+%d",buf[i]);
//	}
	if(clean_mode){
		app_press(buf);
	}
	else{
		if(!key_capture(buf, bufp))
        { 
            app_press(buf);
            
        }else{
//            DBG("capture");
        }
	}
	
}

