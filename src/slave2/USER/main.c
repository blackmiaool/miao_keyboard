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


u8 delegate=0;
void app_init(void);
void routine(){
	led_handle();
}
int calc_free_memory(){
	for(int i=10240;i>0;i--){
		char *area=(char *)malloc(i*100);
		if(area){
			free(area);
			return i;
		}		
	}
	return 0;
}
//int calc_free_memory(){
//	for(int i=10240;i>0;i--){
//		char *area=(char *)malloc(i*100);
//		if(area){
//			free(area);
//			return i;
//		}
//		
//	}
//	return 0;
//}
int main(void)
{
	Stm32_Clock_Init(9);//系统时钟设置
	delay_init(72);		//延时初始化
	uart_init(72,256000);

	JTAG_Set(1);//for free JTAG pins
    
    USB_Cable_Init();	
    USB_Cable_Config (ENABLE);

    
    
	delay_ms(200);		
  
	SPI_Flash_Init(); 
	
	while(SPI_Flash_ReadID()!=W25Q16)
	{
		printf("error%X\r\n",SPI_Flash_ReadID());
		delay_ms(500);

	}                        
	printf("start\r\n");
	Mass_Memory_Size[0]=4000*512;
	Mass_Block_Size[0] =512;
	Mass_Block_Count[0]=4000;
	Set_USBClock();
	USB_Interrupts_Config();  
	USB_Init();	
	
//	while(!INIT_OK);//等待初始化完成 
	delay_ms(1000);	

	
	keyboard_init();
	keyboard_scan();//exec before app_init to check if parse bmk files
	app_init();
	led_init();
	printf("last memory=%d",calc_free_memory());
	while(1){
		keyboard_scan();
		for(u8 i=0;i<70;i++){
			routine();			
			delay_us(100);
		}
	}


}

