#include "w25q16.h" 

#include "spi.h"
#include "stm32lib.h"
#include "delay.h"





/************************config***********************************/
#define	SPI_FLASH_CS_H do{IOout(FLASH_BASE,FLASH_PIN,1);}while(0);   	//选中FLASH	
#define	SPI_FLASH_CS_L do{IOout(FLASH_BASE,FLASH_PIN,0);IOout(COMMU_BASE,COMMU_PIN,1);}while(0); 	//选中FLASH	
/************************config end***********************************/
//////////////////////////////////////////////////////////////////////////////////	 

//睙程序只供学习使用，螏煭作者许可，不得用于其它任何用蜔

//Mini STM32开发板

//W25Q64 驱动函数	   

//正点原子@ALIENTEK

//寣术论坛:www.openedv.com

//修災日期:2014/3/9 

//版睙：V1.0

//版葰所有，盗版必熆。

//Copyright(C) 正点原子 2009-2019

//All rights reserved

////////////////////////////////////////////////////////////////////////////////// 	  

 

u16 SPI_FLASH_TYPE=W25Q16;//默认熗是25Q64



//4Kbytes为一烐Sector

//16烐扇区为1烐Block

//W25Q64

//容量为8M字溭,共有128烐Block,2048烐Sector 



//初蕦化SPI FLASH的IO口

void SPI_Flash_Init(void)

{
	SPIx_Init();
	SCPE(PERIOA);
	IOConfig(IOAB,0,PIN4,3);
//	PAout(4)=1;
//	PAout(5)=0;
			   		//初蕦化SPI1

	SPIx_SetSpeed(SPI_SPEED_4);	//设置为18M时钟,炦速模蕼

	SPI_FLASH_TYPE=SPI_Flash_ReadID();//读取FLASH ID.

}  



//读取SPI_FLASH的讕态屇庢器

//BIT7  6   5   4   3   2   1   0

//SPR   RV  TB BP2 BP1 BP0 WEL BUSY

//SPR:默认0,讕态屇庢器保粈位,配合WP使用

//TB,BP2,BP1,BP0:FLASH区域袔保粈设置

//WEL:袔使能锁稓

//BUSY:脢标屒位(1,脢;0,空闲)

//默认:0x00

u8 SPI_Flash_ReadSR(void)   

{  

	u8 byte=0;   

	SPI_FLASH_CS_L;                            //使能器岨   

	SPIx_ReadWriteByte(W25X_ReadStatusReg);    //发送读取讕态屇庢器命令    

	byte=SPIx_ReadWriteByte(0Xff);             //读取一烐字溭  

	SPI_FLASH_CS_H;                            //取消片选     

	return byte;   

} 

//袔SPI_FLASH讕态屇庢器

//只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以袔!!!

void SPI_FLASH_Write_SR(u8 sr)   

{   

	SPI_FLASH_CS_L;                            //使能器岨   

	SPIx_ReadWriteByte(W25X_WriteStatusReg);   //发送袔取讕态屇庢器命令    

	SPIx_ReadWriteByte(sr);               //袔入一烐字溭  

	SPI_FLASH_CS_H;                            //取消片选     	      

}   

//SPI_FLASH袔使能	

//湯WEL置位   

void SPI_FLASH_Write_Enable(void)   

{

	SPI_FLASH_CS_L;                            //使能器岨   

    SPIx_ReadWriteByte(W25X_WriteEnable);      //发送袔使能  

	SPI_FLASH_CS_H;                            //取消片选     	      

} 

//SPI_FLASH袔滬止	

//湯WEL清零  

void SPI_FLASH_Write_Disable(void)   

{  

	SPI_FLASH_CS_L;                            //使能器岨   

    SPIx_ReadWriteByte(W25X_WriteDisable);     //发送袔滬止譃令    

	SPI_FLASH_CS_H;                            //取消片选     	      

} 			    

//读取袩片ID W25X16的ID:0XEF14

u16 SPI_Flash_ReadID(void)

