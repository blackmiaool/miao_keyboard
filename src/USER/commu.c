#include "commu.h"
//#include "exti.h"
#include "usart.h"
#include "w25q16.h"
#include "stm32lib.h"
#include "stm32f10x_nvic.h"



//u8 commu_buf[550];

/************************config***********************************/
#define SPI_COMMU_CS_H do{IOout(COMMU_BASE,COMMU_PIN,1);}while(0);       //ѡ��FLASH    
#define SPI_COMMU_CS_L do{IOout(COMMU_BASE,COMMU_PIN,0);IOout(FLASH_BASE,FLASH_PIN,1);}while(0);     //ѡ��FLASH    
/************************config end***********************************/

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


 

u8 SD_ReadDisk(u8* buf,u32 sector,u32 cnt)        //����  512
{
//    printf("r%X ",sector);
    SPI_Flash_Read(buf,sector*512,cnt*512);
    return 0;
}
u8 SD_WriteDisk(u8*buf,u32 sector,u32 cnt)        //д��   512 
{
//    printf("r%X ",sector);
    SPI_Flash_Write(buf,sector*512,cnt*512);
    return 0;
}

