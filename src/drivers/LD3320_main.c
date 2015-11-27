#include "LDchip.h"
#include "Reg_RW.h"
#include <rtthread.h>
#include "debug.h"
#include "usbh_hid_mouse.h"

/************************************************************************************
//	nAsrStatus 用来在main主程序中表示程序运行的状态，不是LD3320芯片内部的状态寄存器
//	LD_ASR_NONE:		表示没有在作ASR识别
//	LD_ASR_RUNING：		表示LD3320正在作ASR识别中
//	LD_ASR_FOUNDOK:		表示一次识别流程结束后，有一个识别结果
//	LD_ASR_FOUNDZERO:	表示一次识别流程结束后，没有识别结果
//	LD_ASR_ERROR:		表示一次识别流程中LD3320芯片内部出现不正确的状态
*********************************************************************************/
	uint8 nDemoFlag=0;
	uint8 nAsrStatus=0;
	uint8 nAsrRes;
	uint8 end_loop=0;
	u8 isplaying=0;
	extern void (*exe_func)(void);

uint8 RunASR(void);
void ProcessInt0(void);
void LD3320_EXTI_Cfg(void);
void LD3320_Spi_cfg(void);
void LD3320_GPIO_Cfg(void);
void LED_gpio_cfg(void);
#define toString(a) #a
/***********************************************************
* 名    称： LD3320_
*
* (void)
* 功    能： 主函数LD3320程序入口
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
mouse_data ms_data;
extern rt_mq_t mq_commu;
void mouse_data_clear()
{
    ms_data.button=0;
    ms_data.x=0;
    ms_data.y=0;
    ms_data.z=0;
}
    
void LD_loop()
{
    u8 mail[9];
	u8 i=0;
loop:
    if(nAsrStatus!=LD_ASR_RUNING)
    {
        ld3320_print("***%d****",nAsrStatus);
        if(nAsrStatus==0x31)
        rt_thread_delay(1000);
    }
        
    switch(nAsrStatus)
    {
        case LD_ASR_RUNING:
        case LD_ASR_ERROR:
            break;
        case LD_ASR_NONE:
            nAsrStatus=LD_ASR_RUNING;
            if (RunASR()==0)	//	启动一次ASR识别流程：ASR初始化，ASR添加关键词语，启动ASR运算
            {
                nAsrStatus = LD_ASR_ERROR;
                ld3320_print("init_false");
            }
            break;
        case LD_ASR_FOUNDOK:
            nAsrRes = LD_ReadReg(0xc5);	//	一次ASR识别流程结束，去取ASR最佳识别结果
            ld3320_print("\r\n识别码:");			 /*text.....*/
            putchar(nAsrRes+0x30); /*text.....*/
            mail[i]=0; 
            switch(nAsrRes)		   /*对结果执行相关操作,客户修改*/
            {
                #define clear_mail for(i=0;i<9;i++){mail[i]=0;}
                case CODE_YMD:
                {
                    ld3320_print("“xiaoxi识别成功\r\n"); 
                    clear_mail;	
                    mail[1]=1;
                    rt_mq_send (mq_commu, mail,9);
                    clear_mail;	
                    mail[1]=3;
                    rt_mq_send (mq_commu, mail,9);
                    clear_mail;	
                    mail[1]=3;
                    mail[3]=38;
                    rt_mq_send (mq_commu, mail,9);
                    clear_mail;	

                    rt_mq_send (mq_commu, mail,9);
                    break;
                }
                case CODE_QJ:			/*命令“前进”*/
                    ld3320_print("“前进”命令识别成功\r\n"); /*text.....*/
                    mail[0]=11;
                    mouse_data_clear();
                    ms_data.z=-3;
                    (*(mouse_data*)&mail[1])=ms_data;
                    rt_mq_send (mq_commu, mail,9);
                break;
                //case CODE_SK:
                case CODE_HT:	 /*命令“后退”*/
                    ld3320_print("“后退”命令识别成功\r\n"); /*text.....*/
                    mail[0]=11;
                    mouse_data_clear();
                    ms_data.z=3;
                    (*(mouse_data*)&mail[1])=ms_data;
                    rt_mq_send (mq_commu, mail,9);
                    // houtui();
                    break;
                case CODE_KNM:		/*命令“举手”*/
                {
                    ld3320_print("看你妹命令识别成功\r\n"); /*text.....*/
                    for(i=0;i<9;i++)
                    {
                        mail[i]=0;
                    }	
                    mail[1]=8;
                    rt_mq_send (mq_commu, mail,9);
                    for(i=0;i<9;i++)
                    {
                        mail[i]=0;
                    }	
                    mail[1]=8;
                    mail[3]=7;
                    rt_mq_send (mq_commu, mail,9);
                    for(i=0;i<9;i++)
                    {
                        mail[i]=0;
                    }	
                    mail[1]=8;
                    rt_mq_send (mq_commu, mail,9);
                    for(i=0;i<9;i++)
                    {
                        mail[i]=0;
                    }	
                    rt_mq_send (mq_commu, mail,9);

                //	Key_LED();								 zarien...
                    break;
                }
                case CODE_A:
                {
                    ld3320_print(toString(CODE_A)"命令识别成功\r\n");
                    break;
                }
                case CODE_B:
                {
                    ld3320_print(toString(CODE_B)"命令识别成功\r\n");
                    break;
                }
                case CODE_C:
                {
                    ld3320_print(toString(CODE_C)"命令识别成功\r\n");
                    break;
                }
                case CODE_D:
                {
                    ld3320_print(toString(CODE_D)"命令识别成功\r\n");
                    break;
                }
                case CODE_E:
                {
                    ld3320_print(toString(CODE_E)"命令识别成功\r\n");
                    break;
                }
                case CODE_F:
                {
                    ld3320_print(toString(CODE_F)"命令识别成功\r\n");
                    break;
                }
                case CODE_G:
                {
                    ld3320_print(toString(CODE_G)"命令识别成功\r\n");
                    break;
                }

                default:break;
            }
            nAsrStatus = LD_ASR_NONE;
            break;

        case LD_ASR_FOUNDZERO:
        default:
            nAsrStatus = LD_ASR_NONE;
            break;
    }
    if(nAsrStatus!=LD_ASR_RUNING)
        goto loop;
}

