#include "usb_process.h"
extern u8 EP1BUSY;			//�������ݷ���æ��־ 
extern u8 EP2BUSY;			//������ݷ���æ��־

void mouse_process(u8* buf)
{
	// u8 Buffer[8] = { 8, 1, 0, 0, 0, 0, 0};
	while(EP2BUSY!=0){;}
	
		EP2BUSY = 1;   
			
		Joystick_Send(0,buf);
		
		
	
}
void keyborad_process(u8* buf)
{
	//putchar('k');
	while(EP1BUSY!=0){;}
	
		EP1BUSY = 1;   
			
		Joystick_Send(0,buf);
	
}
