#include "usb_process.h"
#include "w25q16.h"

extern u8 EP1BUSY; //键盘数据发送忙标志
extern u8 EP2BUSY; //鼠标数据发送忙标志

void UserToPMABufferCopy(u8 *pbUsrBuf, u16 wPMABufAddr, u16 wNBytes);
u16 GetEPTxAddr(u8 bEpNum);
void SetEPTxValid(u8 bEpNum);
#define ENDP1 ((u8)1)
#define ENDP2 ((u8)2)

#define KB_SEND_ENDP ENDP1
#define KB_BUSY_FLAG EP1BUSY

#define MOUSE_SEND_ENDP ENDP2
#define MOUSE_BUSY_FLAG EP2BUSY

void send_data_to_endp(u8 *busy_flag, u8 *buf, u8 endp, u8 length)
{
    while (*busy_flag != 0)
    {
        ;
    }
    *busy_flag = 1;
    UserToPMABufferCopy(buf, GetEPTxAddr(endp), length);
    SetEPTxValid(endp);
}
void mouse_process(u8 *buf)
{
    send_data_to_endp(&MOUSE_BUSY_FLAG, buf, MOUSE_SEND_ENDP, 5);
}

void keyboard_process(u8 *buf)
{
    send_data_to_endp(&KB_BUSY_FLAG, buf, KB_SEND_ENDP, 8);
}

u8 SD_ReadDisk(u8 *buf, u32 addr, u32 cnt)
{
    SPI_Flash_Read(buf, addr, cnt);
    return 0;
}
u8 SD_WriteDisk(u8 *buf, u32 sector, u32 cnt)
{
    SPI_Flash_Write(buf, sector, cnt);
    return 0;
}
