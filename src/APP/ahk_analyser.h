#ifndef _AHK_ANALYSER_H_
#define _AHK_ANALYSER_H_
#include "stm32f4lib.h"
#include "string.h"
#include "app_interface.h"

u8 control_key_decode(u8 key);
void filter_Init(ctrl_filter* filter);

int ahk_init(char *path);
enum ctrl_dir{ctrl_left=1,ctrl_right,ctrl_both,ctrl_raw};
u8 filter_add(struct control_key_filter* filter,u8* filter_info);
u8 filter_add_string(struct control_key_filter* filter,const char *string );
u8 key_mode_process(u8* read_buf,u32 size);
void filter_Init(ctrl_filter* filter);
#endif//_AHK_ANALYSER_H_

