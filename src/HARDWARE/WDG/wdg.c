#include "wdg.h"
#include "led.h"
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
            

//��ʼ���������Ź�
//prer:��Ƶ��:0~7(ֻ�е�3λ��Ч!)
//��Ƶ����=4*2^prer.�����ֵֻ����256!
//rlr:��װ�ؼĴ���ֵ:��11λ��Ч.
//ʱ�����(���):Tout=((4*2^prer)*rlr)/40 (ms).
void IWDG_Init(u8 prer,u16 rlr) 
{
    IWDG->KR=0X5555;//ʹ�ܶ�IWDG->PR��IWDG->RLR��д                                                   
      IWDG->PR=prer;  //���÷�Ƶϵ��   
      IWDG->RLR=rlr;  //�Ӽ��ؼĴ��� IWDG->RLR  
    IWDG->KR=0XAAAA;//reload                                               
      IWDG->KR=0XCCCC;//ʹ�ܿ��Ź�    
}
//ι�������Ź�
void IWDG_Feed(void)
{
    IWDG->KR=0XAAAA;//reload                                               
}

//����WWDG������������ֵ,Ĭ��Ϊ���.
u8 WWDG_CNT=0x7f;
//��ʼ�����ڿ��Ź�
//tr   :T[6:0],���ڴ洢��������ֵ
//wr   :W[6:0],���ڴ洢����ֵ
//fprer:���ڿ��Ź���ʵ������
//��2λ��Ч.Fwwdg=PCLK1/4096/2^fprer.
void WWDG_Init(u8 tr,u8 wr,u8 fprer)
{                              
    RCC->APB1ENR|=1<<11;    //ʹ��wwdgʱ��
    WWDG_CNT=tr&WWDG_CNT;   //��ʼ��WWDG_CNT.                                       
    WWDG->CFR|=fprer<<7;    //PCLK1/4096�ٳ�2^fprer
    WWDG->CFR|=1<<9;        //ʹ����ǰ�����ж�
    WWDG->CFR&=0XFF80;     
    WWDG->CFR|=wr;            //�趨����ֵ     
    WWDG->CR|=WWDG_CNT|(1<<7); //�����Ź�,����7λ������                                  
    MY_NVIC_Init(2,3,WWDG_IRQChannel,2);//��ռ2�������ȼ�3����2       
}
//������WWDG��������ֵ
void WWDG_Set_Counter(u8 cnt)
{
    WWDG->CR|=(cnt&0x7F);//������7λ������
}
//���ڿ��Ź��жϷ������
void WWDG_IRQHandler(void)
{                        
    u8 wr,tr;  
     wr=WWDG->CFR&0X7F;
    tr=WWDG->CR&0X7F;
    if(tr<wr)WWDG_Set_Counter(WWDG_CNT);//ֻ�е���������ֵ,С�ڴ��ڼĴ�����ֵ����дCR!!                      
    WWDG->SR=0X00;//�����ǰ�����жϱ�־λ
    LED1=!LED1;
}



















