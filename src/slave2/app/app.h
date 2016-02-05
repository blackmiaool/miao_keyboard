#ifndef _APP_INTERFACE_H_
#define _APP_INTERFACE_H_
#include "stm32lib.h"
#include "ff.h"
#include "stdio.h"
#define DEBUG
#ifdef 	DEBUG
#define	DBG(...)  printf(__VA_ARGS__)
#else	  
#define	DBG(...)
#endif

u8 key_capture(u8 *buf);
void key_cap_Init(void);
void press_string_pure(u16 *buf,u32 lenth);
s8 control_key_index(const char buf[]);
extern const u8  ascii2usb[128];
typedef u8 uint8_t;
//typedef char int8_t;
extern u8 blue_choose;
#define LCtrl       0x1
#define LShift     0x2
#define LAlt        0x4
#define LWin      0x8
#define RCtrl      0x10
#define RShift       0x20
#define RAlt        0x40
#define RWin    0x80
#define key_string_num 8
#define key_stinrg_max_word 6
#define key_cap_cnt_all 70
extern const u8 shift_table[128];
void cmd(u8* content);

extern u8 *read_buf;
typedef struct block_information block_info;
struct control_key_filter
{
    u8 control_filter[8];
    u8 control_filter_cnt;
    u8 key;
};
typedef struct control_key_filter ctrl_filter;
struct block_information
{
    ctrl_filter filter;
    u8 state;//0:nothing(not init) 1:inited 2:find ctrl key 3:in '::'
};
struct st_key_cap{
    ctrl_filter filter;
    void (*key_exe)(struct st_key_cap*);
    //  st_key_cap* pre;
    struct st_key_cap* next;
    u16 *string;
    u16 string_lenth;
    u8 flag;
};
typedef struct st_key_cap cap;
extern cap key_cap_free[key_cap_cnt_all];

extern u8 buf_out[10];
void buf_clear(void );
extern void app_press(u8 *buf);
#define press  do{app_press(buf_out+1);}while(0);
extern const u8 key_string[key_string_num][key_stinrg_max_word];
extern u8  key_changetable[8];
void  key_cap_add(cap* cap_this);
#endif//_APP_INTERFACE_H_


