#include "commu.h"
#include "debug.h"
#include "rtthread.h"
#include "app_interface.h"
#include "mouse_gesture.h"

//PIN0~PIN7:info
//PIN8:CLK
//PIN9:endflag(high enable)
//#define CLK_H IO1(IOEB,PIN11)
//#define CLK_L IO0(IOEB,PIN11)
//#define END_H IO1(IOEB,PIN13)
//#define END_L IO0(IOEB,PIN13)
//#define DATA_SET(a) do{GPIOE->ODR&=0XFC03;GPIOE->ODR|=((((u16)a)<<2)&0X3fc);}while(0)
rt_mq_t mq_commu_data;
rt_mq_t mq_commu;
rt_sem_t sem_commu;
rt_sem_t sem_commu_self;
rt_sem_t sem_app_init;
rt_mq_t mq_commu_write;
extern  u32 flash_addr;
extern u8 blue_choose;
void commu_blue_send(u8* buf,u32 lenth);
void key_cap_Init(void);
bool key_capture(u8 *buf);
extern rt_mq_t mq_commu;
void blue_tooth_Init(void);
ALIGN(RT_ALIGN_SIZE)
char thread_commu_stack[1024];



struct rt_thread thread_commu;
u8 commu_send_byte(u8 info);


#define IRQ_L IO0(IOCB,PIN9)
#define IRQ_H IO1(IOCB,PIN9)

//#define COMMU_MQ_RECV_FINISH 0
//#define COMMU_MQ_SEND_FINISH 1
//#define COMMU_MQ_FLASH_WRITE 2
//#define COMMU_MQ_FLASH_READ 3
//#define COMMU_MQ_KB_WRITE 4
//#define COMMU_MQ_MOUSE_WRITE 5

//static u8 spi_state;
//static u32 spi_send_len;
//static u8 *spi_send_buf;
void commu_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStruct;
    
    SCPE(PERIOA);
    SCPE(PERIOC);
    SCPE(PERSPI3);

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    // enable clock for used IO pins
    
    /* configure pins used by SPI3
     * PC10 = SCK
     * PC11 = MISO
     * PC12 = MOSI
	 * PA15 = NSS
	 * PC9  = slave has info
     */
    IOConfig(IOCB,PIN9,tuiwanshuchu);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_11 | GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
    IRQ_L;
    
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init( GPIOA, &GPIO_InitStructure );
    
    
    
    
    
    // connect SPI3 pins to SPI alternate function
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource15,GPIO_AF_SPI3);
    
    
    SPI_I2S_ITConfig(SPI3, SPI_I2S_IT_RXNE, ENABLE);
    SPI_I2S_ITConfig(SPI3, SPI_I2S_IT_TXE, ENABLE);
    
    
    
    // enable clock for used IO pins
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_InitStructure.NVIC_IRQChannel = SPI3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//refer platform_config.h
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    
    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // set to full duplex
    SPI_InitStruct.SPI_Mode = SPI_Mode_Slave;     // transmit in slave mode, NSS pin has
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; // one packet of data is 8 bits wide
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;        // clock is low when idle
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;      // data sampled at first edge
    SPI_InitStruct.SPI_NSS = SPI_NSS_Hard; // set the NSS HARD
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; // SPI frequency is
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;// data is transmitted MSB first
    SPI_Init(SPI3, &SPI_InitStruct);
    SPI_Cmd(SPI3, ENABLE); // enable SPI3
    
    
}
#define SPI_STATE_WAIT 0
#define SPI_STATE_SEND_WAIT 1
#define SPI_STATE_SEND_CONTENT 2
#define SPI_STATE_RECVING 3
#define SPI_SEND_HEAD 0x3E
#define SPI_SEND_NOTHING 0x8c
#define SPI_RECV_HEAD 0x1D
#define SPI_RECV_HEAD_MASTER_RECV 0XA8
#define SPI_RECV_HEAD_MASTER_SEND 0X8a

char thread_commu_read_stack[1024];
struct rt_thread thread_commu_read;

//#define COMMU_MQ_RECV_FINISH 0
//#define COMMU_MQ_SEND_FINISH 1
//#define COMMU_MQ_FLASH_WRITE 2
//#define COMMU_MQ_FLASH_READ 3
//#define COMMU_MQ_KB_WRITE 4
//#define COMMU_MQ_MOUSE_WRITE 5
//#define COMMU_MQ_TEST 6

#define SELF_STATE_IDLE 0
#define SELF_STATE_READ 1
#define SELF_STATE_WRITE 2


void commu_send(u8 *buf,u32 len){
	rt_mq_send(mq_commu_write,buf,len);
}

#define COMMU_RECV_STATE_WAIT 0
#define COMMU_RECV_STATE_RECVING 1
#define COMMU_RECV_HEAD_MASTER_RECV 'S'
#define COMMU_RECV_HEAD_MASTER_SEND 'R'
static void commu_putchar(u8 data){
	while((USART2->SR&0X40)==0);//循环发送,直到发送完毕 	
	USART2->DR =data ;
}

rt_mailbox_t mb_commu_send;

struct commu_st{
	u8 *buf;
	u8 type;
	u32 len;
	rt_mutex_t mutex;
};

