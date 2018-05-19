#include "wdg.h"
//prer:分频数:0~7(只有低 3 位有效!)
//rlr:自动重装载值,0~0XFFF.
//分频因子=4*2^prer.但最大值只能是 256!
//rlr:重装载寄存器值:低 11 位有效.
//时间计算(大概):Tout=((4*2^prer)*rlr)/32 (ms)

void IWDG_Init(u8 prer, u16 rlr)//常用prer = 4, rlr = 500, 此时溢出时间为1s
{
	//注意: 直接写入键值, 不需要用与或
	IWDG->KR = 0x5555;		//使能对IWDG->PR和IWDG->RLR的写
	IWDG->PR = prer;
	IWDG->RLR = rlr;
	IWDG->KR = 0xAAAA;		//装载数值
	IWDG->KR = 0xCCCC;		//使能看门狗
}

//每隔一段时间 喂狗
void IWDG_Feed(void)
{
	IWDG->KR = 0xAAAA;	//装载数值
}


/*********************************
Window Watch Dog
**********************************/
//保存 WWDG 计数器的设置值,默认为最大.
u8 WWDG_CNT=0x7f;	//WWDG->CR低7位用来存储计数器的值

//初始化窗口看门狗
//tr :T[6:0],计数器值
//wr :W[6:0],窗口值
//fprer:分频系数（ WDGTB） ,仅最低 2 位有效
//Fwwdg=PCLK1/(4096*2^fprer). 一般 PCLK1=42Mhz
void WWDG_Init(u8 tr, u8 wr, u8 fprer)
{
	RCC->APB1ENR |= 1<<11;	//使能wwdg时钟
	WWDG_CNT = tr & WWDG_CNT;	//原WWDG_CNT值为全1

	WWDG->CFR |= fprer<<7;	//CFR第七位对应的是分频系数
	WWDG->CFR &= 0xFF80;	//W[6:0]为0, 窗口值清零
	WWDG->CFR |= wr;		//设定窗口值

	WWDG->CR |= WWDG_CNT;	//装填计数值
	WWDG->CR |= 1<<7;		//开启看门狗

	MY_NVIC_Init(2,3,WWDG_IRQChannel,2);	//抢占优先级为2, 子优先级为3, 组2
	WWDG->SR = 0x00;	//清除提前唤醒中断标志
	WWDG->CFR |= 1<<9;	//使能提前唤醒中断

}

//重设WWDG计数器的值 相当于喂狗
void WWDG_Set_Counter(u8 cnt)
{
	WWDG->CR = (cnt&0x7f);	
}

//WWDG中断服务程序
void WWDG_IRQHandler(void)
{
	WWDG_Set_Counter(WWDG_CNT);	//重设窗口看门狗的值(这个值是在init函数里处理过的值,不是开始定义的值)
	WWDG->SR = 0x00;	//清除提前唤醒中断标志
    SCB->AIRCR = 0x05FA0000 | (u32)0x04;//restart
}









