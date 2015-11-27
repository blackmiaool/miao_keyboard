/************************************************************************************
**	��Ȩ���У�Copyright (c) 2005 - 2010 ICRoute INC.
** Ӳ��˵����STM32f103vet6  8MHZ  �ڲ�ʱ��Ϊ72MHZ 
** 					 LD3320 ��Դ���� 12MHZ  ��ͬ�ľ������޸�LDChip.h �еĶ���ֵ����define CLK_IN  12��
** ���˵�������ж�д�Ĵ�����ʽ�������������ʽ���޸�Reg_RW.c �ļ����ɣ�
** �޸���:  http://shop64790118.taobao.com/
** ʱ�䣺2011.10.15
** ˵�����������޸��� ICRoute��˾ ���׵�Դ���򣬰�Ȩ�������С�
************************************************************************************/

#ifndef LD_CHIP_H
#define LD_CHIP_H
#include "stm32f4lib.h"
//#define uint8 unsigned char
//#define uint16 unsigned int
#define uint32 unsigned long
#define ld3320_print(arg...) {do{if(ini.Debug.audio){DBG(arg);}else{ no_use_printf(arg);}}while(0);}
#define ld3320_putchar(arg) {do{if(ini.Debug.audio){putchar(arg);}else{no_use_putchar(arg);}}while(0);}
//	��������״̬����������¼������������ASRʶ����������MP3����
#define LD_MODE_IDLE		0x00
#define LD_MODE_ASR_RUN		0x08
#define LD_MODE_MP3		 	0x40


//	�������״̬����������¼������������ASRʶ������е��ĸ�״̬
#define LD_ASR_NONE				0x00	//	��ʾû������ASRʶ��
#define LD_ASR_RUNING			0x01	//	��ʾLD3320������ASRʶ����
#define LD_ASR_FOUNDOK			0x10	//	��ʾһ��ʶ�����̽�������һ��ʶ����
#define LD_ASR_FOUNDZERO 		0x11	//	��ʾһ��ʶ�����̽�����û��ʶ����
#define LD_ASR_ERROR	 		0x31	//	��ʾһ��ʶ��������LD3320оƬ�ڲ����ֲ���ȷ��״̬


#define CLK_IN   	30/* user need modify this value according to clock in */
#define LD_PLL_11			(uint8)((CLK_IN/2.0)-1)
#define LD_PLL_MP3_19		0x0f
#define LD_PLL_MP3_1B		0x18
#define LD_PLL_MP3_1D   	(uint8)(((90.0*((LD_PLL_11)+1))/(CLK_IN))-1)

#define LD_PLL_ASR_19 		(uint8)(CLK_IN*32.0/(LD_PLL_11+1) - 0.51)
#define LD_PLL_ASR_1B 		0x48
#define LD_PLL_ASR_1D 		0x1f

// LD chip fixed values.
#define        RESUM_OF_MUSIC               0x01
#define        CAUSE_MP3_SONG_END           0x20

#define        MASK_INT_SYNC				0x10
#define        MASK_INT_FIFO				0x04
#define    	   MASK_AFIFO_INT				0x01
#define        MASK_FIFO_STATUS_AFULL		0x08
struct sttsound{
	uint32 lenth;
	uint32 addr_start;
};

void sound_play(u8 index);
void LD_reset(void);
void  LD3320_main_Init(void);
void LD_Init_Common(void);
void LD_Init_MP3(void);//zarien...
void LD_Init_ASR(void);

void LD_play(uint8 index);//zarien...
void LD_AdjustMIX2SPVolume(uint8 value);//zarien...
void LD_ReloadMp3Data(uint8 index);
void LD_ReloadMp3Data_2(void);

uint8 LD_ProcessAsr(uint32 RecogAddr);
void LD_AsrStart(void);
uint8 LD_AsrRun(void);
uint8 LD_AsrAddFixed(void);
uint8 LD_GetResult(void);
void sound_Init(void );

void LD_ReadMemoryBlock(uint8 dev, uint8 * ptr, uint32 addr, uint8 count);
void LD_WriteMemoryBlock(uint8 dev, uint8 * ptr, uint32 addr, uint8 count);
void LD3320_Init(void);
//*******************************************
extern uint32 nMp3StartPos;
extern uint32 nMp3Size;			// zarien...
extern uint32 nMp3Pos;
extern uint8 bMp3Play;
extern uint8 nDemoFlag;
//*******************************************
extern uint8  nLD_Mode;


////ʶ���루�ͻ��޸Ĵ���
//#define CODE_QJ	1	 /*ǰ��*/
//#define CODE_HT	2	 /*����*/
//#define CODE_JS	3	 /*����*/
//#define CODE_TT	4	 /*̧��*/

//23��ʶ���루�޸ĵط���
#define CODE_QJ	1	 /*���*/
#define CODE_HT	2	 /*�������ҽ���*/
#define CODE_KNM	3	 /*����������*/
#define CODE_YMD	4	 /*չʾ����*/
#define CODE_A 5 /*����������*/
#define CODE_B 6  /*��ᳪ����*/

#define CODE_C 7	 /*�������°�*/
#define CODE_D 8 /*�й�����*/
#define CODE_E 9 /*�������*/
#define CODE_F 10	 /*ǰ��*/
#define CODE_G 11 /*����*/
#define CODE_H 12  /*���*/

#define CODE_I 13	 /*����ת*/
#define CODE_J 14 /*����ת*/
#define CODE_K 15 /*��������*/
#define CODE_L 16 /*��������*/
#define CODE_M 17 /*�ټ�*/
#define CODE_N 18  /*ѧӢ��*/
			 
#define CODE_O 19	 /*����֪ʶ*/
#define CODE_P 20 /*�밡*/
#define CODE_Q 21 /*����*/

#define CODE_R 22/*����*/
#define CODE_S 23/*�����ش�*/
#define CODE_T 24


#define MIC_VOL 0x43
#endif