{

	u16 Temp = 0;	  

	SPI_FLASH_CS_L;				    

	SPIx_ReadWriteByte(0x90);//发送读取ID命令	    

	SPIx_ReadWriteByte(0x00); 	    

	SPIx_ReadWriteByte(0x00); 	    

	SPIx_ReadWriteByte(0x00); 	
//SPIx_ReadWriteByte(0x00); 	

	Temp|=SPIx_ReadWriteByte(0xFF)<<8;  

	Temp|=SPIx_ReadWriteByte(0xFF);	 

	SPI_FLASH_CS_H;				    

	return Temp;

}   		    

//读取SPI FLASH  

//在譃稓地址开蕦读取譃稓硛度的数熭

//pBuffer:数熭庢帰区

//ReadAddr:开蕦读取的地址(24bit)

//NumByteToRead:要读取的字溭数(最庴65535)

void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   

{ 

 	u16 i;    												    

	SPI_FLASH_CS_L;                            //使能器岨   

    SPIx_ReadWriteByte(W25X_ReadData);         //发送读取命令   

    SPIx_ReadWriteByte((u8)((ReadAddr)>>16));  //发送24bit地址    

    SPIx_ReadWriteByte((u8)((ReadAddr)>>8));   

    SPIx_ReadWriteByte((u8)ReadAddr);   

    for(i=0;i<NumByteToRead;i++)

	{ 

        pBuffer[i]=SPIx_ReadWriteByte(0XFF);   //循环读数  

    }

	SPI_FLASH_CS_H;                            //取消片选     	      

}  

//SPI在一页(0~65535)内袔入少于256烐字溭的数熭

//在譃稓地址开蕦袔入最庴256字溭的数熭

//pBuffer:数熭庢帰区

//WriteAddr:开蕦袔入的地址(24bit)

//NumByteToWrite:要袔入的字溭数(最庴256),灻数不訆灻超过灻页的剩余字溭数!!!	 

void SPI_Flash_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)

{

 	u16 i;  

    SPI_FLASH_Write_Enable();                  //SET WEL 

	SPI_FLASH_CS_L;                            //使能器岨   

    SPIx_ReadWriteByte(W25X_PageProgram);      //发送袔页命令   

    SPIx_ReadWriteByte((u8)((WriteAddr)>>16)); //发送24bit地址    

    SPIx_ReadWriteByte((u8)((WriteAddr)>>8));   

    SPIx_ReadWriteByte((u8)WriteAddr);   

    for(i=0;i<NumByteToWrite;i++)SPIx_ReadWriteByte(pBuffer[i]);//循环袔数  

	SPI_FLASH_CS_H;                            //取消片选 

	SPI_Flash_Wait_Busy();					   //等廄袔入溼束

} 

//无岇验袔SPI FLASH 

//必须确保所袔的地址范围内的数熭全部为0XFF,否则在非0XFF帄袔入的数熭湯失败!

//熯有自动换页箠能 

//在譃稓地址开蕦袔入譃稓硛度的数熭,但是要确保地址不詼滅!

//pBuffer:数熭庢帰区

//WriteAddr:开蕦袔入的地址(24bit)

//NumByteToWrite:要袔入的字溭数(最庴65535)

//CHECK OK

void SPI_Flash_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   

{ 			 		 

	u16 pageremain;	   

	pageremain=256-WriteAddr%256; //单页剩余的字溭数		 	    

	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不庴于256烐字溭

	while(1)

	{	   

		SPI_Flash_Write_Page(pBuffer,WriteAddr,pageremain);

		if(NumByteToWrite==pageremain)break;//袔入溼束了

	 	else //NumByteToWrite>pageremain

		{

			pBuffer+=pageremain;

			WriteAddr+=pageremain;	



			NumByteToWrite-=pageremain;			  //岝去已煭袔入了的字溭数

			if(NumByteToWrite>256)pageremain=256; //一幬可以袔入256烐字溭

			else pageremain=NumByteToWrite; 	  //不够256烐字溭了

		}

	};	    

} 

//袔SPI FLASH  

//在譃稓地址开蕦袔入譃稓硛度的数熭

//灻函数庿擦除操作!

//pBuffer:数熭庢帰区

//WriteAddr:开蕦袔入的地址(24bit)

//NumByteToWrite:要袔入的字溭数(最庴65535)  		   

u8 SPI_FLASH_BUF[4096];

void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   

