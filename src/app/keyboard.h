#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
#include "stm32lib.h"
#include "keyboard_base.h"
void keyboard_init(void);

void keyboard_scan(void);
#define  COL_LEN 14
#define  ROW_LEN 5
#define  KEY_BUF_LEN 14

struct key_filter_struct{
    u16 time;
    single_key_t key;
};
typedef struct key_filter_struct key_filter_t;
typedef struct{
    u8 cnt;
    key_filter_t filter[KEY_BUF_LEN];
} key_filter_list_t;

extern const unsigned char key_map[3][ROW_LEN][COL_LEN];
extern unsigned char current_mode;
extern unsigned char clean_mode;
extern unsigned char udisk_mode;
#endif //_KEYBOARD_H_
