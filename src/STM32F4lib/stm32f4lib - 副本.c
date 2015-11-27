#include "stm32f4lib.h"
bool SCPE(u16 PE)//(SCPE(PERIOA))
{
	switch(PE/0X1000)
	{
		case 0:return false;
		case 1:RCC->AHB1ENR|=1<<(PE&0xff);break;
		case 2:RCC->AHB2ENR|=1<<(PE&0xff);break;
		case 3:RCC->AHB3ENR|=1<<(PE&0xff);break;
		case 4:RCC->APB1ENR|=1<<(PE&0xff);break;
      case 5:RCC->APB2ENR|=1<<(PE&0xff);break;
		default:return false;
	}
	return true;
}
u32 max(u32 *shulie ,u32 len)
{
	u32 max=0;
	u32 i=0;
	for(i=0;i<len;i++)
	{
		if(shulie[i]>max)
		max=shulie[i];
	}
	return max;

}


    //使能PORTA时钟
static u8  fac_us=0;//us延时倍乘数
static u16 fac_ms=0;//ms延时倍乘数
void delay_init(u8 SYSCLK)
{
	SysTick->CTRL&=0xfffffffb;//bit2清空,选择外部时钟  HCLK/8
	fac_us=SYSCLK/8;		    
	fac_ms=(u16)fac_us*1000;
}	
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;           //清空计数器
	SysTick->CTRL=0x01 ;          //开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL=0x00;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	  	    
}  
void IOConfig(GPIO_TypeDef*  IOBase,u32 PIN,enum IOtype mode)	  //(IOCB,0,PIN3|PIN2,8)//
{
	GPIO_InitTypeDef GPIO_InitStructure ;
	GPIO_InitStructure.GPIO_Pin = PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz ;
	switch(mode)
	{
		case monishuru:{
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AN;
			break;
		}
		case shanglashuru:{
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
			GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
			break;
		}
		case xialashuru:{
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
			GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;
			break;
		}
		case fukongshuru:{
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
			GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
			break;
		}
		case kailoushuchu:{
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;
			break;
		}
		case tuiwanshuchu:{
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
			break;
		}
		case fuyong:{
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
			break;
		}
		
	
	}
//	GPIO_InitStructure.GPIO_Mode =(enum IOtype)mode;
	GPIO_Init(IOBase ,&GPIO_InitStructure) ;
}
void waibuzhongduanInit()
{
  NVIC_InitTypeDef   NVIC_InitStructure;	
	EXTI_InitTypeDef   EXTI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
 
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //时间加载	  		 
	SysTick->VAL=0x00;        //清空计数器
	SysTick->CTRL=0x01 ;      //开始倒数 	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL=0x00;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	 
}


	
	
	
	
	



void zhongduanConfig(u8 NVIC_Channel,int i)
{
	//MY_NVIC_Init(i,0,NVIC_Channel,4);			
}
void usart2putchar(char c)
{
	USART2->DR=c;				 
}
void delay_s(u32 aa)
{
	//u32 i;
	//for(i=0;i<aa;i++)
	//delay_ms(1000);
}
//waibuzhongduanConfig(GPIOA,13,FTIR,12);


/*EXTI0_IRQChannel             ((u8)0x06) 
EXTI1_IRQChannel             ((u8)0x07)  
EXTI2_IRQChannel             ((u8)0x08)  
EXTI3_IRQChannel             ((u8)0x09)  
EXTI4_IRQChannel 		
EXTI9_5_IRQChannel 
EXTI15_10_IRQChannel
EXTI0_IRQHandler
*/
//EXTI->PR=9<<0;


void mprintf(char *shuru,...)
{
    int i=0;
    int canshujishu=1;
    //printf("%c",*(shuru));
   // while(*(((char *)(&shuru+4*i)))!='\0')
    while(*(char *)(shuru+i)!='\0')
    {
        char now=*(char *)(shuru+i);
        char next=*(char *)(shuru+i+1);
        switch(now)
        {

        case '%':
        {
            switch(next)
            {
                case 'd':
                {
                    char  jishu=0;
                    char xulie[20];
                    u32 shuchu=*((int *)((&shuru)+(canshujishu++)));
					if(shuchu==0)
					pc('0');
                    for(jishu=0;jishu<20&&shuchu!=0;jishu++)
                    {
                        xulie[jishu]=shuchu%10;
                        shuchu/=10;
                    }
                    for(;jishu>0;jishu--)
                    {
                        pc(xulie[jishu-1]+48);
                    }

                    break;
                }
                case 'b':
                {
                    char  jishu=0;
                    char xulie[50];
                    int shuchu=*((int *)((&shuru)+(canshujishu++)));
					if(shuchu==0)
					pc('0');
                    for(jishu=0;jishu<20&&shuchu!=0;jishu++)
                    {
                        xulie[jishu]=shuchu%2;
                        shuchu/=2;
                    }
                    for(;jishu>0;jishu--)
                    {
                        pc(xulie[jishu-1]+48);
                    }

                    break;
                }
                case 'H':
                {
                    char  jishu=0;
                    char xulie[20];
                    int shuchu=*((int *)((&shuru)+(canshujishu++)));
					if(shuchu==0)
					pc('0');
                    for(jishu=0;jishu<20&&shuchu!=0;jishu++)
                    {
                        xulie[jishu]=shuchu%16;
                        shuchu/=16;
                    }
                    for(;jishu>0;jishu--)
                    {
                        if(xulie[jishu-1]<10)
                         pc(xulie[jishu-1]+48);
                        else
                            pc(xulie[jishu-1]-10+'A');
                    }

                    break;
                }
            case 'h':
            {
                char  jishu=0;
                char xulie[20];
                int shuchu=*((int *)((&shuru)+(canshujishu++)));
				if(shuchu==0)
					pc('0');
                for(jishu=0;jishu<20&&shuchu!=0;jishu++)
                {
                    xulie[jishu]=shuchu%16;
                    shuchu/=16;
                }
                for(;jishu>0;jishu--)
                {
                    if(xulie[jishu-1]<10)
                     pc(xulie[jishu-1]+48);
                    else
                        pc(xulie[jishu-1]-10+'a');
                }

                break;
            }
                case 'c':
                {
                    char shuchu=*((char *)((&shuru)+(canshujishu++)));
                    pc(shuchu);
                }
            }
            i++;
            break;
        }
        default:pc(now);
        }
        i++;
    }
}
u32 StringToInt(u8 * mm,u8 jinzhi)
{
	u32 i=0,fangchujie=0,deshu=0;
	if(jinzhi==10)
	{
		while((mm[i]!=0x0D)&&(mm[i]!=0x0A))
		{
			 deshu*=10;
			 deshu+=(mm[i]-48);
			 i++;
			 fangchujie++;
		//	 printf("deshu:%d,i:%d",deshu,i);
			// if(fangchujie>20||mm[i]>58||mm[i<48])
			 //return 2262;
		}
		return deshu;
	}
	else return 2282;
	
}
u32 min(short *miao,u32 len)
{
	u32 i=0;
	char min=*miao;
	u32 weizhi=0;
	for(i=0;i<len;i++)
	{

			if(miao[i]<=min)
			{
				min=miao[i];
				weizhi=i;
			}
		
	//		printf("  %d:%d=%d ",i,*(miao+i),min);
	}
	return weizhi;
}


void pc_real(u8 cc)
{
	USART_SendData(USART1,cc);
	 while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	delay_ms(1);
}

