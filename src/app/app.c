#include "app.h"
#include "string.h"
#include "keyboard.h"
#include "lua_inf.h"

u8 *read_buf;
u8 buf_out[10];

static u16 key_cap_cnt = 0;


void reset_system() {
	printf("resetting\r\n");
	SCB->AIRCR = 0x05FA0000 | (u32) 0x04;
}

extern u8 use_lua;
static u8 key_capture(key_t* bufp) {
	if (use_lua) {
		return lua_handle(bufp);
	}else{
		return 0;
	}
}

static FATFS fs;

void app_init() {
	if (clean_mode)
		return;

	u8 result = f_mount(&fs, "/", 1);
	if (result) {
		DBG("FS mount failed!!\r\n%d", result);
		return;
	}

	lua_init();
}
extern void keyboard_process(u8* buf);

void app_handle(u8 *buf, key_t* bufp) {
//	DBG("CNT=%d\r\n",key_cap_cnt);
//	for(u8 i=0;i<8;i++)
//	{
//		DBG("+%d",buf[i]);
//	}
	if (clean_mode) {
		keyboard_process(buf);
	} else if (!key_capture(bufp)) {
		keyboard_process(buf);
	}

}

