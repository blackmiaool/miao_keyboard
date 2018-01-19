#include "usb_process.h"
extern u8 EP1BUSY;			//键盘数据发送忙标志
extern u8 EP2BUSY;			//鼠标数据发送忙标志

//void mouse_process(u8* buf)
//{
//	// u8 Buffer[8] = { 8, 1, 0, 0, 0, 0, 0};
//	while(EP2BUSY!=0){;}
//	
//		EP2BUSY = 1;   
//			
//		Joystick_Send(0,buf);
//		
//		
//	
//}
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
void keyborad_process(u8* buf)
{
	while(EP1BUSY!=0){;}
	EP1BUSY = 1;   
	UserToPMABufferCopy(buf, GetEPTxAddr(ENDP1), 8);
	SetEPTxValid(ENDP1);
}