#define PrintCom(A,B) printf(B)
#define Delay(A) delay_us2(A)
void end_handle(void);
extern struct sttsound sound[35];
void delay_ms2(u32 ms);
void  LD3320_main_Init(void)
{
	nAsrStatus=0;
	LD3320_GPIO_Cfg();	
	LD3320_EXTI_Cfg();
	LD_reset();
    end_loop=0;
	LD_reset();
	LD_reset();
    rt_thread_delay(100);
	nAsrStatus = LD_ASR_NONE;		//	初始状态：没有在作ASR
    LD_reset();
    rt_thread_delay(60);
    
}
uint8 RunASR(void)
{
	uint8 i=0;
	uint8 asrflag=0;
	delay_ms2(10);

	DBG("\r\nstart_init");
	for (i=0; i<5; i++)			//	防止由于硬件原因导致LD3320芯片工作不正常，所以一共尝试5次启动ASR识别流程
	{
		LD_AsrStart();			//初始化ASR
		rt_thread_delay(10);
		if (LD_AsrAddFixed()==0)	//添加关键词语到LD3320芯片中
		{
			ld3320_print("\r\nerror0");
			LD_reset();			//	LD3320芯片内部出现不正常，立即重启LD3320芯片
			rt_thread_delay(10);			//	并从初始化开始重新ASR识别流程
			continue;
		}

		rt_thread_delay(10);
		
		if (LD_AsrRun() == 0)
		{
			DBG("\r\nerror1");
			LD_reset();			//	LD3320芯片内部出现不正常，立即重启LD3320芯片
			rt_thread_delay(10);			//	并从初始化开始重新ASR识别流程
			continue;
		}

		asrflag=1;
		break;					//	ASR流程启动成功，退出当前for循环。开始等待LD3320送出的中断信号
	}

	return asrflag;
}

