#ifndef _COMMU_H_
#define _COMMU_H_
#include "stm32f4lib.h"
#include "usbh_hid_mouse.h"
void commu_send(u8* buf,u32 lenth);
void commu_recv(u8* buf,u32 lenth);
void commu_Init(void);
u8 commu_send_byte(u8 info);
extern char thread_commu_stack[1024];
extern struct rt_thread thread_commu;
void rt_thread_entry_commu(void* parameter);

extern char thread_commu_read_stack[1024];
extern struct rt_thread thread_commu_read;
void rt_thread_entry_commu(void* parameter);
extern rt_mq_t mq_commu_data;

void common_commu_send(u8 *buf,u32 len,u8 type);



#define COMMU_TYPE(a) COMMU_TYPE_##a
enum{
	COMMU_TYPE(FLASH_READ_CMD),
	COMMU_TYPE(FLASH_READ_DATA),
	COMMU_TYPE(FLASH_WRITE),
	COMMU_TYPE(KEYBOARD_MS),
	COMMU_TYPE(KEYBOARD_SM),
	COMMU_TYPE(MOUSE_MS),
	COMMU_TYPE(MOUSE_SM),
	COMMU_TYPE(COM_WRITE),
	COMMU_TYPE(COM_DATA)
};


#endif//_COMMU_H_

