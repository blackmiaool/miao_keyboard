#ifndef __RTC_H
#define __RTC_H        
//////////////////////////////////////////////////////////////////////////////////     
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//RTCʵʱʱ�� ��������           
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/12/30 
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************
//V1.1�޸�˵��
//�޸���RTC_Init������Ƶ������Ч��bug
//�޸���RTC_Get������һ��bug
//////////////////////////////////////////////////////////////////////////////////     


//ʱ��ṹ��
typedef struct 
{
    u8 hour;
    u8 min;
    u8 sec;            
    //������������
    u16 w_year;
    u8  w_month;
    u8  w_date;
    u8  week;         
}tm;                     
extern tm timer; 

extern u8 const mon_table[12];//�·��������ݱ�
void Disp_Time(u8 x,u8 y,u8 size);//���ƶ�λ�ÿ�ʼ��ʾʱ��
void Disp_Week(u8 x,u8 y,u8 size,u8 lang);//��ָ��λ����ʾ����
u8 RTC_Init(void);        //��ʼ��RTC,����0,ʧ��;1,�ɹ�;
u8 Is_Leap_Year(u16 year);//ƽ��,�����ж�
u8 RTC_Get(void);         //����ʱ��   
u8 RTC_Get_Week(u16 year,u8 month,u8 day);
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);//����ʱ��      
void Auto_Time_Set(void);//����ʱ��Ϊ����ʱ��
#endif




























 
