#ifndef __WDG_H
#define __WDG_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////     
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//���Ź� ��������           
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/12/02
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////       

void IWDG_Init(u8 prer,u16 rlr);     //��ʼ��IWDG
void IWDG_Feed(void);                 //IWDGι��
void WWDG_Init(u8 tr,u8 wr,u8 fprer);//��ʼ��WWDG
void WWDG_Set_Counter(u8 cnt);       //����WWDG�ļ�����
#endif




























