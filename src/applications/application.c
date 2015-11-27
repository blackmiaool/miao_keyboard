/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */

/**
 * @addtogroup STM32
 */
/*@{*/

#include <stdio.h>
#include "app_interface.h"
#include "stm32f4xx.h"

#include <board.h>
#include <rtthread.h>
#include "debug.h"
#include "usb_bsp.h"
#include "usbh_core.h"
#include "usbh_usr.h"
#include "usbh_hid_core.h"
#include "stm32f4lib.h"
#include "commu.h"
#include "w25q16.h"
#include "mouse_gesture.h"
#include "inifile.h"
#include "key_remap.h"
volatile u32 flash_addr;
u8 flash_buf[512];
void cmd(u8* content);
void commu_blue_send(u8* buf,u32 lenth);
void delay_ms2(u32 ms);
extern rt_mq_t mq_commu;
extern rt_mq_t mq_lua;
//rt_mq_t mq_commu;
extern rt_sem_t sem_commu;
extern rt_sem_t sem_commu_self;
extern rt_sem_t sem_app_init;
rt_sem_t sem_ld3320;
rt_sem_t sem_flash;
//   rt_mailbox_t mb_commu;
#define UART4_GPIO_TX		GPIO_Pin_0
#define UART4_TX_PIN_SOURCE GPIO_PinSource0
#define UART4_GPIO_RX		GPIO_Pin_1
#define UART4_RX_PIN_SOURCE GPIO_PinSource1
#define UART4_GPIO			GPIOA
#define UART4_GPIO_RCC   	RCC_AHB1Periph_GPIOA
#define RCC_APBPeriph_UART4	RCC_APB1Periph_UART4
void blue_putchar(u8 ch)
{
    USART_SendData(UART4, (uint8_t) ch);
    while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET){;}
}
void UART4_IRQHandler(void)//bluetooth
{
    static long lenth=0;
    static u8 buf[9];
    /* enter interrupt */
    rt_interrupt_enter();

    //rt_hw_serial_isr(&uart3_device);
    buf[lenth]=UART4->DR;
    lenth++;
    if(lenth==9)
    {
        lenth=0;
        rt_mq_send (mq_commu, (void*)buf, 9);
    }
    /* leave interrupt */
    rt_interrupt_leave();
    USART_ClearITPendingBit(UART4, USART_IT_RXNE);
}
void blue_tooth_Init()
{
    NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    SCPE(PERIOC);
    SCPE(PERIOA);
    IOConfig(IOCB,PIN13,tuiwanshuchu);


	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;

	/* Enable USART3 clock */
	RCC_APB1PeriphClockCmd(RCC_APBPeriph_UART4, ENABLE);


	GPIO_InitStructure.GPIO_Pin =   UART4_GPIO_TX|UART4_GPIO_RX;
	GPIO_Init(UART4_GPIO, &GPIO_InitStructure);
  //  IOConfig(IOAB,PIN1,fukongshuru);

    /* Connect alternate function */
    GPIO_PinAFConfig(UART4_GPIO, UART4_TX_PIN_SOURCE, GPIO_AF_UART4);
    GPIO_PinAFConfig(UART4_GPIO, UART4_RX_PIN_SOURCE, GPIO_AF_UART4);
	/* DMA clock enable */

    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4, &USART_InitStructure);
    
    USART_Cmd(UART4, ENABLE);
//        IO1(IOCB,PIN13);
//    rt_thread_delay(1000);
//    //IO(IOCB,PIN13);
//    rt_thread_delay(1000);
//    //blue_puts("at+uart=115200,0,0\r\n");//1st ADDR:14:1:61429
//    rt_thread_delay(1000);
//   // blue_puts("at+bind=14,1,61429\r\n");
//    rt_thread_delay(1000);
//  //  blue_puts("AT+ROLE=0\r\n");
//    rt_thread_delay(1000);
    IO0(IOCB,PIN13);

}
#include "usbh_hid_mouse.h"
u8 blue_choose=0;


