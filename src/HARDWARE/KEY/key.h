#ifndef __KEY_H
#define __KEY_H     
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////     
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//�������� ��������           
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/11/28 
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************
//V1.1�޸�˵��
//�޸İ���ɨ�躯����ʹ�����������֧��SWD���ء�
//////////////////////////////////////////////////////////////////////////////////     


#define KEY0 PAin(13)   //PA13
#define KEY1 PAin(15)    //PA15 
#define KEY2 PAin(0)    //PA0  WK_UP
void read(void);     
void KEY_Init(void);//IO��ʼ��
void jiemafuwei(void);
u8 KEY_Scan(void);  //����ɨ�躯��    
extern int sudu[900];
extern u32 jishu10;                    
void EXTIX_Init(void);
#endif
