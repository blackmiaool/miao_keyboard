#ifndef _STM32LIB_H_
#define _STM32LIB_H_

#include "stm32f4xx.h"
//#include "stm32f4xx_gpio.h"




#define bool			unsigned char
#define true 			1
#define false			0
#define uint16    u16
#define uint8     u8
//#define pc(aa)			//putchar(aa) //串口输出函数,printf用
typedef long long u64;
//////////////////////////
//////////////偶邪恶的函数们////////////////
u8 SCPE(u16 PE);
u32 max(u32 *shulie ,u32 len);
enum IOtype{monishuru,shanglashuru,xialashuru,fukongshuru,kailoushuchu,tuiwanshuchu,fuyong};
void IOConfig(GPIO_TypeDef*  IOBase,u32 PIN,enum IOtype mode);	
void zhongduanConfig(u8 NVIC_Channel,int i);
void waibuzhongduan(char port,u8 pin,u8 la,u8 state);
void waibuzhongduanConfig(GPIO_TypeDef*  IOBase,u8 BITx,u8 TRIM,u8 youxianji);
void usart2putchar(char c);
void delay_s(u32 aa);
void SPIInit(void);
u8 SPIRW(u8 TxData);
void mprintf(char *shuru,...);	
void delay_ms(u16 nms);
void delay_init(u8 SYSCLK);
void delay_us(u32 nus);
u32 StringToInt(u8 * mm,u8 jinzhi);
u32 min(short *miao,u32 len);
void pc_real(u8 cc);
#define pc(a) pc_real(a)
__inline void IO1(GPIO_TypeDef *IOBase,uint16_t PIN)
{

	IOBase->BSRRL=PIN;
}

__inline void IO0(GPIO_TypeDef *IOBase,uint16_t PIN)
{
	IOBase->BSRRH=PIN;
}
/////////////外设地址定义，第二位是1，则RCC->APB2ENR2，以此类推//////////////////
#define abs(signed_data) (((signed_data)>0)?(signed_data):(-(signed_data)))
#define shangla			1
#define xiala 			2
#define fall 			1
#define raise			2	
#define high			1
#define low				0




#define PERIOA				 		  0x1000
#define PERIOB				 		  0x1001
#define PERIOC 	  		 		  0x1002
#define PERIOD				 		  0x1003
#define PERIOE				 		  0x1004
#define PERIOF				 		  0x1005
#define PERIOG				 		  0x1006
#define PERIOH				 		  0X1007
#define	PERIOI				 			0X1008
#define	PERCRC			   			0X100C
#define	PERFLITF			 			0X100F
#define	PERSRAM1			 			0X1010
#define	PERSRAM2			 			0X1011
#define	PERBKPSRAM					0X1012
#define PERCCMDATARAMEN			0X1014
#define PERDMA1			 				0X1015
#define PERDMA2					 		0X1016
#define PERETH_MAC			 		0X1015
#define PERETH_MAC_TX				0X1015
#define PERETH_MAC_RX				0X1015
#define PERETH_MAC_PTP			0X1015
#define PEROTG_HS			 			0X1015
#define PEROTG_HS_ULPI			0X1015



#define PERDCMI             0x2000
#define PERCRYP             0x2004
#define PERHASH             0x2005
#define PERRNG              0x2006
#define PEROTG_FS           0x2007


#define PERFSMC             0x3000


#define PERTIM2             0x4000
#define PERTIM3             0x4001
#define PERTIM4             0x4002
#define PERTIM5             0x4003
#define PERTIM6             0x4004
#define PERTIM7             0x4005
#define PERTIM12            0x4006
#define PERTIM13            0x4007
#define PERTIM14            0x4008
#define PERWWDG             0x400B
#define PERSPI2             0x400E
#define PERSPI3             0x400F
#define PERUSART2           0x4011
#define PERUSART3           0x4012
#define PERUART4            0x4013
#define PERUART5            0x4014
#define PERI2C1             0x4015
#define PERI2C2             0x4016
#define PERI2C3             0x4017
#define PERCAN1             0x4019
#define PERCAN2             0x401A
#define PERPWR              0x401C
#define PERDAC              0x401D

#define PERTIM1             0X5000
#define PERTIM8             0X5001
#define PERUSART1             0X5004
#define PERUSART6             0X5005
#define PERADC1             0X5008
#define PERADC2             0X5009
#define PERADC3             0X500A
#define PERSDIO             0X500B
#define PERSPI1             0X500C
#define PERSYSCFG             0X500E
#define PERTIM9             0X5010
#define PERTIM10             0X5011
#define PERTIM11             0X5012


#define IOAB			 GPIOA
#define IOBB			 GPIOB
#define IOCB			 GPIOC
#define IODB			 GPIOD
#define IOEB			 GPIOE
#define IOFB			 GPIOF
#define IOGB			 GPIOG
#define PIN0			GPIO_Pin_0
#define PIN1			GPIO_Pin_1
#define PIN2			GPIO_Pin_2
#define PIN3			GPIO_Pin_3
#define PIN4			GPIO_Pin_4
#define PIN5			GPIO_Pin_5
#define PIN6			GPIO_Pin_6
#define PIN7			GPIO_Pin_7
#define PIN8			GPIO_Pin_8
#define PIN9			GPIO_Pin_9
#define PIN10			GPIO_Pin_10
#define PIN11			GPIO_Pin_11
#define PIN12			GPIO_Pin_12
#define PIN13			GPIO_Pin_13
#define PIN14			GPIO_Pin_14
#define PIN15			GPIO_Pin_15
#define zhongduan0						EXTI0_IRQChannel
#define zhongduan1						EXTI1_IRQChannel
#define zhongduan2						EXTI2_IRQChannel
#define zhongduan3						EXTI3_IRQChannel
#define zhongduan4						EXTI4_IRQChannel
#define zhongduan5_9					EXTI9_5_IRQChannel 
#define zhongduan10_15				EXTI15_10_IRQChannel
#define zhongduanhanshu0  		EXTI0_IRQHandler
#define zhongduanhanshu1  		EXTI1_IRQHandler
#define zhongduanhanshu2  		EXTI2_IRQHandler
#define zhongduanhanshu3  		EXTI3_IRQHandler
#define zhongduanhanshu4  		EXTI4_IRQHandler
#define zhongduanhanshu5_9  	EXTI9_5_IRQHandler
#define zhongduanhanshu10_15 	EXTI15_10_IRQHandler


#define PAin(a) (IOAB->IDR&(1<<a))
#define PBin(a) (IOBB->IDR&(1<<a))
#define PCin(a) (IOCB->IDR&(1<<a))
#define PDin(a) (IODB->IDR&(1<<a))
#define PEin(a) (IOEB->IDR&(1<<a))
#define PFin(a) (IOFB->IDR&(1<<a))
#define PGin(a) (IOGB->IDR&(1<<a))
#define PHin(a) (IOHB->IDR&(1<<a))
#define PIin(a) (IOIB->IDR&(1<<a))

#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long

#endif