void LD3320_main_Init(void);
void ProcessInt0(void );
void LD_loop(void);
ALIGN(RT_ALIGN_SIZE)
static char thread_ld3320_stack[3096];
struct rt_thread thread_ld3320;
static void rt_thread_entry_ld3320(void* parameter)
{
    if(!ini.Service.audio)
        return ;
	DBG("LD3320 Init_start\r\n");
    LD3320_main_Init();
	DBG("LD3320 Init_end\r\n");
	LD_loop();
    while(1)
    {
		
        while(PCin(2)!=0)
        {
            rt_thread_delay(1); 
        }
        ProcessInt0();
        LD_loop();
    }
}


ALIGN(RT_ALIGN_SIZE)
static char thread_Flash_Read_stack[1024];
struct rt_thread thread_Flash_Read;
static void rt_thread_entry_Flash_Read(void* parameter)
{

    u8 buf[15];
    u8 i=0;
    for(i=0;i<15;i++)
        buf[i]=0;


    SCPE(PERIOB);
    IOConfig(IOBB,PIN6,xialashuru);
   // rt_thread_delay(100);
    //	DBG("--------------------SCAN_START\r\n");
    rt_thread_delay(100);
    while(1)
    {
        while(PBin(6)==0)
        {
            rt_thread_delay(1);
        }
        //DBG("----------FOUND\r\n");
        buf[0]=3;//read!
        rt_mq_send (mq_commu, (void*)buf, 9);//ask addr
        rt_sem_take(sem_commu,RT_WAITING_FOREVER);//addr gotton
        
        if(flash_addr>=1000000)
        {
            buf[0]=5;
            rt_mq_send (mq_commu, (void*)buf, 9);
            rt_sem_take(sem_commu,RT_WAITING_FOREVER);
            rt_sem_take(sem_flash,RT_WAITING_FOREVER);
            SPI_Flash_Write(flash_buf, (flash_addr-1000000)<<9, 512);
            rt_sem_release(sem_flash);
        }
        else
        {
          
            SPI_Flash_Read(flash_buf, flash_addr<<9, 512);
            buf[0]=4;
            rt_mq_send (mq_commu, (void*)buf, 9);
            rt_sem_take(sem_commu,RT_WAITING_FOREVER);
        }
    }
    

}
void assert_failed(uint8_t* file, uint32_t line)
{
printf("Wrong parameters value: file %s on line %d\r\n",file, line);
}


__ALIGN_BEGIN USB_OTG_CORE_HANDLE           USB_OTG_Core_dev __ALIGN_END ;
__ALIGN_BEGIN USBH_HOST                     USB_Host __ALIGN_END ;
static char thread_usb_stack[1024];
struct rt_thread thread_usb;
extern uint8_t err_flag;
extern char debug_en;
static void rt_thread_entry_usb(void* parameter)
{
    #ifndef DEBUG
    #define DEBUG
    #endif
    USBH_Init(&USB_OTG_Core_dev,//hardware reg&info
              USB_OTG_FS_CORE_ID,		//use FS not HS(enum)
              &USB_Host,//USB state
              &HID_cb,//Class callback structure
              &USR_Callbacks);//User callback structure
	

	while(1)
	{
		USBH_Process(&USB_OTG_Core_dev , &USB_Host);
        rt_thread_delay(3);       
	} 
}
extern  char thread_app_stack[10196];
extern struct rt_thread thread_app;
void rt_thread_entry_app(void* parameter);