{ 

	u32 secpos;

	u16 secoff;

	u16 secremain;	   

 	u16 i;    



	secpos=WriteAddr/4096;//扇区地址  

	secoff=WriteAddr%4096;//在扇区内的偏移

	secremain=4096-secoff;//扇区剩余空屼庴小   



	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不庴于4096烐字溭

	while(1) 

	{	

		SPI_Flash_Read(SPI_FLASH_BUF,secpos*4096,4096);//读出整烐扇区的内容

		for(i=0;i<secremain;i++)//校验数熭

		{

			if(SPI_FLASH_BUF[secoff+i]!=0XFF)break;//需要擦除  	  

		}

		if(i<secremain)//需要擦除

		{

			SPI_Flash_Erase_Sector(secpos);//擦除这烐扇区

			for(i=0;i<secremain;i++)	   //瀻制

			{

				SPI_FLASH_BUF[i+secoff]=pBuffer[i];	  

			}

			SPI_Flash_Write_NoCheck(SPI_FLASH_BUF,secpos*4096,4096);//袔入整烐扇区  



		}else SPI_Flash_Write_NoCheck(pBuffer,WriteAddr,secremain);//袔已煭擦除了的,直溣袔入扇区剩余区屼. 				   

		if(NumByteToWrite==secremain)break;//袔入溼束了

		else//袔入螏溼束

		{

			secpos++;//扇区地址增1

			secoff=0;//偏移位置为0 	 



		   	pBuffer+=secremain;  //譃针偏移

			WriteAddr+=secremain;//袔地址偏移	   

		   	NumByteToWrite-=secremain;				//字溭数递岝

			if(NumByteToWrite>4096)secremain=4096;	//下一烐扇区还是袔不完

			else secremain=NumByteToWrite;			//下一烐扇区可以袔完了

		}	 

	};	 	 

}

//擦除整烐袩片

//整片擦除时屼:

//W25X16:25s 

//W25X32:40s 

//W25X64:40s 

//等廄时屼超硛...

void SPI_Flash_Erase_Chip(void)   

{                                             

    SPI_FLASH_Write_Enable();                  //SET WEL 

    SPI_Flash_Wait_Busy();   

  	SPI_FLASH_CS_L;                            //使能器岨   

    SPIx_ReadWriteByte(W25X_ChipErase);        //发送片擦除命令  

	SPI_FLASH_CS_H;                            //取消片选     	      

	SPI_Flash_Wait_Busy();   				   //等廄袩片擦除溼束

}   

//擦除一烐扇区

//Dst_Addr:扇区地址 0~2047 for W25Q64

//擦除一烐蓽区的最少时屼:150ms

void SPI_Flash_Erase_Sector(u32 Dst_Addr)   

{   

	Dst_Addr*=4096;

    SPI_FLASH_Write_Enable();                  //SET WEL 	 

    SPI_Flash_Wait_Busy();   

  	SPI_FLASH_CS_L;                            //使能器岨   

    SPIx_ReadWriteByte(W25X_SectorErase);      //发送扇区擦除譃令 

    SPIx_ReadWriteByte((u8)((Dst_Addr)>>16));  //发送24bit地址    

    SPIx_ReadWriteByte((u8)((Dst_Addr)>>8));   

    SPIx_ReadWriteByte((u8)Dst_Addr);  

	SPI_FLASH_CS_H;                            //取消片选     	      

    SPI_Flash_Wait_Busy();   				   //等廄擦除完成

}  

//等廄空闲

void SPI_Flash_Wait_Busy(void)   

{   

	while ((SPI_Flash_ReadSR()&0x01)==0x01);   // 等廄BUSY位清空

}  

//滧入掉电模蕼

void SPI_Flash_PowerDown(void)   

{ 

  	SPI_FLASH_CS_L;                            //使能器岨   

    SPIx_ReadWriteByte(W25X_PowerDown);        //发送掉电命令  

	SPI_FLASH_CS_H;                            //取消片选     	      

    delay_us(3);                               //等廄TPD  

}   

//粶醒

void SPI_Flash_WAKEUP(void)   

{  

  	SPI_FLASH_CS_L;                            //使能器岨   

    SPIx_ReadWriteByte(W25X_ReleasePowerDown); //  send W25X_PowerDown command 0xAB    

	SPI_FLASH_CS_H;                            //取消片选     	      

    delay_us(3);                               //等廄TRES1

}   



















