/***********************************************************
* 名    称：LD3320_GPIO_Cfg(void)
* 功    能：初始化需要用到的IO口
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
*
*
**********************************************************/ 
extern rt_sem_t sem_ld3320;
void LD3320_GPIO_Cfg(void)
{	
    SCPE(PERIOC);
    SCPE(PERIOA);
    IOConfig(IOCB,PIN1|PIN3,tuiwanshuchu);
    IOConfig(IOCB,PIN2,fukongshuru);
    IOConfig(IOAB,PIN6,fukongshuru);
    LD_CS_H();
}
/***********************************************************
* 名    称：LD3320_Spi_cfg(void)
* 功    能：配置SPI功能和端口初始化
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void LD3320_Spi_cfg(void)
{
//	SPI_InitTypeDef  SPI_InitStructure;
//	GPIO_InitTypeDef GPIO_InitStructure;
//  //spi端口配置
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 | RCC_APB2Periph_GPIOB,ENABLE);	   //使能SPI2外设时钟
//  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 

//	//P0/P1/P2
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);

////	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;			//spis 片选	WR		 zarien...
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;						 zarien...
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;						 zarien...
////	GPIO_Init(GPIOB, &GPIO_InitStructure);									 zarien...
//  
//	LD_CS_H();
//	
//	//spi功能配置
//	SPI_Cmd(SPI2, DISABLE);
//	/* SPI2 配置 */
//	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //全双工
//	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						   //主模式
//	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					   //8位
//	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;						   //时钟极性 空闲状态时，SCK保持低电平
//	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						   //时钟相位 数据采样从第一个时钟边沿开始
//	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							   //软件产生NSS
//	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;   //波特率控制 SYSCLK/128
//	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   //数据高位在前
//	SPI_InitStructure.SPI_CRCPolynomial = 7;							   //CRC多项式寄存器初始值为7
//	SPI_Init(SPI2, &SPI_InitStructure);
//	/* 使能SPI2 */
//	SPI_Cmd(SPI2, ENABLE);

}
/***********************************************************
* 名    称： LD3320_EXTI_Cfg(void) 
* 功    能： 外部中断功能配置和相关端口配置
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void LD3320_EXTI_Cfg(void)
{
//  EXTI_InitTypeDef EXTI_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//  GPIO_InitTypeDef GPIO_InitStructure;

//  //中断引脚配置
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_11;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
//	//外部中断线配置 
//  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);
//  EXTI_InitStructure.EXTI_Line = EXTI_Line11;
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  EXTI_InitStructure.EXTI_Trigger =EXTI_Trigger_Falling;
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);
//  EXTI_GenerateSWInterrupt(EXTI_Line11);
//		
//	GPIO_SetBits(GPIOB,GPIO_Pin_11);	 //默认拉高中断引脚

//	EXTI_ClearFlag(EXTI_Line11);
//	EXTI_ClearITPendingBit(EXTI_Line11);
//	//中断嵌套配置
//  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure)                                                                             ;
}


/***********************************************************
* 名    称：  EXTI1_IRQHandler(void)
* 功    能： 外部中断函数
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 

void EXTI15_10_IRQHandler(void)
{

	if(EXTI_GetITStatus(EXTI_Line11)!= RESET ) 
	{
 		if(isplaying==0)
		{
			ProcessInt0(); 
		printf("进入中断\r\n");	
		}
		/*text........................*/
		EXTI_ClearFlag(EXTI_Line11);
        EXTI_ClearITPendingBit(EXTI_Line11);
        LD_loop();
		//end_loop=1;
	}
	
}
/***********************************************************
* 名    称：void LED_gpio_cfg(void)
* 功    能：LED端口配置
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
#include "LDchip.h"
#include "Reg_RW.h"
#include "usart.h"
#include "debug.h"

uint32 nMp3StartPos=0; //	 zarien...
uint32 nMp3Size=0;	   //	 zarien...
uint32 nMp3Pos=0;	   //	 zarien...
uint32 nCurMp3Pos=0;   //	 zarien...

uint8 nLD_Mode = LD_MODE_IDLE;		//	用来记录当前是在进行ASR识别还是在播放MP3

//********************************************************************************
uint8 bMp3Play=0;		//	用来记录播放MP3的状态			  zarien...
//uint8 idata ucRegVal;	 //
uint8  ucHighInt;	 //
uint8  ucLowInt;	 //
uint8  ucStatus;	 //
uint8  ucSPVol=15; // MAX=15 MIN=0		//	Speaker喇叭输出的音量
//*********************************************************************************
uint8 ucRegVal;
extern uint8  nAsrStatus;
extern uint8  nDemoFlag;  //zarien...
extern u8 isplaying;

/***********************************************************
* 名    称：void LD_reset(void)
* 功    能：LD芯片硬件初始化
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
#define LD_RST_H() IO1(IOCB,PIN3)
#define LD_RST_L() IO0(IOCB,PIN3)
void LD_reset(void)
{
	LD_RST_H();		//		zarien...
	rt_thread_delay(2);	//	zarien...
	LD_RST_L();			//	zarien...
	rt_thread_delay(2);//		zarien...
	LD_RST_H();		///		zarien...
	rt_thread_delay(2);	//	zarien...
	LD_CS_L();
	rt_thread_delay(2);
	LD_CS_H();		
	rt_thread_delay(2);
}

/***********************************************************
* 名    称： void LD_Init_Common(void)
* 功    能： 初始化命令
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void LD_Init_Common(void)
{

	LD_ReadReg(0x06);  
	LD_WriteReg(0x17, 0x35); 
	rt_thread_delay(5);
	LD_ReadReg(0x06);  

	LD_WriteReg(0x89, 0x03);  
	rt_thread_delay(5);
	LD_WriteReg(0xCF, 0x43);   
	rt_thread_delay(5);
	LD_WriteReg(0xCB, 0x02);
	
	/*PLL setting*/
	LD_WriteReg(0x11, LD_PLL_11);       
	if (nLD_Mode == LD_MODE_MP3)
	{
		LD_WriteReg(0x1E, 0x00); 
		LD_WriteReg(0x19, LD_PLL_MP3_19);   
		LD_WriteReg(0x1B, LD_PLL_MP3_1B);   
		LD_WriteReg(0x1D, LD_PLL_MP3_1D);
	}
	else
	{
		LD_WriteReg(0x1E,0x00);
		LD_WriteReg(0x19, LD_PLL_ASR_19); 
		LD_WriteReg(0x1B, LD_PLL_ASR_1B);		
	    LD_WriteReg(0x1D, LD_PLL_ASR_1D);
	}
	rt_thread_delay(5);
	
	LD_WriteReg(0xCD, 0x04);
	LD_WriteReg(0x17, 0x4c); 
	rt_thread_delay(1);
	LD_WriteReg(0xB9, 0x00);
	LD_WriteReg(0xCF, 0x4F); 
	LD_WriteReg(0x6F, 0xFF); 
}