void common_commu_send(u8 *buf,u32 len,u8 type){
	struct commu_st *st2send;
	rt_mutex_t mutex=rt_mutex_create("wait", RT_IPC_FLAG_FIFO);
	st2send=rt_malloc(sizeof(struct commu_st)*2);
	st2send->buf=buf;
	st2send->len=len;
	st2send->mutex=mutex;
	st2send->type=type;	
	rt_mb_send(mb_commu_send,(rt_uint32_t)st2send);
	rt_mutex_take(mutex, RT_WAITING_FOREVER);
	rt_free(st2send);
	rt_mutex_delete(mutex);
}
void rt_thread_entry_commu_send(void* parameter){
	while(1){
		struct commu_st * commu_send_st;
		rt_mb_recv(mb_commu_send,(rt_uint32_t *)&commu_send_st,RT_WAITING_FOREVER);
//		rt_kprintf("wwwwwwwwwww");
		commu_putchar(COMMU_RECV_HEAD_MASTER_RECV);
		
		u8 *buf=commu_send_st->buf;
		u32 len=commu_send_st->len;
		u8 type=commu_send_st->type;
		
		commu_putchar(len>>8);
		commu_putchar(len&0XFF);
		commu_putchar(type);
//		rt_kprintf("send len %d type %d ",len,type);
		for(u32 i=0;i<len;i++){
			commu_putchar((u8)buf[i]);
			
		}
			
		while((USART2->SR&0X40)==0);
		rt_mutex_release(commu_send_st->mutex);
	}
}

u32 test_data=0;
u8 commu_handle_buf[600];
rt_mq_t mq_commu_recv;
rt_mq_t mq_key_ms;
extern rt_mq_t mq_flash_read;
void rt_thread_entry_commu(void* parameter){
//	u8 state=SELF_STATE_IDLE;
	while(1){

		rt_err_t result=rt_mq_recv (mq_commu_recv,commu_handle_buf,600,100);
		if(!result){
			u32 len=(commu_handle_buf[0]<<8)+commu_handle_buf[1];
			u8 type=commu_handle_buf[2];
			rt_kprintf("s%d %d ",len,type);
//			for(u32 i=0;i<len;i++){
//				rt_kprintf("%c%02X",commu_handle_buf[i+3],commu_handle_buf[i+3]);   
//			}
			switch(type){
				case COMMU_TYPE(KEYBOARD_MS):
					//directly return
					rt_mq_send(mq_key_ms,commu_handle_buf+3,8);
//					common_commu_send(commu_handle_buf+3,8,COMMU_TYPE(KEYBOARD_SM));
					break;
				case COMMU_TYPE(FLASH_READ_DATA):
					rt_kprintf("flash get");
					rt_mq_send(mq_flash_read,commu_handle_buf+3,512);
////					common_commu_send(commu_handle_buf+3,8,COMMU_TYPE(KEYBOARD_SM));
					break;
			}
			rt_kprintf("\r\n");
		}
//		commu_send_data("wuwwu",5);
		

	}
}

u8 commu_state=COMMU_RECV_STATE_WAIT;
#define RECV_BUF_SIZE 600
/*
recv package format
0:COMMU_RECV_HEAD_MASTER_RECV
1:len>>8
2:len&0xff
other:data
*/
#define RECV_BUF_SIZE 600


void commu_recv_handle(u8 data){
	static u32 cnt = 0,len=0;
	static u8 recv_buf[RECV_BUF_SIZE];
	if(commu_state==COMMU_RECV_STATE_WAIT){
		if(data==COMMU_RECV_HEAD_MASTER_SEND){
			commu_state=COMMU_RECV_STATE_RECVING;
			cnt = 0;
			len=0;
		}else{
		}
	}else if(commu_state==COMMU_RECV_STATE_RECVING){
		u32 pos=cnt>RECV_BUF_SIZE+1?RECV_BUF_SIZE-1:cnt;
		recv_buf[pos]=data;
		if(cnt==0){
			len=data<<8;
		}else if(cnt==1){
			len|=data;
		}else if(cnt<len+2){
		}else if(cnt>=len+2){
			rt_mq_send(mq_commu_recv,recv_buf, len+3);
			
			commu_state=COMMU_RECV_STATE_WAIT;
		}
		
		cnt++;	
	}else{
		//do nothing		
	}
}

void SPI3_IRQHandler(void)
{
//    static u32  cnt = 0;
    if (SPI_I2S_GetITStatus(SPI3, SPI_I2S_IT_TXE) == SET)
    {
		SPI3->DR=0;
    }
//	static u32 len=0;
    if (SPI_I2S_GetITStatus(SPI3, SPI_I2S_IT_RXNE) == SET)
    {
		u8 data= SPI3->DR;

    }    
}



void blue_putchar(u8 ch);
void commu_blue_send(u8* buf,u32 lenth)
{
    u32 i=0;
    // blue_putchar(7<<4);
    for(i=0;i<lenth;i++)
    {
        blue_putchar(buf[i]);
        rt_thread_delay(1);
    }
}

void commu_recv(u8* buf,u32 lenth)
{

}

