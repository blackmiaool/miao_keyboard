#ifndef _COMMU_H_
#define _COMMU_H_
#include "stm32lib.h"
#include "bsp.h"
void commu_init(void );
void commu_write(char *buf,u32 len);
u8 *commu_read(u32 *len);

u8 SD_ReadDisk(u8*buf,u32 sector,u32 cnt);		//¶Á¿é
u8 SD_WriteDisk(u8*buf,u32 sector,u32 cnt);		//Ð´¿é

#endif//_COMMU_H_