void LD_Init_ASR(void)
{
	nLD_Mode=LD_MODE_ASR_RUN;
	LD_Init_Common();

	LD_WriteReg(0xBD, 0x00);
	LD_WriteReg(0x17, 0x48);	
	rt_thread_delay( 5 );
	LD_WriteReg(0x3C, 0x80);    
	LD_WriteReg(0x3E, 0x07);
	LD_WriteReg(0x38, 0xff);    
	LD_WriteReg(0x3A, 0x07);
	LD_WriteReg(0x40, 0);          
	LD_WriteReg(0x42, 8);
	LD_WriteReg(0x44, 0);    
	LD_WriteReg(0x46, 8); 
	rt_thread_delay( 1 );
}

/***********************************************************
* 名    称：void ProcessInt0(void)
* 功    能：识别处理函数
* 入口参数：  
* 出口参数：
* 说    明：可以利用查询方式，或中断方式进行调用
* 调用方法： 
**********************************************************/ 
void ProcessInt0(void)
{
    uint8 nAsrResCount=0;

    ucRegVal = LD_ReadReg(0x2B);

    // 语音识别产生的中断
    // （有声音输入，不论识别成功或失败都有中断）
    if(nLD_Mode == LD_MODE_ASR_RUN)		 //zaien...添加判断条件
    {
        LD_WriteReg(0x29,0) ;
        LD_WriteReg(0x02,0) ;
        
        if((ucRegVal & 0x10) && LD_ReadReg(0xb2)==0x21 && LD_ReadReg(0xbf)==0x35)		
        {	 
            nAsrResCount = LD_ReadReg(0xba);
            
            if(nAsrResCount>0 && nAsrResCount<=4) 
            {
                nAsrStatus=LD_ASR_FOUNDOK; 				
            }
            else
            {
                nAsrStatus=LD_ASR_FOUNDZERO;							
            }	
        }
        else{
            nAsrStatus=LD_ASR_FOUNDZERO;	//执行没有识别
        }
        
        LD_WriteReg(0x2b, 0);	//中断请求编号=0
        LD_WriteReg(0x1C,0);/*写0:ADC不可用*/
        
        LD_WriteReg(0x29,0) ;	//FIFO，同步中断=0，不允许
        LD_WriteReg(0x02,0) ;	//FIFO中断允许位=0
        LD_WriteReg(0x2B,  0);	//中断请求编号=0
        LD_WriteReg(0xBA, 0);	 //中断辅助=0
        LD_WriteReg(0xBC,0);	 //ASR,MP3结束控制=0
        LD_WriteReg(0x08,1);	 /*清除FIFO_DATA*/
        LD_WriteReg(0x08,0);	/*清除FIFO_DATA后 再次写0*/
    }
}


