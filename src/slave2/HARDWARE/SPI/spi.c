#include "spi.h"
#include "stm32lib.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//SPI ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/6/13 
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  
 

//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ������SD Card/W25X16/24L01/JF24C							  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI1�ĳ�ʼ��
static u8 initted=0;
void SPIx_slave_Init(void){
	
//	IOConfig(IOBB,low,PIN5,4);
//	IOConfig(IOBB,low,PIN4,0xb);
//	IOConfig(IOAB,high,PIN15,4);
//	IOConfig(IOBB,low,PIN6,3);
//	PBout(6)=0;
	RCC->APB2ENR|=1<<2;       //PORTAʱ��ʹ�� 	 
	RCC->APB2ENR|=1<<12;      //SPI1ʱ��ʹ�� 
		RCC->APB2ENR|=1<<0;     //��������ʱ��   
	//����ֻ���SPI�ڳ�ʼ��
//	GPIOA->CRL&=0X0f0FF000; 
//	GPIOA->CRL|=0XB0B00BBB;//PA5.6.7����
	IOConfig(IOAB,0,PIN5,0X0B);
	IOConfig(IOAB,0,PIN6,0X0B);
	IOConfig(IOAB,0,PIN7,0X0B);
	
		   
	AFIO->MAPR&=0XFFFFFFFE; //���MAPR��[11:10]
	AFIO->MAPR|=1<<0;      //������ӳ��,TIM3_CH2->PB5
	
	SPI1->CR1=0;
	SPI1->CR1|=0<<10;//ȫ˫��ģʽ	
	SPI1->CR1|=0<<9; //���nss����
	//SPI1->CR1|=1<<8;  

	SPI1->CR1|=0<<2; //SPI slave
	SPI1->CR1|=0<<11;//8bit���ݸ�ʽ	
	//��24L01Ҫ���� CPHA=0;CPOL=0;
	
	SPI1->CR1|=1<<1; //����ģʽ��SCKΪ1 CPOL=1			   
	SPI1->CR1|=1<<0; //��һ��ʱ�ӵ��½���,CPHA=1 CPOL=1	   
	SPI1->CR1|=0<<3; //Fsck=Fcpu/256
	SPI1->CR1|=0<<7; //MSBfirst   
	SPI1->CR2|=1<<6;  
	MY_NVIC_Init(0,0,SPI1_IRQChannel,2); 
	SPI1->CR1|=1<<6; //SPI�豸ʹ��
  SPIx_SetSpeed(SPI_SPEED_256);
}
void SPIx_Init(void)
{	 
	if(!initted){
		initted=true;
	}else{
		return;
	}
	
	RCC->APB2ENR|=1<<2;       //PORTAʱ��ʹ�� 	 
	RCC->APB2ENR|=1<<12;      //SPI1ʱ��ʹ�� 
		   
	//����ֻ���SPI�ڳ�ʼ��
//	GPIOA->CRL&=0X0f0FF000; 
//	GPIOA->CRL|=0XB0B00BBB;//PA5.6.7����
	IOConfig(IOAB,0,PIN5,0X0B);
	IOConfig(IOAB,0,PIN6,0X0B);
	IOConfig(IOAB,0,PIN7,0X0B);
	
	GPIOA->ODR|=0X7<<5;    //PA5.6.7����
	SPI1->CR1=0;	
	SPI1->CR1|=0<<10;//ȫ˫��ģʽ	
	SPI1->CR1|=1<<9; //���nss����
	SPI1->CR1|=1<<8;  

	SPI1->CR1|=1<<2; //SPI����
	SPI1->CR1|=0<<11;//8bit���ݸ�ʽ	
	//��24L01Ҫ���� CPHA=0;CPOL=0;
	SPI1->CR1|=0<<1; //CPOL=0ʱ����ģʽ��SCKΪ1 
	//SPI1->CR1|=1<<1; //����ģʽ��SCKΪ1 CPOL=1			   
	SPI1->CR1|=0<<0; //��һ��ʱ�ӵ��½���,CPHA=1 CPOL=1	   
	SPI1->CR1|=7<<3; //Fsck=Fcpu/256
	SPI1->CR1|=0<<7; //MSBfirst   
	SPI1->CR1|=1<<6; //SPI�豸ʹ��
	SPIx_ReadWriteByte(0xff);//��������		 
	
}   
//SPI �ٶ����ú���
//SpeedSet:
//SPI_SPEED_2   2��Ƶ   (SPI 36M@sys 72M)
//SPI_SPEED_8   8��Ƶ   (SPI 9M@sys 72M)
//SPI_SPEED_16  16��Ƶ  (SPI 4.5M@sys 72M)
//SPI_SPEED_256 256��Ƶ (SPI 281.25K@sys 72M)
void SPIx_SetSpeed(u8 SpeedSet)
{
	SPI1->CR1&=0XFFC7;//Fsck=Fcpu/256
	if(SpeedSet==SPI_SPEED_2)//����Ƶ
	{
		SPI1->CR1|=0<<3;//Fsck=Fpclk/2=36Mhz	
	}else if(SpeedSet==SPI_SPEED_8)//�˷�Ƶ 
	{
		SPI1->CR1|=2<<3;//Fsck=Fpclk/8=9Mhz	
	}else if(SpeedSet==SPI_SPEED_16)//ʮ����Ƶ
	{
		SPI1->CR1|=3<<3;//Fsck=Fpclk/16=4.5Mhz
	}else			 	 //256��Ƶ
	{
		SPI1->CR1|=7<<3; //Fsck=Fpclk/256=281.25Khz ����ģʽ
	}
	SPI1->CR1|=1<<6; //SPI�豸ʹ��	  
} 
//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPIx_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 
	while((SPI1->SR&1<<1)==0)//�ȴ���������	
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI1->DR=TxData;	 	  //����һ��byte 
	retry=0;
	while((SPI1->SR&1<<0)==0) //�ȴ�������һ��byte  
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI1->DR;          //�����յ�������				    
}































