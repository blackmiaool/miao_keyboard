#include "timer.h"
#include "led.h"
#include"lcd.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//通用定时器 驱动代码			   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/12/03
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  

//定时器3中断服务程序	 

void TIM3_IRQHandler(void)
{ 		    		  			    
	

	TIM3->SR&=~(1<<0);
		    
}
/*
void Timer4_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<2;//TIM3时钟使能    
 	TIM4->ARR=arr;  //设定计数器自动重装值//刚好1ms    
	TIM4->PSC=psc;  //预分频器7200,得到10Khz的计数时钟
	//这两个东东要同时设置才可以使用中断		  							    
	TIM4->CR1|=0x01;    //使能定时器3
  	MY_NVIC_Init(1,3,TIM4_IRQChannel ,2);//抢占1，子优先级3，组2									 
}		   */

//TIM3 PWM部分
//正点原子@ALIENTEK
//2010/6/2	 

//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void daPWM(u16 arr,u16 psc)
{		 					 
	TIM3->ARR=arr;//设定计数器自动重装值 
	TIM3->PSC=psc;//预分频器不分频
//	TIM3->DIER|=1<<0;   //允许更新中断				
//	TIM3->DIER|=1<<6;   //允许触发中断	
	TIM3->CCMR1|=7<<12;  //CH2 PWM2模式		 
	TIM3->CCMR1|=1<<11; //CH2预装载使能
	TIM3->CCMR1|=7<<4;  //CH2 PWM2模式
	TIM3->CCMR1|=1<<3; //CH2预装载使能	
	TIM3->CCER|=1<<4;   //OC2 输出使能	
	TIM3->CCER|=1;   //OC2 输出使能
	TIM3->CR1=0x80;   //ARPE使能 
	TIM3->CR1|=0x01;    //使能定时器3 	
	MY_NVIC_Init(1,3,TIM3_IRQChannel ,3);										  
}  
void xiaoPWM(u16 arr,u16 psc)
{
	TIM2->ARR=arr;
	TIM2->PSC=psc;
	TIM2->CCR2=TIM2->ARR/2;
	TIM2->CCMR1|=7<<12;  //CH2 PWM2模式	
	TIM2->CCMR1|=1<<11; //CH2预装载使能
//	TIM2->CCER|=1<<4;   //OC2 输出使能	
//	TIM2->CR1=0x80;   //ARPE使能 
	TIM2->CR1|=0x01;    //使能定时器3 
}
void shizhong_Init(u16 arr,u16 psc)
{
	TIM2->ARR=arr;
	TIM2->PSC=psc;
//	TIM2->CCR2=arr/2;
	TIM2->CCMR1|=7<<12;  //CH2 PWM2模式	
	TIM2->CCMR1|=1<<11; //CH2预装载使能
	TIM2->CCER|=1<<4;   //OC2 输出使能	
	TIM2->CR1=0x80;   //ARPE使能 
	TIM2->CR1|=0x01;    //使能定时器3 	
}	
void TIM4_Init(u16 arr,u16 psc)
{	
	GPIOB->CRL&=0X00FFFFff; 
	GPIOB->CRL|=0X88000000;				 
	TIM4->ARR=arr;//设定计数器自动重装值 
	TIM4->PSC=psc;//预分频器不分频
//	TIM3->DIER|=1<<0;   //允许更新中断				
//	TIM3->DIER|=1<<6;   //允许触发中断	
	TIM4->CCMR1|=1;  //CH2 PWM2模式		 
	TIM4->CCMR1|=1<<8; //CH2预装载使能
	TIM4->CCER=0;   //OC2 输出使能	
//	TIM4->CR1=0x80;   //ARPE使能 
	TIM4->CR1|=0x01;    //使能定时器3 	
	TIM4->SMCR|=3;
//	TIM1->CCR1=500;	
}

void TIM1_Init(u16 arr,u16 psc)
{	
	GPIOB->CRL&=0X00FFFFff; 
	GPIOB->CRL|=0X88000000;				 
	TIM1->ARR=arr;//设定计数器自动重装值 
	TIM1->PSC=psc;//预分频器不分频
//	TIM3->DIER|=1<<0;   //允许更新中断				
//	TIM3->DIER|=1<<6;   //允许触发中断	
//	TIM1->CCMR1|=1;  //CH2 PWM2模式		 
//	TIM4->CCMR1|=1<<8; //CH2预装载使能
//	TIM4->CCER=0;   //OC2 输出使能	
//	TIM4->CR1=0x80;   //ARPE使能 
	TIM1->CR1|=0x01;    //使能定时器3 	
	TIM1->SMCR=7+0x10*7+(1<<14);
//	TIM1->CCR1=500;					  
}
/*
void TIM1_Init(u16 arr,u16 psc)
{	
	GPIOB->CRL&=0X00FFFFff; 
	GPIOB->CRL|=0X88000000;				 
	TIM1->ARR=arr;//设定计数器自动重装值 
	TIM1->PSC=psc;//预分频器不分频
//	TIM3->DIER|=1<<0;   //允许更新中断				
//	TIM3->DIER|=1<<6;   //允许触发中断	
	TIM1->CCMR1|=1;  	 
//	TIM4->CCMR1|=1<<8; //CH2预装载使能
//	TIM4->CCER=0;   //OC2 输出使能	
//	TIM4->CR1=0x80;   //ARPE使能 
	TIM1->CR1|=0x01;    //使能定时器3 	
//	TIM1->SMCR=7+0x10*7+(1<<14);
//	TIM1->CCR1=500;	
	TIM1->SMCR=2+0x10*5;				  
}  */



