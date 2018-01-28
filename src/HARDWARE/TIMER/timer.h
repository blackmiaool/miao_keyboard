#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
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

//ͨ���ı�TIM3->CCR2��ֵ���ı�ռ�ձȣ��Ӷ�����LED0������
#define zhengsu TIM3->CCR1 
#define fansu TIM3->CCR2 
#define busu TIM2->CCR2 

void Timer4_Init(u16 arr,u16 psc);
void daPWM(u16 arr,u16 psc);
void xiaoPWM(u16 arr,u16 psc);
void TIM3_IRQHandler(void);
void TIM4_Init(u16 arr,u16 psc);
void TIM1_Init(u16 arr,u16 psc);
extern u32 jishi,jishi1,jishi2;
extern u16 xianshi,wancheng;
#endif























