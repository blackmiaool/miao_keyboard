#include "spi.h"
#include "stm32lib.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//SPI 驱动函数
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/6/13
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  
 

//以下是SPI模块的初始化代码，配置成主机模式，访问SD Card/W25X16/24L01/JF24C
//SPI口初始化
//这里针是对SPI1的初始化
static u8 initted=0;
void SPIx_slave_Init(void){
	
//	IOConfig(IOBB,low,PIN5,4);
//	IOConfig(IOBB,low,PIN4,0xb);
//	IOConfig(IOAB,high,PIN15,4);
//	IOConfig(IOBB,low,PIN6,3);
//	PBout(6)=0;
	RCC->APB2ENR|=1<<2;       //PORTA时钟使能
	RCC->APB2ENR|=1<<12;      //SPI1时钟使能
		RCC->APB2ENR|=1<<0;     //开启辅助时钟
	//这里只针对SPI口初始化
//	GPIOA->CRL&=0X0f0FF000; 
//	GPIOA->CRL|=0XB0B00BBB;//PA5.6.7复用
	IOConfig(IOAB,0,PIN5,0X0B);
	IOConfig(IOAB,0,PIN6,0X0B);
	IOConfig(IOAB,0,PIN7,0X0B);
	
		   
	AFIO->MAPR&=0XFFFFFFFE; //清除MAPR的[11:10]
	AFIO->MAPR|=1<<0;      //部分重映像,TIM3_CH2->PB5
	
	SPI1->CR1=0;
	SPI1->CR1|=0<<10;//全双工模式
	SPI1->CR1|=0<<9; //软件nss管理
	//SPI1->CR1|=1<<8;  

	SPI1->CR1|=0<<2; //SPI slave
	SPI1->CR1|=0<<11;//8bit数据格式
	//对24L01要设置 CPHA=0;CPOL=0;
	
	SPI1->CR1|=1<<1; //空闲模式下SCK为1 CPOL=1
	SPI1->CR1|=1<<0; //第一个时钟的下降沿,CPHA=1 CPOL=1
	SPI1->CR1|=0<<3; //Fsck=Fcpu/256
	SPI1->CR1|=0<<7; //MSBfirst   
	SPI1->CR2|=1<<6;  
	MY_NVIC_Init(0,0,SPI1_IRQChannel,2); 
	SPI1->CR1|=1<<6; //SPI设备使能
  SPIx_SetSpeed(SPI_SPEED_256);
}
void SPIx_Init(void)
{	 
	if(!initted){
		initted=true;
	}else{
		return;
	}
	
	RCC->APB2ENR|=1<<2;       //PORTA时钟使能
	RCC->APB2ENR|=1<<12;      //SPI1时钟使能
		   
	//这里只针对SPI口初始化
//	GPIOA->CRL&=0X0f0FF000; 
//	GPIOA->CRL|=0XB0B00BBB;//PA5.6.7复用
	IOConfig(IOAB,0,PIN5,0X0B);
	IOConfig(IOAB,0,PIN6,0X0B);
	IOConfig(IOAB,0,PIN7,0X0B);
	
	GPIOA->ODR|=0X7<<5;    //PA5.6.7上拉
	SPI1->CR1=0;	
	SPI1->CR1|=0<<10;//全双工模式
	SPI1->CR1|=1<<9; //软件nss管理
	SPI1->CR1|=1<<8;  

	SPI1->CR1|=1<<2; //SPI主机
	SPI1->CR1|=0<<11;//8bit数据格式
	//对24L01要设置 CPHA=0;CPOL=0;
	SPI1->CR1|=0<<1; //CPOL=0时空闲模式下SCK为1
	//SPI1->CR1|=1<<1; //空闲模式下SCK为1 CPOL=1
	SPI1->CR1|=0<<0; //第一个时钟的下降沿,CPHA=1 CPOL=1
	SPI1->CR1|=7<<3; //Fsck=Fcpu/256
	SPI1->CR1|=0<<7; //MSBfirst   
	SPI1->CR1|=1<<6; //SPI设备使能
	SPIx_ReadWriteByte(0xff);//启动传输
	
}   
//SPI 速度设置函数
//SpeedSet:
//SPI_SPEED_2   2分频   (SPI 36M@sys 72M)
//SPI_SPEED_8   8分频   (SPI 9M@sys 72M)
//SPI_SPEED_16  16分频  (SPI 4.5M@sys 72M)
//SPI_SPEED_256 256分频 (SPI 281.25K@sys 72M)
void SPIx_SetSpeed(u8 SpeedSet)
{
	SPI1->CR1&=0XFFC7;//Fsck=Fcpu/256
	if(SpeedSet==SPI_SPEED_2)//二分频
	{
		SPI1->CR1|=0<<3;//Fsck=Fpclk/2=36Mhz	
	}else if(SpeedSet==SPI_SPEED_8)//八分频
	{
		SPI1->CR1|=2<<3;//Fsck=Fpclk/8=9Mhz	
	}else if(SpeedSet==SPI_SPEED_16)//十六分频
	{
		SPI1->CR1|=3<<3;//Fsck=Fpclk/16=4.5Mhz
	}else			 	 //256分频
	{
		SPI1->CR1|=7<<3; //Fsck=Fpclk/256=281.25Khz 低速模式
	}
	SPI1->CR1|=1<<6; //SPI设备使能
} 
//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPIx_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 
	while((SPI1->SR&1<<1)==0)//等待发送区空
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI1->DR=TxData;	 	  //发送一个byte
	retry=0;
	while((SPI1->SR&1<<0)==0) //等待接收完一个byte
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI1->DR;          //返回收到的数据
}































