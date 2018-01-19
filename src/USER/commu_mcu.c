#include "usart.h"
#include "w25q16.h"
#include "stm32lib.h"
#include "stm32f10x_nvic.h"
#include "commu_mcu.h"

#define COMMU_RECV_HEAD_MASTER_RECV 'S'
#define COMMU_RECV_HEAD_MASTER_SEND 'R'


static void  usart_putchar(char ch)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕
	USART1->DR = (u8) ch;      
}
void commu_send(u8 *buf,u32 len,u8 type){
	usart_putchar(COMMU_RECV_HEAD_MASTER_SEND);
	usart_putchar(len>>8);
	usart_putchar(len&0xFF);
	usart_putchar(type);
	for(u32 i=0;i<len;i++){
		usart_putchar(buf[i]);
	}
}
//void keyboard_send(key_t key_buf){
//	u8 buf[9];
//	buf[0]=0;
//	buf[1]=(key_buf.L_CTRL<<0)+(key_buf.L_SHIFT<<1)+(key_buf.L_ALT<<2)+(key_buf.L_GUI<<3)+(key_buf.R_CTRL<<4)+(key_buf.R_SHIFT<<5)+(key_buf.R_ALT<<6)+(key_buf.R_GUI<<7);
//	buf[2]=0;
//	for(u8 i=0;i<6;i++){
//		buf[3+i]=key_buf.key[i];
//	}
//	
//	keyboard_process(buf);
//}
__asm void INT_DIS(void) 
{
  CPSID f
  BX r14 

} 
/*************????************************/
__asm void INT_EN(void)
{
  CPSIE f
  BX r14
}
volatile u8 keyboard_flag=0;
u8 buf_send[600];
u8 buf_key[9];
void commu_recv(u8 *buf,u32 len,u8 type){
//	printf("got%d\r\n",len);
	INT_DIS();
	switch(type){
		case COMMU_TYPE(KEYBOARD_SM):
		{			
			buf_key[0]=0;//keyboard head
			for(u8 i=0;i<8;i++){
				buf_key[1+i]=buf[i];
//				printf("%X",buf[i+1]);
			}
			keyboard_flag=true;
				
//			keyboard_process(buf_send);
			break;
		}
		case COMMU_TYPE(FLASH_READ_CMD):
		{
			
			u8 action_type=buf[0];
			action_type=action_type;
			u32 sector=(buf[1]<<24)+(buf[2]<<16)+(buf[3]<<8)+(buf[4]);
			u16 cnt=(buf[5]<<8)+buf[6];
			printf("disk %d %d \r\n",sector,cnt);
			SPI_Flash_Read(buf_send,sector*512,cnt*512);
			commu_send(buf_send,512,COMMU_TYPE(FLASH_READ_DATA));
			break;
		}
		
	}	
	INT_EN();
}


#define COMMU_RECV_STATE_WAIT 0
#define COMMU_RECV_STATE_RECVING 1
#define COMMU_RECV_HEAD_MASTER_RECV 'S'
#define COMMU_RECV_HEAD_MASTER_SEND 'R'
u8 commu_state=COMMU_RECV_STATE_WAIT;
#define RECV_BUF_SIZE 600


static void commu_recv_handle(u8 data){
	static u8 recv_buf[RECV_BUF_SIZE];
	static u32 cnt = 0,len=0;
	if(commu_state==COMMU_RECV_STATE_WAIT){
		if(data==COMMU_RECV_HEAD_MASTER_RECV){
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
			u8 type=recv_buf[2];
			commu_recv(recv_buf+3, len,type);
			commu_state=COMMU_RECV_STATE_WAIT;
		}
		
		cnt++;	
	}else{
		//do nothing		
	}
}

void USART1_IRQHandler(void)
{
	if(USART1->SR&(1<<5))//接收到数据
	{	 
		u8 res=USART1->DR; 
		commu_recv_handle(res);
	}
}








