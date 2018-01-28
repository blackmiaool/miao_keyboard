#include "sys.h"
#include "rtc.h" 
#include "delay.h"
#include "usart.h" 
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
  
//Mini STM32������
//RTCʵʱʱ�� ��������             
//����ԭ��@ALIENTEK
//2010/6/6

       
tm timer;//ʱ�ӽṹ��        
//ʵʱʱ������
//��ʼ��RTCʱ��,ͬʱ���ʱ���Ƿ�������
//BKP->DR1���ڱ����Ƿ��һ�����õ�����
//����0:����
//����:�������
u8 RTC_Init(void)
{
    //����ǲ��ǵ�һ������ʱ��
    u8 temp=0;
    if(BKP->DR1!=0X5050)//��һ������
    {     
          RCC->APB1ENR|=1<<28;     //ʹ�ܵ�Դʱ��        
        RCC->APB1ENR|=1<<27;     //ʹ�ܱ���ʱ��        
        PWR->CR|=1<<8;           //ȡ��������д����
        RCC->BDCR|=1<<16;        //����������λ       
        RCC->BDCR&=~(1<<16);     //����������λ����           
        RCC->BDCR|=1<<0;         //�����ⲿ�������� 
        while((!(RCC->BDCR&0X02))&&temp<250)//�ȴ��ⲿʱ�Ӿ���     
        {
            temp++;
            delay_ms(10);
        };
        if(temp>=250)return 1;//��ʼ��ʱ��ʧ��,����������        

        RCC->BDCR|=1<<8; //LSI��ΪRTCʱ��         
        RCC->BDCR|=1<<15;//RTCʱ��ʹ��      
          while(!(RTC->CRL&(1<<5)));//�ȴ�RTC�Ĵ����������     
        while(!(RTC->CRL&(1<<3)));//�ȴ�RTC�Ĵ���ͬ��  
        RTC->CRH|=0X01;            //�������ж�
        while(!(RTC->CRL&(1<<5)));//�ȴ�RTC�Ĵ����������     
        RTC->CRL|=1<<4;              //��������       
        RTC->PRLH=0X0000;
        RTC->PRLL=32767;             //ʱ����������(�д��۲�,���Ƿ�������?)����ֵ��32767                                         
        Auto_Time_Set();
        //RTC_Set(2009,12,2,10,0,55);  //����ʱ��      
        RTC->CRL&=~(1<<4);           //���ø���
        while(!(RTC->CRL&(1<<5)));   //�ȴ�RTC�Ĵ����������                                               
        BKP->DR1=0X5050;
        //BKP_Write(1,0X5050);;//�ڼĴ���1����Ѿ������� 
        //printf("FIRST TIME\n");
    }else//ϵͳ������ʱ
    {
        while(!(RTC->CRL&(1<<3)));//�ȴ�RTC�Ĵ���ͬ��  
        RTC->CRH|=0X01;            //�������ж�
        while(!(RTC->CRL&(1<<5)));//�ȴ�RTC�Ĵ����������
        //printf("OK\n");
    }                              
    MY_NVIC_Init(0,0,RTC_IRQChannel,2);//RTC,G2,P2,S2.���ȼ����     
    RTC_Get();//����ʱ�� 
    return 0; //ok
}
//RTC�жϷ�����         
//const u8* Week[2][7]=
//{
//{"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"},
//{"��","һ","��","��","��","��","��"}
//};                        
//RTCʱ���ж�
//ÿ�봥��һ��   
void RTC_IRQHandler(void)
{                                
    if(RTC->CRL&0x0001)//�����ж�
    {                            
        RTC_Get();//����ʱ��      
        //printf("CRL:%d\n",RTC->CRL);    
     }
    if(RTC->CRL&0x0002)//�����ж�
    {
        //printf("Alarm!\n");    
        RTC->CRL&=~(0x0002);//�������ж�
         //���Ӵ���
    }                                                    
    RTC->CRL&=0X0FFA;         //�������������жϱ�־
    while(!(RTC->CRL&(1<<5)));//�ȴ�RTC�Ĵ����������                                                    
}
//�ж��Ƿ������꺯��
//�·�   1  2  3  4  5  6  7  8  9  10 11 12
//����   31 29 31 30 31 30 31 31 30 31 30 31
//������ 31 28 31 30 31 30 31 31 30 31 30 31
//����:���
//���:������ǲ�������.1,��.0,����
u8 Is_Leap_Year(u16 year)
{              
    if(year%4==0) //�����ܱ�4����
    { 
        if(year%100==0) 
        { 
            if(year%400==0)return 1;//�����00��β,��Ҫ�ܱ�400����        
            else return 0;   
        }else return 1;   
    }else return 0;    
}                    
//����ʱ��
//�������ʱ��ת��Ϊ����
//��1970��1��1��Ϊ��׼
//1970~2099��Ϊ�Ϸ����
//����ֵ:0,�ɹ�;����:�������.
//�·����ݱ�                                             
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //���������ݱ�      
//ƽ����·����ڱ�
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
    u16 t;
    u32 seccount=0;
    if(syear<1970||syear>2099)return 1;       
    for(t=1970;t<syear;t++)    //��������ݵ��������
    {
        if(Is_Leap_Year(t))seccount+=31622400;//�����������
        else seccount+=31536000;              //ƽ���������
    }
    smon-=1;
    for(t=0;t<smon;t++)       //��ǰ���·ݵ����������
    {
        seccount+=(u32)mon_table[t]*86400;//�·����������
        if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//����2�·�����һ���������       
    }
    seccount+=(u32)(sday-1)*86400;//��ǰ�����ڵ���������� 
    seccount+=(u32)hour*3600;//Сʱ������
    seccount+=(u32)min*60;     //����������
    seccount+=sec;//�������Ӽ���ȥ
                                                        
    //����ʱ��
    RCC->APB1ENR|=1<<28;//ʹ�ܵ�Դʱ��
    RCC->APB1ENR|=1<<27;//ʹ�ܱ���ʱ��
    PWR->CR|=1<<8;    //ȡ��������д����
    //���������Ǳ����!
    RTC->CRL|=1<<4;   //�������� 
    RTC->CNTL=seccount&0xffff;
    RTC->CNTH=seccount>>16;
    RTC->CRL&=~(1<<4);//���ø���
    while(!(RTC->CRL&(1<<5)));//�ȴ�RTC�Ĵ����������      
    return 0;        
}
//�õ���ǰ��ʱ��
//����ֵ:0,�ɹ�;����:�������.
u8 RTC_Get(void)
{
    static u16 daycnt=0;
    u32 timecount=0; 
    u32 temp=0;
    u16 temp1=0;      
       
    timecount=RTC->CNTH;//�õ��������е�ֵ(������)
    timecount<<=16;
    timecount+=RTC->CNTL;             

    temp=timecount/86400;   //�õ�����(��������Ӧ��)
    if(daycnt!=temp)//����һ����
    {      
        daycnt=temp;
        temp1=1970;    //��1970�꿪ʼ
        while(temp>=365)
        {                 
            if(Is_Leap_Year(temp1))//������
            {
                if(temp>=366)temp-=366;//�����������
                else break;  
            }
            else temp-=365;      //ƽ�� 
            temp1++;  
        }   
        timer.w_year=temp1;//�õ����
        temp1=0;
        while(temp>=28)//������һ����
        {
            if(Is_Leap_Year(timer.w_year)&&temp1==1)//�����ǲ�������/2�·�
            {
                if(temp>=29)temp-=29;//�����������
                else break; 
            }
            else 
            {
                if(temp>=mon_table[temp1])temp-=mon_table[temp1];//ƽ��
                else break;
            }
            temp1++;  
        }
        timer.w_month=temp1+1;//�õ��·�
        timer.w_date=temp+1;  //�õ����� 
    }
    temp=timecount%86400;     //�õ�������          
    timer.hour=temp/3600;     //Сʱ
    timer.min=(temp%3600)/60; //����    
    timer.sec=(temp%3600)%60; //����
    timer.week=RTC_Get_Week(timer.w_year,timer.w_month,timer.w_date);//��ȡ����   
    return 0;
}     
//������������ڼ�
//��������:���빫�����ڵõ�����(ֻ����1901-2099��)
//������������������� 
//����ֵ�����ں�                                                                                         
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{    
    u16 temp2;
    u8 yearH,yearL;
    
    yearH=year/100;    yearL=year%100; 
    // ���Ϊ21����,�������100  
    if (yearH>19)yearL+=100;
    // ����������ֻ��1900��֮���  
    temp2=yearL+yearL/4;
    temp2=temp2%7; 
    temp2=temp2+day+table_week[month-1];
    if (yearL%4==0&&month<3)temp2--;
    return(temp2%7);
} 
//�Ƚ������ַ���ָ�����ȵ������Ƿ����
//����:s1,s2Ҫ�Ƚϵ������ַ���;len,�Ƚϳ���
//����ֵ:1,���;0,�����
u8 str_cmpx(u8*s1,u8*s2,u8 len)
{
    u8 i;
    for(i=0;i<len;i++)if((*s1++)!=*s2++)return 0;
    return 1;       
}
const u8 *COMPILED_DATE;//��ñ�������
const u8 *COMPILED_TIME;//��ñ���ʱ��
const u8 Month_Tab[12][3]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"}; 
//�Զ�����ʱ��Ϊ������ʱ��   
void Auto_Time_Set(void)
{
    u8 temp[3];
    u8 i;
    u8 mon,date;
    u16 year;
    u8 sec,min,hour;
    for(i=0;i<3;i++)temp[i]=COMPILED_DATE[i];   
    for(i=0;i<12;i++)if(str_cmpx((u8*)Month_Tab[i],temp,3))break;    
    mon=i+1;//�õ��·�
    if(COMPILED_DATE[4]==' ')date=COMPILED_DATE[5]-'0'; 
    else date=10*(COMPILED_DATE[4]-'0')+COMPILED_DATE[5]-'0';  
    year=1000*(COMPILED_DATE[7]-'0')+100*(COMPILED_DATE[8]-'0')+10*(COMPILED_DATE[9]-'0')+COMPILED_DATE[10]-'0';       
    hour=10*(COMPILED_TIME[0]-'0')+COMPILED_TIME[1]-'0';  
    min=10*(COMPILED_TIME[3]-'0')+COMPILED_TIME[4]-'0';  
    sec=10*(COMPILED_TIME[6]-'0')+COMPILED_TIME[7]-'0';  
    RTC_Set(year,mon,date,hour,min,sec)    ;
    //printf("%d-%d-%d  %d:%d:%d\n",year,mon,date,hour,min,sec);
} 



















