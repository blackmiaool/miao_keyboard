#ifndef _KEY_REMAP_H_
#define _KEY_REMAP_H_
#include "stm32f4lib.h"
#include "app_interface.h"
#include "inifile.h"
u8 key_remap_init(void);
u8 key_table_process(u8* read_buf,u32 size);
#endif//_KEY_REMAP_H_

