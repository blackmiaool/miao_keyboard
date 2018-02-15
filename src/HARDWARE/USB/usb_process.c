#include "usb_process.h"
#include "w25q16.h"

extern u8 EP1BUSY;            //键盘数据发送忙标志
extern u8 EP2BUSY;            //鼠标数据发送忙标志

void UserToPMABufferCopy(u8 *pbUsrBuf, u16 wPMABufAddr, u16 wNBytes);
u16 GetEPTxAddr(u8 bEpNum);
void SetEPTxValid(u8 bEpNum);
#define ENDP1   ((u8)1)
#define ENDP2   ((u8)2)
void mouse_process(u8* buf){
    u8 buf_wrap[6];
    for(u8 i=0;i<6;i++){
        buf_wrap[i]=buf[i];
    }
    while(EP2BUSY!=0){;}
    EP2BUSY = 1; 
    UserToPMABufferCopy(buf_wrap, GetEPTxAddr(ENDP2), 5);
    SetEPTxValid(ENDP2);
} 
void keyboard_process(u8* buf)
{
    while(EP1BUSY!=0){;}
    EP1BUSY = 1;   
    UserToPMABufferCopy(buf, GetEPTxAddr(ENDP1), 8);
    SetEPTxValid(ENDP1);
}

u8 SD_ReadDisk(u8* buf,u32 sector,u32 cnt)        //����  512
{
//    printf("r%X ",sector);
    SPI_Flash_Read(buf,sector*512,cnt*512);
    return 0;
}
u8 SD_WriteDisk(u8*buf,u32 sector,u32 cnt)        //д��   512 
{
//    printf("r%X ",sector);
    SPI_Flash_Write(buf,sector*512,cnt*512);
    return 0;
}
