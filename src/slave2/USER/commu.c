#include "commu.h"
//#include "exti.h"
#include "usart.h"
#include "w25q16.h"
#include "stm32lib.h"
#include "stm32f10x_nvic.h"



u8 commu_buf[600];

/************************config***********************************/
#define SPI_COMMU_CS_H do{IOout(COMMU_BASE,COMMU_PIN,1);}while(0);   	//选中FLASH	
#define SPI_COMMU_CS_L do{IOout(COMMU_BASE,COMMU_PIN,0);IOout(FLASH_BASE,FLASH_PIN,1);}while(0); 	//选中FLASH	
/************************config end***********************************/
void commu_write(char *buf,u32 len){
//	INT_DIS();
//	SPI_COMMU_CS_L;
//	printf("len=%d ",len);
//	SPIx_ReadWriteByte(0x8a);
//	delay_us(1);
//	SPIx_ReadWriteByte(len>>8);
//	delay_us(1);
//	SPIx_ReadWriteByte(len&0xff);
//	delay_us(1);
//	for(u32 i=0;i<len;i++){
//		SPIx_ReadWriteByte(buf[i]);
//		delay_us(1);
//	}
//	SPI_COMMU_CS_H;
//	INT_EN();
}
//void commu_send(u8 *buf,u32 len){
//}
u8 *commu_read(u32 *len){
//	printf("init ");
//	while(!IOin(IRQ_BASE,IRQ_PIN));
////	printf("stage1");
//	u32 cnt=0;
//	INT_DIS();
//	SPI_COMMU_CS_L;
//	commu_buf[cnt++]=SPIx_ReadWriteByte(0xa8);
//	while(IOin(IRQ_BASE,IRQ_PIN)){
//		
//		commu_buf[cnt++]=SPIx_ReadWriteByte(0x8c);
//		if(cnt== 514)
//			cnt=513;
//		delay_us(1);
//		
//	}
//	SPI_COMMU_CS_H;
//	INT_EN();
//	long start_pos=-1;

//	for(u32 i=0;i<cnt;i++){
//		if(commu_buf[i]==0x3E){
//			start_pos=i;
//			break;
//		}
//	}
//	if(start_pos==-1){
//		*len=0;
//		printf("commu err");
//		return commu_buf;
//	}
////	printf("state2");
//	*len=(commu_buf[start_pos+1]<<8)+(commu_buf[start_pos+2]);
//	return commu_buf+start_pos+3;
}
void commu_init()
{
	SPIx_Init();
	SPIx_SetSpeed(SPI_SPEED_256);
	SCPE(PERIOA);
	IOConfig(COMMU_BASE,COMMU_PIN>7,1<<(COMMU_PIN%8*4),3);
	IOout(COMMU_BASE,COMMU_PIN,1);
	IOConfig(IRQ_BASE,IRQ_PIN>7,1<<(IRQ_PIN%8*4),8);
	IOout(IRQ_BASE,IRQ_PIN,0);
	
}


 

u8 SD_ReadDisk(u8* buf,u32 sector,u32 cnt)		//读块  512
{
//	printf("r%X ",sector);
	SPI_Flash_Read(buf,sector*512,cnt*512);
	return 0;
}
u8 SD_WriteDisk(u8*buf,u32 sector,u32 cnt)		//写块   512 
{
//	printf("r%X ",sector);
	SPI_Flash_Write(buf,sector*512,cnt*512);
	return 0;
}

