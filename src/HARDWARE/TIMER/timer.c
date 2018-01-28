#include "timer.h"
#include "led.h"
#include"lcd.h"
//////////////////////////////////////////////////////////////////////////////////     
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//ͨ�ö�ʱ�� ��������               
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/12/03
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////       

//��ʱ��3�жϷ������     

void TIM3_IRQHandler(void)
{                                       
    

    TIM3->SR&=~(1<<0);
            
}
/*
void Timer4_Init(u16 arr,u16 psc)
{
    RCC->APB1ENR|=1<<2;//TIM3ʱ��ʹ��    
     TIM4->ARR=arr;  //�趨�������Զ���װֵ//�պ�1ms    
    TIM4->PSC=psc;  //Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��
    //����������Ҫͬʱ���òſ���ʹ���ж�                                          
    TIM4->CR1|=0x01;    //ʹ�ܶ�ʱ��3
      MY_NVIC_Init(1,3,TIM4_IRQChannel ,2);//��ռ1�������ȼ�3����2                                     
}           */

//TIM3 PWM����
//����ԭ��@ALIENTEK
//2010/6/2     

//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void daPWM(u16 arr,u16 psc)
{                              
    TIM3->ARR=arr;//�趨�������Զ���װֵ 
    TIM3->PSC=psc;//Ԥ��Ƶ������Ƶ
//    TIM3->DIER|=1<<0;   //��������ж�                
//    TIM3->DIER|=1<<6;   //�������ж�    
    TIM3->CCMR1|=7<<12;  //CH2 PWM2ģʽ         
    TIM3->CCMR1|=1<<11; //CH2Ԥװ��ʹ��
    TIM3->CCMR1|=7<<4;  //CH2 PWM2ģʽ
    TIM3->CCMR1|=1<<3; //CH2Ԥװ��ʹ��    
    TIM3->CCER|=1<<4;   //OC2 ���ʹ��    
    TIM3->CCER|=1;   //OC2 ���ʹ��
    TIM3->CR1=0x80;   //ARPEʹ�� 
    TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3     
    MY_NVIC_Init(1,3,TIM3_IRQChannel ,3);                                          
}  
void xiaoPWM(u16 arr,u16 psc)
{
    TIM2->ARR=arr;
    TIM2->PSC=psc;
    TIM2->CCR2=TIM2->ARR/2;
    TIM2->CCMR1|=7<<12;  //CH2 PWM2ģʽ    
    TIM2->CCMR1|=1<<11; //CH2Ԥװ��ʹ��
//    TIM2->CCER|=1<<4;   //OC2 ���ʹ��    
//    TIM2->CR1=0x80;   //ARPEʹ�� 
    TIM2->CR1|=0x01;    //ʹ�ܶ�ʱ��3 
}
void shizhong_Init(u16 arr,u16 psc)
{
    TIM2->ARR=arr;
    TIM2->PSC=psc;
//    TIM2->CCR2=arr/2;
    TIM2->CCMR1|=7<<12;  //CH2 PWM2ģʽ    
    TIM2->CCMR1|=1<<11; //CH2Ԥװ��ʹ��
    TIM2->CCER|=1<<4;   //OC2 ���ʹ��    
    TIM2->CR1=0x80;   //ARPEʹ�� 
    TIM2->CR1|=0x01;    //ʹ�ܶ�ʱ��3     
}    
void TIM4_Init(u16 arr,u16 psc)
{    
    GPIOB->CRL&=0X00FFFFff; 
    GPIOB->CRL|=0X88000000;                 
    TIM4->ARR=arr;//�趨�������Զ���װֵ 
    TIM4->PSC=psc;//Ԥ��Ƶ������Ƶ
//    TIM3->DIER|=1<<0;   //��������ж�                
//    TIM3->DIER|=1<<6;   //�������ж�    
    TIM4->CCMR1|=1;  //CH2 PWM2ģʽ         
    TIM4->CCMR1|=1<<8; //CH2Ԥװ��ʹ��
    TIM4->CCER=0;   //OC2 ���ʹ��    
//    TIM4->CR1=0x80;   //ARPEʹ�� 
    TIM4->CR1|=0x01;    //ʹ�ܶ�ʱ��3     
    TIM4->SMCR|=3;
//    TIM1->CCR1=500;    
}

void TIM1_Init(u16 arr,u16 psc)
{    
    GPIOB->CRL&=0X00FFFFff; 
    GPIOB->CRL|=0X88000000;                 
    TIM1->ARR=arr;//�趨�������Զ���װֵ 
    TIM1->PSC=psc;//Ԥ��Ƶ������Ƶ
//    TIM3->DIER|=1<<0;   //��������ж�                
//    TIM3->DIER|=1<<6;   //�������ж�    
//    TIM1->CCMR1|=1;  //CH2 PWM2ģʽ         
//    TIM4->CCMR1|=1<<8; //CH2Ԥװ��ʹ��
//    TIM4->CCER=0;   //OC2 ���ʹ��    
//    TIM4->CR1=0x80;   //ARPEʹ�� 
    TIM1->CR1|=0x01;    //ʹ�ܶ�ʱ��3     
    TIM1->SMCR=7+0x10*7+(1<<14);
//    TIM1->CCR1=500;                      
}
/*
void TIM1_Init(u16 arr,u16 psc)
{    
    GPIOB->CRL&=0X00FFFFff; 
    GPIOB->CRL|=0X88000000;                 
    TIM1->ARR=arr;//�趨�������Զ���װֵ 
    TIM1->PSC=psc;//Ԥ��Ƶ������Ƶ
//    TIM3->DIER|=1<<0;   //��������ж�                
//    TIM3->DIER|=1<<6;   //�������ж�    
    TIM1->CCMR1|=1;       
//    TIM4->CCMR1|=1<<8; //CH2Ԥװ��ʹ��
//    TIM4->CCER=0;   //OC2 ���ʹ��    
//    TIM4->CR1=0x80;   //ARPEʹ�� 
    TIM1->CR1|=0x01;    //ʹ�ܶ�ʱ��3     
//    TIM1->SMCR=7+0x10*7+(1<<14);
//    TIM1->CCR1=500;    
    TIM1->SMCR=2+0x10*5;                  
}  */



