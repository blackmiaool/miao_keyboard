#include "usb_process.h"
extern u8 EP1BUSY;			//�������ݷ���æ��־ 
extern u8 EP2BUSY;			//������ݷ���æ��־

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
	buf_wrap[0]=1;
	for(u8 i=0;i<5;i++){
		buf_wrap[i+1]=buf[i];
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