FATFS fs;
ALIGN(RT_ALIGN_SIZE)
char thread_init_stack[20024];
struct rt_thread
thread_init;
extern bool system_init;
void rt_thread_entry_init(void* parameter)
{
//    rt_device_t LED_dev;
//    u8 led_value;
//    u8 i;
	u8 result=f_mount(&fs,"/",1);
    if(result)
    {
        rt_kprintf("FS mount failed!!\r\n%d",result);
		return;
    }
    
    ini_init();
    key_cap_Init();
	system_init=true;
	while(1)
	{
        rt_thread_delay(1000);       
	}
//    blue_tooth_Init();
//    commu_Init();
//    OLED_dev=rt_device_find("OLED");
//    if(OLED_dev!=RT_NULL)
//    {
//        rt_kprintf("OLED dev found\n\r");
//    }  
//	rt_device_open(OLED_dev,RT_DEVICE_FLAG_ACTIVATED);
//    
//    
//    LED_dev=rt_device_find("LED");
//    rt_device_open(LED_dev,RT_DEVICE_FLAG_ACTIVATED);
//    if(LED_dev!=RT_NULL)
//    {
//        rt_kprintf("LED dev found\n\r");
//        rt_device_write(LED_dev,0,&led_value,1);
//        led_value++;
//    }
//    rt_device_close(LED_dev);
//    cmd("Initializing~");
//    
    
//    for(i=0;i<1;i++)
//    draw_bmp(i,63,"/background.bmp");
//    
//   // draw_bmp(0,43,"/24L01_1.bmp");
//    draw_bmp(26,43,"/icon/AHKScript.bmp");
//    draw_bmp(52,43,"/icon/KeyBoardOff.bmp");
//    draw_bmp(78,43,"/icon/AHKScript_1.bmp");
//    draw_bmp(104,43,"/icon/micoff.bmp");
//    
//    draw_bmp(0,23,"/icon/MouseOff.bmp");
//    draw_bmp(26,23,"/icon/udisk_rd.bmp");
//    draw_bmp(52,23,"/icon/udisk_rd.bmp");
//    draw_bmp(78,23,"/icon/udisk_rd.bmp");
//    draw_bmp(104,23,"/icon/udisk_rd.bmp");
//    
//    draw_bmp(0,63,"/icon/udisk_rd.bmp");
//    draw_bmp(26,63,"/icon/udisk_rd.bmp");
//    draw_bmp(52,63,"/icon/udisk_rd.bmp");
//    draw_bmp(78,63,"/icon/udisk_rd.bmp");
//    draw_bmp(104,63,"/icon/udisk_rd.bmp");
    
}
char thread_test_stack[1024];
struct rt_thread thread_test;
void rt_thread_entry_test(void* parameter){
	while(1){
		rt_thread_delay(1000);
//		rt_kprintf("found\n\r");
//		u8 buf[8]={0,0,39};
//		common_commu_send(buf,8,COMMU_TYPE(KEYBOARD_SM));		
//		rt_thread_delay(1000);	
//		u8 empty[8]={COMMU_TYPE(KEYBOARD_SM)};
//		common_commu_send(empty,8);
	}
}

char thread_key_sm_stack[1024];
struct rt_thread thread_key_sm_send;
void rt_thread_entry_key_sm(void* parameter){
	while(1){
		u8 buf[9];
		rt_mq_recv(mq_key_sm,buf,9,RT_WAITING_FOREVER);
		if(buf[8])//auto press
		{
			rt_thread_delay(10);
		}else{//normal
			
		}
		
		common_commu_send(buf,8,COMMU_TYPE(KEYBOARD_SM));
	}
}


char thread_commu_send_stack[1024];
struct rt_thread thread_commu_send;

