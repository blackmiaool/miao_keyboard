#include "commu.h"
//#include "exti.h"
#include "usart.h"
u32 commu_lenth=0;
u8 commu_buf[50][20];
 u8 commu_flag=2;//2:idle 1:finish 0:ing
u32 sector_addr;
u8* sector_buf;
#define DATA_GET (GPIOB->IDR&0XFF)
#define DATA_PUT(A) do{GPIOB->ODR&=0XFF00;GPIOB->ODR|=(A&0xff);}while(0)
#define CLK_IN (PBin(9))
#define FINISH_FLAG_H (PBout(8)=1)
#define FINISH_FLAG_L (PBout(8)=0)
#define FLASH_IRQ_H (PBout(6)=1)//EN
#define FLASH_IRQ_L (PBout(6)=0)
u8 ptr_write=0;
u8 ptr_read=0;
volatile u8 commuing_flag=0;

void USART1_IRQHandler(void)
{
//	u8 res;	
//	
	if(USART1->SR&(1<<5));//接收到数据
//	{	 
//		res=USART1->DR; 
//  	
//		if(commu_flag==2)
//		{
//			if(res=='s')
//			{
//				commu_flag=0;
//			}
//		}
//		else 
//		{
//			if(commu_lenth==8)
//			{
//				commu_buf[commu_lenth++]=res;
//				commu_flag=1;
//			}
//			else
//			{
//				commu_buf[commu_lenth++]=res;
//			}
//			
//			
//		}
//	}
	//putchar('s');
} 
u8 Slave_Temp;
void commu_Init()
{
//	SCPE(PERIOB);
//	SCPE(PERIOA);


//	IOConfig(IOBB,high,PIN8,3);
//	IOConfig(IOBB,high,PIN9,8);	
//	IOConfig(IOAB,high,PIN10,3);	
//	GPIOB->ODR=0X00ff;	

//	FLASH_IRQ_L;

//	Ex_NVIC_Config(GPIO_B,9,FTIR); //?????
	
//    MY_NVIC_Init(0,1,EXTI9_5_IRQChannel,2);//??2,????1,?2

	SCPE(PERIOB);
	SCPE(PERSPI1);
	SCPE(PERIOA);
	IOConfig(IOBB,low,PIN3,4);
	IOConfig(IOBB,low,PIN5,4);
	IOConfig(IOBB,low,PIN4,0xb);
	IOConfig(IOAB,high,PIN15,4);
	IOConfig(IOBB,low,PIN6,3);
	PBout(6)=0;
//	  //????????1,??????1,?????4
	RCC->APB2ENR|=1<<0;     //开启辅助时钟	   
	AFIO->MAPR&=0XFFFFFFFE; //清除MAPR的[11:10]
	AFIO->MAPR|=1<<0;      //部分重映像,TIM3_CH2->PB5
	
	SPI1->CR1|=0<<10;//全双工模式	
	SPI1->CR1|=0<<9; //软件nss管理
	//SPI1->CR1|=1<<8;  

	SPI1->CR1&=(0xffff-(1<<2)); //SPI主机
	SPI1->CR1|=0<<11;//8bit数据格式	
	//对24L01要设置 CPHA=0;CPOL=0;
	
	SPI1->CR1|=1<<1; //空闲模式下SCK为1 CPOL=1			   
	SPI1->CR1|=1<<0; //第一个时钟的下降沿,CPHA=1 CPOL=1	   
	SPI1->CR1|=0<<3; //Fsck=Fcpu/256
	SPI1->CR1|=0<<7; //MSBfirst   
	 SPI1->CR2|=1<<6;  
	 MY_NVIC_Init(0,0,SPI1_IRQChannel,2); 
	SPI1->CR1|=1<<6; //SPI设备使能
	//PBout(4)=1;
	 //SPI1_SetSpeed(SPI_SPEED_256);

	
}
u8 SPI1_ReadWriteByte(u8 TxData)
{  
     u8 retry=0;  
SPI1->DR=TxData;     //????byte 	 
     while((SPI1->SR&1<<1)==0)//?????? 
     {
          retry++;
          if(retry>200)return 0;
     }     
     
     retry=0;
//     while((SPI1->SR&1<<0)==0) //???????byte  
//     {
//          retry++;
//          if(retry>200)return 0;
//     }             
     return SPI1->DR;          //???????        
}
u8 SPI1_ReadByte(u8 TxData)
{  
     int retry=0;     
	u8 data=0;

  
     while((SPI1->SR&1<<0)==0) //???????byte  
     {
          retry++;
          if(retry>2000)return 0;
     }  
	data= SPI1->DR;	
	while((SPI1->SR&1<<1)==0)//?????? 
     {
          retry++;
          if(retry>2000)return 0;
     } 
	SPI1->DR=TxData; 
     while((SPI1->SR&1<<1)==2)//?????? 
     {
          retry++;
          if(retry>2000)return 0;
     }  	
    
   
     
     retry=0;
            
     return data;         
}
u8 data_process(u8 data);
static u8 next_data=0;
void SPI1_IRQHandler(void)
{
//	while(1);
     if((SPI1->SR&1<<0)==1) 
     {     
		 
          Slave_Temp = SPI1_ReadByte(next_data); 
		 
     //     SPI1_ReadWriteByte(next_data); 
		  next_data=data_process(Slave_Temp);
		  
     }  
}
 int flash_finish=0;
