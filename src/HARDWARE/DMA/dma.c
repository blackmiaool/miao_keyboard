#include "dma.h"
//////////////////////////////////////////////////////////////////////////////////     
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//DMA ��������               
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/6/7 
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////       
 

u16 DMA1_MEM_LEN;//����DMAÿ�����ݴ��͵ĳ���             
//DMA1�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_CHx:DMAͨ��CHx
//cpar:�����ַ
//cmar:�洢����ַ
//cndtr:���ݴ�����  
void MYDMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
    u32 DR_Base;  //��������,��֪��Ϊʲô.��Ҫ����
    RCC->AHBENR|=1<<0;//����DMA1ʱ��
    DR_Base=cpar;
    DMA_CHx->CPAR=DR_Base;      //DMA1 �����ַ 
    DMA_CHx->CMAR=(u32)cmar; //DMA1,�洢����ַ
    DMA1_MEM_LEN=cndtr;      //����DMA����������
    DMA_CHx->CNDTR=cndtr;    //DMA1,����������
    DMA_CHx->CCR=0X00000000;//��λ
    DMA_CHx->CCR|=1<<4;  //�Ӵ洢����
    DMA_CHx->CCR|=0<<5;  //��ͨģʽ
    DMA_CHx->CCR|=0<<6;  //�����ַ������ģʽ
    DMA_CHx->CCR|=1<<7;  //�洢������ģʽ
    DMA_CHx->CCR|=0<<8;  //�������ݿ��Ϊ8λ
    DMA_CHx->CCR|=0<<10; //�洢�����ݿ��8λ
    DMA_CHx->CCR|=1<<12; //�е����ȼ�
    DMA_CHx->CCR|=0<<14; //�Ǵ洢�����洢��ģʽ              
} 
//����һ��DMA����
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{
    DMA_CHx->CCR&=~(1<<0);       //�ر�DMA���� 
    DMA_CHx->CNDTR=DMA1_MEM_LEN; //DMA1,���������� 
    DMA_CHx->CCR|=1<<0;          //����DMA����
}      

 

























