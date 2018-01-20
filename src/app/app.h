#ifndef _APP_INTERFACE_H_
#define _APP_INTERFACE_H_
#include "stm32lib.h"
#include "ff.h"
#include "stdio.h"
#include "keyboard.h"
#define DEBUG
#ifdef 	DEBUG
#define	DBG(...)  printf(__VA_ARGS__)
#else	  
#define	DBG(...)
#endif


typedef u8 uint8_t;

// #define LCtrl       0x1
// #define LShift     0x2
// #define LAlt        0x4
// #define LWin      0x8
// #define RCtrl      0x10
// #define RShift       0x20
// #define RAlt        0x40
// #define RWin    0x80

extern void app_init(void);
extern void app_handle(u8 *buf, key_t* bufp);
extern void app_press(u8 *buf);

#endif//_APP_INTERFACE_H_