u8 data_process(u8 data)
{
	static u8 recv_flag=0;
//	u8 i=0;
//	u8 j=0;
	 
start:	
//	putchar(recv_flag);
//	putchar(data);
	switch(recv_flag)
	{
	case 0:
	{		
		if(data==(7<<4))//in
		{
			recv_flag=1;
			
		}
		else if(data==(9<<4))//out
		{
			recv_flag=2;	
			goto start;
		}
		else if(data==(0xb0))//in512
		{
			recv_flag=3;
			//goto start;
		}
		else if(data==(12<<4))//out512
		{
			recv_flag=4;
			goto start;			
		}
		else
		{
			recv_flag=0;	
		}
		
		break;
		
	}	
	case 1:
	{
		static int i=0;
		//printf("%c",i);
		if(i>=8)
		{
			commu_buf[ptr_write][i]=data;
			
			i=0;
			if((ptr_write+1)%50==ptr_read)
			{
				//do nothing
			}
			else
			{
				ptr_write=(ptr_write+1)%50;
			}
			recv_flag=0;
			//printf("mouse_done");
		}
		else
		{
			commu_buf[ptr_write][i]=data;
			i++;
		}

		break;
	}
	case 2:
	{
		static int i=0;
//		putchar((*(((u8*)&sector_addr)+i)));
		if(i>=4)
		{
			recv_flag=0;
			i=0;
			return (*(((u8*)&sector_addr)+3));
			
		}
		else
		{
			return (*(((u8*)&sector_addr)+(i++)));
		}
//		break;
	}
	case 3:
	{
		static int i=0;
		sector_buf[i++]=data;
		if(i>=512)
		{
			i=0;
			recv_flag=0;
			flash_finish=1;			
		}

		break;
	}
	case 4:
	{
		static int i=0;
		
		if(i>=513)
		{
			i=0;
			recv_flag=0;	
			flash_finish=1;
		}
		else
			i++;
		return sector_buf[i-1];
	}
	default:
	{
		
	}
	}
	return 0;	
}

void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   //读取flash
{

}
void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)//写入flash
{
	
}
u8 SD_ReadDisk(u8*buf,u32 sector,u8 cnt)		//读块
{
//	u32 i=0;
	
//	printf("R%d,%d",sector,cnt);
	sector_addr=sector;
	
	sector_buf=buf;
	//putchar((u8)sector_addr);

	FLASH_IRQ_H;
	while(flash_finish==0);
		flash_finish=0;
	FLASH_IRQ_L;
//	printf("r=%d,%d,%d\r\n",sector,buf[0],cnt);
	
	//delay_us(10);
	return 0;
	
}
u8 SD_WriteDisk(u8*buf,u32 sector,u8 cnt)		//写块
{
	sector_addr=sector+1000000;
//	sector_buf=buf;
//	commuing_flag=0;
//	FLASH_IRQ_H;
//	while(commuing_flag==0);
//	while(commuing_flag==1);
//	
//	FLASH_IRQ_H;
//	while(commuing_flag==0);
//	while(commuing_flag==1);
//	FLASH_IRQ_L;
//	printf("w");
//	delay_us(300);

	
	sector_buf=buf;
	//putchar((u8)sector_addr);
	FLASH_IRQ_H;
	while(flash_finish==0);
		flash_finish=0;
	FLASH_IRQ_L;
	//printf("r");
	
	//delay_us(10);
	return 0;

}