extern void rt_thread_entry_commu_send(void* parameter);
extern rt_mailbox_t mb_commu_send;
extern rt_mq_t mq_commu_recv;
extern rt_mq_t mq_flash_read;
extern rt_mq_t mq_key_ms;
extern rt_mq_t mq_key_sm;
int rt_application_init()
{
 
	rt_thread_init(&thread_commu_read,
                   "commu",
                   rt_thread_entry_commu,
                   RT_NULL,
                   &thread_commu_read_stack[0],
            sizeof(thread_commu_read_stack),11,5);
	rt_thread_init(&thread_test,
                   "test",
                   rt_thread_entry_test,
                   RT_NULL,
                   &thread_test_stack[0],
            sizeof(thread_test_stack),13,5);
	rt_thread_init(&thread_commu_send,
                   "commu_send",
                   rt_thread_entry_commu_send,
                   RT_NULL,
                   &thread_commu_send_stack[0],
            sizeof(thread_commu_send_stack),13,5);
	
	rt_thread_init(&thread_init,
			   "init",
			   rt_thread_entry_init,
			   RT_NULL,
			   &thread_init_stack[0],
		sizeof(thread_init_stack),1,5);
	rt_thread_init(&thread_app,
                   "app",
                   rt_thread_entry_app,
                   RT_NULL,
                   &thread_app_stack[0],
            sizeof(thread_app_stack),10,5);
    rt_thread_startup(&thread_app);
	
	rt_thread_init(&thread_key_sm_send,
                   "key_sm",
                   rt_thread_entry_key_sm,
                   RT_NULL,
                   &thread_key_sm_stack[0],
            sizeof(thread_key_sm_stack),3,5);
    rt_thread_startup(&thread_key_sm_send);
	
	
	
	
    rt_thread_startup(&thread_commu_read);
	rt_thread_startup(&thread_test);
	rt_thread_startup(&thread_commu_send);
	rt_thread_startup(&thread_init);
	
	
	sem_commu_self=rt_sem_create ("sem_commu_self", 0, RT_IPC_FLAG_FIFO);
	mq_commu=rt_mq_create ("mq_commu", 9, 100, RT_IPC_FLAG_FIFO);
	
	mq_flash_read=rt_mq_create ("mq_flash_read", 600, 3, RT_IPC_FLAG_FIFO);
	mb_commu_send=rt_mb_create ("mb_commu_send", 10, RT_IPC_FLAG_FIFO);
	mq_commu_data=rt_mq_create ("mq_commu_data", 3, 600, RT_IPC_FLAG_FIFO);
	mq_commu_recv=rt_mq_create ("mq_commu_recv", 600, 4, RT_IPC_FLAG_FIFO);
	mq_key_ms=rt_mq_create ("mq_key_ms", 9, 40, RT_IPC_FLAG_FIFO);	
	mq_key_sm=rt_mq_create ("mq_key_sm", 9, 100, RT_IPC_FLAG_FIFO);	
	
	mq_lua=rt_mq_create ("mq_lua", 10, 100, RT_IPC_FLAG_FIFO);
    
    sem_commu=rt_sem_create ("sem_commu", 0, RT_IPC_FLAG_FIFO);

    sem_flash=rt_sem_create ("sem_flash", 1, RT_IPC_FLAG_FIFO);
    sem_app_init=rt_sem_create ("sem_init", 0, RT_IPC_FLAG_FIFO);
	
	
	
	return 0;
	while(1){
		
		delay_ms2(3000);
	}

    

    rt_thread_init(&thread_usb,
                   "thread_usb",
                   rt_thread_entry_usb,
                   RT_NULL,
                   &thread_usb_stack[0],
            sizeof(thread_usb_stack),6,5);
    rt_thread_startup(&thread_usb);




    rt_thread_init(&thread_commu,
                   "commu",
                   rt_thread_entry_commu,
                   RT_NULL,
                   &thread_commu_stack[0],
            sizeof(thread_commu_stack),7,5);
    rt_thread_startup(&thread_commu);


    rt_thread_init(&thread_Flash_Read,
                   "flash",
                   rt_thread_entry_Flash_Read,
                   RT_NULL,
                   &thread_Flash_Read_stack[0],
            sizeof(thread_Flash_Read_stack),11,5);
    rt_thread_startup(&thread_Flash_Read);

    rt_thread_init(&thread_ld3320,
                   "ld3320",
                   rt_thread_entry_ld3320,
                   RT_NULL,
                   &thread_ld3320_stack[0],
            sizeof(thread_ld3320_stack),12,5);
    rt_thread_startup(&thread_ld3320);
    
    
        
  //  Jacob_appinit();

    return 0;
}
