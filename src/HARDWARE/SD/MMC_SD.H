#ifndef _MMC_SD_H_
#define _MMC_SD_H_              
#include <stm32f10x_lib.h> 
//////////////////////////////////////////////////////////////////////////////////     
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//SD�� ��������           
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/11/28 
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************
//V1.1�޸�˵��
//2010/5/13                                       
//������һЩ��ʱ,ʵ�����֧��TF��(1G/2G),��ʿ��2G,4G 16G SD��
//2010/6/24
//������u8 SD_GetResponse(u8 Response)����
//�޸���u8 SD_WaitDataReady(void)����
//������USB������֧�ֵ�u8 MSD_ReadBuffer(u8* pBuffer, u32 ReadAddr, u32 NumByteToRead);
//��u8 MSD_WriteBuffer(u8* pBuffer, u32 WriteAddr, u32 NumByteToWrite);��������
//////////////////////////////////////////////////////////////////////////////////       


//SD�������ݽ������Ƿ��ͷ����ߺ궨��  
#define NO_RELEASE      0
#define RELEASE         1         
// SD�����Ͷ���  
#define SD_TYPE_MMC     0
#define SD_TYPE_V1      1
#define SD_TYPE_V2      2
#define SD_TYPE_V2HC    4       
// SD��ָ���         
#define CMD0    0       //����λ
#define CMD1    1
#define CMD9    9       //����9 ����CSD����
#define CMD10   10      //����10����CID����
#define CMD12   12      //����12��ֹͣ���ݴ���
#define CMD16   16      //����16������SectorSize Ӧ����0x00
#define CMD17   17      //����17����sector
#define CMD18   18      //����18����Multi sector
#define ACMD23  23      //����23�����ö�sectorд��ǰԤ�Ȳ���N��block
#define CMD24   24      //����24��дsector
#define CMD25   25      //����25��дMulti sector
#define ACMD41  41      //����41��Ӧ����0x00
#define CMD55   55      //����55��Ӧ����0x01
#define CMD58   58      //����58����OCR��Ϣ
#define CMD59   59      //����59��ʹ��/��ֹCRC��Ӧ����0x00
//����д���Ӧ������
#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF
//SD����Ӧ�����
#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF
                                                              
//�ⲿ��Ӧ���ݾ�����������޸�!
//Mini STM32ʹ�õ���PA3��ΪSD����CS��.
#define    SD_CS  PAout(3) //SD��Ƭѡ����                              

extern u8  SD_Type;//SD��������
//���������� 
u8 SD_WaitReady(void);                          //�ȴ�SD������
u8 SD_SendCommand(u8 cmd, u32 arg, u8 crc);     //SD������һ������
u8 SD_SendCommand_NoDeassert(u8 cmd, u32 arg, u8 crc);
u8 SD_Init(void);                               //SD����ʼ��
u8 SD_Idle_Sta(void);                           //����SD��������ģʽ

u8 SD_ReceiveData(u8 *data, u16 len, u8 release);//SD��������
u8 SD_GetCID(u8 *cid_data);                     //��SD��CID
u8 SD_GetCSD(u8 *csd_data);                     //��SD��CSD
u32 SD_GetCapacity(void);                       //ȡSD������
//USB ������ SD����������
u8 MSD_WriteBuffer(u8* pBuffer, u32 WriteAddr, u32 NumByteToWrite);
u8 MSD_ReadBuffer(u8* pBuffer, u32 ReadAddr, u32 NumByteToRead);

u8 SD_ReadSingleBlock(u32 sector, u8 *buffer);  //��һ��sector
u8 SD_WriteSingleBlock(u32 sector, const u8 *buffer);         //дһ��sector
u8 SD_ReadMultiBlock(u32 sector, u8 *buffer, u8 count);     //�����sector
u8 SD_WriteMultiBlock(u32 sector, const u8 *data, u8 count);//д���sector
u8 SD_Read_Bytes(unsigned long address,unsigned char *buf,unsigned int offset,unsigned int bytes);//��ȡһbyte
#endif