uint8 LD_Check_ASRBusyFlag_b2(void)
{
	uint8 j;
	uint8 flag = 0;
	char mm[2];
	mm[1]=0;
	
	for (j=0; j<10; j++)
	{

		mm[0]=LD_ReadReg(0xb2);

		//DBG("MM=%d",mm[0]);
		if (mm[0] == 0x21)
		{
			flag = 1;
			break;
		}
		rt_thread_delay(10);		
	}
	return flag;
}
/***********************************************************
* 名    称： void LD_AsrStart(void)
* 功    能：
* 入口参数：  ASR初始化
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void LD_AsrStart(void)
{
	LD_Init_ASR();
}
/***********************************************************
* 名    称： uint8 LD_AsrRun(void)
* 功    能： ASR执行函数
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
uint8 LD_AsrRun(void)
{
	nAsrStatus=LD_ASR_RUNING;//zarien 12.10
	LD_WriteReg(0x35, MIC_VOL);
	LD_WriteReg(0x1C, 0x09);
	LD_WriteReg(0xBD, 0x20);
	LD_WriteReg(0x08, 0x01);
	rt_thread_delay( 5 );
	LD_WriteReg(0x08, 0x00);
	rt_thread_delay( 5);

	if(LD_Check_ASRBusyFlag_b2() == 0)
	{
		return 0;
	}

	LD_WriteReg(0xB2, 0xff);	
	LD_WriteReg(0x37, 0x06);
	LD_WriteReg(0x37, 0x06);
	rt_thread_delay( 5);
	LD_WriteReg(0x1C, 0x0b);
	LD_WriteReg(0x29, 0x10);
	
	LD_WriteReg(0xBD, 0x00);   
	return 1;
}
uint8 LD_AsrAddFixed(void)
{
	uint8 k, flag;
	uint8 nAsrAddLength;
	#define DATE_A 14       /*数组二维数值*/
	#define DATE_B 21		/*数组一维数值*/
	 uint8  sRecog[DATE_A][DATE_B] = {
	 									"fan ye",\
										"shang ke",\
										"kan ni mei",\
									    "xiao xi",\
										"pu run te",\
										"bi li",\
										"se ba",\
									    "di ya",\
										"yi na",\
										"ai fu",\
										"ji lo",\
										"ei chi",\
										"ai bi",\
										"zhi mi",\
//										"kei",\
//									    "lai ao",\
//										"ai mu",\
//										"en",\
//										"ou",\
//										"pi",\
//										"kiu",\
//										"a",\
//										"ai si",\
//										"ti" \

									};	/*添加关键词，用户修改*/
	 uint8  pCode[DATE_A] = {
										 CODE_QJ,\
										 CODE_HT,\
										 CODE_KNM,\
										 CODE_YMD,\
										 CODE_A,\
										 CODE_B,\
										 CODE_C,\
										 CODE_D,\
										 CODE_E,\
										 CODE_F,\
										 CODE_G,\
										 CODE_H,\
										 CODE_I,\
										 CODE_J,\
//										 CODE_K,\
//										 CODE_L,\
//										 CODE_M,\
//										 CODE_N,\
//										 CODE_O,\
//										 CODE_P,\
//										 CODE_Q,\
//										 CODE_R,\
//										 CODE_S,\
//										 CODE_T \
										 										 									
							};	/*添加识别码，用户修改*/
	flag = 1;
	for (k=0; k<DATE_A; k++)
	{			
		if(LD_Check_ASRBusyFlag_b2() == 0)
		{
			flag = 0;
			break;
		}
		
		LD_WriteReg(0xc1, pCode[k] );
		LD_WriteReg(0xc3, 0 );
		LD_WriteReg(0x08, 0x04);
		rt_thread_delay(1);		  
		LD_WriteReg(0x08, 0x00);
		rt_thread_delay(1);		  

		for (nAsrAddLength=0; nAsrAddLength<DATE_B; nAsrAddLength++)
		{
			if (sRecog[k][nAsrAddLength] == 0)
				break;
			LD_WriteReg(0x5, sRecog[k][nAsrAddLength]);
		}
		LD_WriteReg(0xb9, nAsrAddLength);
		LD_WriteReg(0xb2, 0xff);
		LD_WriteReg(0x37, 0x04);
//		LD_WriteReg(0x37, 0x04);	//zarien 12.10
	}	 

    return flag;
}


u8 LD3320_WrDat(unsigned char dat);

void LD_WriteReg(uint8 data1,uint8 data2)
{
	LD_CS_L();
	LD3320_WrDat(0x04);
	LD3320_WrDat(data1);
	LD3320_WrDat(data2);
	LD_CS_H();
}

uint8 LD_ReadReg(uint8 reg_add)
{
	uint8 i;
	LD_CS_L();
	LD3320_WrDat(0x05);
	LD3320_WrDat(reg_add);
	i=LD3320_WrDat(0x00);	/*读SPI*/
	LD_CS_H();
	return(i);
}



