#ifndef _COMMU_MCU_H_
#define _COMMU_MCU_H_

void commu_send(u8 *buf,u32 len,u8 type);
void commu_recv(u8 *buf,u32 len,u8 type);
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
    COMMU_TYPE(COM_DATA),
    COMMU_TYPE(DEBUG),
};
#endif //_COMMU_MCU_H_
