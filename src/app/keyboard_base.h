#ifndef _APP_KEYBOARD_BASE_H_
#define _APP_KEYBOARD_BASE_H_
#include "stm32lib.h"
// #define LCtrl       0x1
// #define LShift     0x2
// #define LAlt        0x4
// #define LWin      0x8
// #define RCtrl      0x10
// #define RShift       0x20
// #define RAlt        0x40
// #define RWin    0x80
typedef struct{
    u8 pos[2];
} single_key_t;
typedef struct {
    u8 control;
    u8 cnt;
    single_key_t key[6];   
} key_t;

#endif//_APP_KEYBOARD_BASE_H_


