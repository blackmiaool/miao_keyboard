#include <stm32f10x_lib.h>
#include "sys.h"
#include "delay.h"	   
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"  
#include "usart.h"
#include "stm32lib.h"
#include "commu.h"
#include "usb_process.h"
#include "mass_mal.h"
extern u8 EP1BUSY;			//键盘数据发送忙标志 
extern u8 EP2BUSY;			//鼠标数据发送忙标志
extern u8 INIT_OK;



extern long long Mass_Memory_Size[MAX_LUN+1];
extern u32 Mass_Block_Size[MAX_LUN+1];
extern u32 Mass_Block_Count[MAX_LUN+1];
extern u8 USB_STATUS_REG;

//mouse      1 x y button  hua 
//keyboard   2 ~
extern u8 ptr_write;
extern u8 ptr_read;
u8 buf_test[512];
int main(void)
{
	u32 i=0;

	Stm32_Clock_Init(9);//系统时钟设置
	delay_init(72);		//延时初始化
	uart_init(72,115200);
	//USB配置
	  
	
	JTAG_Set(1);
    delay_ms(20);		
	commu_Init();
	SCPE(PERIOA);
    IOConfig(IOAB,low,PIN4|PIN5|PIN6|PIN7,8);
	Set_USBClock();
	USB_Interrupts_Config();  
	USB_Init();	
	delay_ms(1000);			//等待初始化完成 
	
	
	Mass_Memory_Size[0]=16000*512;
	Mass_Block_Size[0] =512;
	Mass_Block_Count[0]=16000;
	

	while(1)
	{
		if(ptr_write!=ptr_read)
		{
			commu_flag=2;
			//printf("---%d---cmb%d----kbd?%d---write%d\r\n",commu_buf[ptr_read][4],ptr_read,commu_buf[ptr_read][0],ptr_write);
			 
			for(i=0;i<9;i++)
			{
				//putchar(commu_buf[ptr_read][i]);
			}
			if(commu_buf[ptr_read][0]==1)
			{
				if(commu_buf[ptr_read][8]==90)
				mouse_process(commu_buf[ptr_read]);
			}
			else if(commu_buf[ptr_read][0]==0)
			{
				if(commu_buf[ptr_read][8]==92)
				keyborad_process(commu_buf[ptr_read]);
			}
			commu_lenth=0;
			ptr_read=(ptr_read+1)%50;
		}
	}

}

