#include "wkup.h"
#include "led.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////     
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//�������� ��������               
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/6/7 
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved                       
//////////////////////////////////////////////////////////////////////////////////     
 

//ϵͳ�������ģʽ
void Sys_Enter_Standby(void)
{             
    //�ر���������(����ʵ�����д)
       RCC->APB2RSTR|=0X01FC;//��λ����IO��
    Sys_Standby();//�������ģʽ
}
//���WKUP�ŵ��ź�
//����ֵ1:��������3s����
//      0:����Ĵ���    
u8 Check_WKUP(void) 
{
    u8 t=0;
    u8 tx=0;//��¼�ɿ��Ĵ���
    LED0=0; //����DS0 
    while(1)
    {
        if(WKUP_KD)//�Ѿ�������
        {
            t++;
            tx=0;
        }else 
        {
            tx++; //����300ms��û��WKUP�ź�
            if(tx>3)
            {
                LED0=1;
                return 0;//����İ���,���´�������
            }
        }
        delay_ms(30);
        if(t>=100)//���³���3����
        {
            LED0=0;      //����DS0 
            return 1; //����3s������
        }
    }
}  
//�ж�,��⵽PA0�ŵ�һ��������.      
//�ж���0���ϵ��жϼ��
void EXTI0_IRQHandler(void)
{                                                          
    EXTI->PR=1<<0;  //���LINE10�ϵ��жϱ�־λ          
    if(Check_WKUP())//�ػ�?
    {          
        Sys_Enter_Standby();  
    }
} 
//PA0 WKUP���ѳ�ʼ��
void WKUP_Init(void)
{                
    RCC->APB2ENR|=1<<2;     //��ʹ������IO PORTAʱ��    
    RCC->APB2ENR|=1<<0;     //������ʱ��          
     
    GPIOA->CRL&=0XFFFFFFF0;//PA0���ó�����      
    GPIOA->CRL|=0X00000008;  
    Ex_NVIC_Config(GPIO_A,0,RTIR);//PA0�����ش���       
                                                                 
    //(����Ƿ���������)��             
    if(Check_WKUP()==0)Sys_Standby();    //���ǿ���,�������ģʽ  
    MY_NVIC_Init(2,2,EXTI0_IRQChannel,2);//��ռ2�������ȼ�2����2
}
















