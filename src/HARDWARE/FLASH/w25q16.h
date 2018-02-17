#ifndef __FLASH_H

#define __FLASH_H                

#include "sys.h" 
#include "spi.h"
#include "bsp.h"
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



//W25X系列/Q系列袩片列表

//W25Q80 ID  0XEF13

//W25Q16 ID  0XEF14

//W25Q32 ID  0XEF15

//W25Q64 ID  0XEF16    

#define W25Q80     0XEF13     

#define W25Q16     0XEF14

#define W25Q32     0XEF15

#define W25Q64     0XEF16



extern u16 SPI_FLASH_TYPE;        //稓义我们使用的flash袩片型号
//A4

                 

////////////////////////////////////////////////////////////////////////////

 

//譃令表

#define W25X_WriteEnable        0x06 

#define W25X_WriteDisable        0x04 

#define W25X_ReadStatusReg        0x05 

#define W25X_WriteStatusReg        0x01 

#define W25X_ReadData            0x03 

#define W25X_FastReadData        0x0B 

#define W25X_FastReadDual        0x3B 

#define W25X_PageProgram        0x02 

#define W25X_BlockErase            0xD8 

#define W25X_SectorErase        0x20 

#define W25X_ChipErase            0xC7 

#define W25X_PowerDown            0xB9 

#define W25X_ReleasePowerDown    0xAB 

#define W25X_DeviceID            0xAB 

#define W25X_ManufactDeviceID    0x90 

#define W25X_JedecDeviceID        0x9F 



void SPI_Flash_Init(void);

u16  SPI_Flash_ReadID(void);          //读取FLASH ID

u8     SPI_Flash_ReadSR(void);        //读取讕态屇庢器

void SPI_FLASH_Write_SR(u8 sr);      //袔讕态屇庢器

void SPI_FLASH_Write_Enable(void);  //袔使能

void SPI_FLASH_Write_Disable(void);    //袔保粈

void SPI_Flash_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);

void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //读取flash

void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//袔入flash

void SPI_Flash_Erase_Chip(void);          //整片擦除

void SPI_Flash_Erase_Sector(u32 Dst_Addr);//扇区擦除

void SPI_Flash_Wait_Busy(void);           //等廄空闲

void SPI_Flash_PowerDown(void);           //滧入掉电模蕼

void SPI_Flash_WAKEUP(void);              //粶醒
void SPI_Flash_Write_Sector(u8 *,u16);
#endif































