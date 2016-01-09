#include <stm32f10x_lib.h>
#include "sys.h"
#include "delay.h"	   
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"  
#include "usart.h"
#include "stm32lib.h"
#include "usb_process.h"
#include "mass_mal.h"
#include "keyboard.h"
#include "commu.h"
#include "commu_mcu.h"
#include "w25q16.h"
#include "led.h"
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

u8 buf_try[]={0,0,20,0,0,0,0,0};
extern bool keyboard_flag;
extern u8 buf_send[9];
extern u8 buf_key[9];
u8 delegate=0;
void app_init(void);
void routine(){
	led_handle();
}
int main(void)
{
	Stm32_Clock_Init(9);//系统时钟设置
	delay_init(72);		//延时初始化
	uart_init(72,256000);

	JTAG_Set(1);//for free JTAG pins
	delay_ms(200);		
  
	SPI_Flash_Init(); 
	
	while(SPI_Flash_ReadID()!=W25Q16)
	{
		printf("error%X\r\n",SPI_Flash_ReadID());
		delay_ms(500);

	}
	Mass_Memory_Size[0]=4000*512;
	Mass_Block_Size[0] =512;
	Mass_Block_Count[0]=4000;
	Set_USBClock();
	USB_Interrupts_Config();  
	USB_Init();	
	
	
	delay_ms(1000);			//等待初始化完成 

	
	keyboard_init();
	keyboard_scan();
	app_init();
	led_init();
	while(1){
		keyboard_scan();
//		commu_send("miao\r\n",6,COMMU_TYPE(DEBUG));
		for(u8 i=0;i<110;i++){
// 			if(keyboard_flag){				
//				keyboard_flag=0;
//				keyborad_process(buf_key);
//			}
			routine();			
			delay_us(100);
		}
	}

//	while(1)
//	{
//		if(ptr_write!=ptr_read)
//		{
//			commu_flag=2;
//			//printf("---%d---cmb%d----kbd?%d---write%d\r\n",commu_buf[ptr_read][4],ptr_read,commu_buf[ptr_read][0],ptr_write);
//			 
//			for(i=0;i<9;i++)
//			{
//				//putchar(commu_buf[ptr_read][i]);
//			}
//			if(commu_buf[ptr_read][0]==1)
//			{
//				if(commu_buf[ptr_read][8]==90)
//				mouse_process(commu_buf[ptr_read]);
//			}
//			else if(commu_buf[ptr_read][0]==0)
//			{
//				if(commu_buf[ptr_read][8]==92)
//				keyborad_process(commu_buf[ptr_read]);
//			}
//			commu_lenth=0;
//			ptr_read=(ptr_read+1)%50;
//		}
//	}

}

