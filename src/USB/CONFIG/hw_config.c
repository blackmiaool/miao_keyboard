/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : hw_config.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Hardware Configuration & Setup
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "sys.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "platform_config.h"
#include "usb_pwr.h"
#include "mass_mal.h"
//static u8 LED1;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
 
 
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
              

//配置USB时钟,USBclk=48Mhz
void Set_USBClock(void)
{
     RCC->CFGR&=~(1<<22); //USBclk=PLLclk/1.5=48Mhz        
    RCC->APB1ENR|=1<<23; //USB时钟使能
}
 
/*******************************************************************************
* Function Name  : Enter_LowPowerMode.
* Description    : Power-off system clocks and power while entering suspend mode.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{
  /* Set the device state to suspend */
  bDeviceState = SUSPENDED;      
  /* Request to enter STOP mode with regulator in low power mode */
  //PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
}              

/*******************************************************************************
* Function Name  : Leave_LowPowerMode.
* Description    : Restores system clocks and power while exiting suspend mode.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Leave_LowPowerMode(void)
{
  DEVICE_INFO *pInfo = &Device_Info;
                                   
                                                       
  /* Set the device state to the correct state */
  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
  else
  {
    bDeviceState = ATTACHED;
  }
}

//USB中断配置
void USB_Interrupts_Config(void)
{
  
    EXTI->IMR|=1<<18;//  开启线18上的中断
     EXTI->RTSR|=1<<18;//line 18上事件上升降沿触发
    MY_NVIC_Init(1,1,USB_LP_CAN_RX0_IRQChannel,2);//组2，优先级次之
    MY_NVIC_Init(0,1,USBWakeUp_IRQChannel,2);     //组2，优先级最高
}
            
/*******************************************************************************
* Function Name : Joystick_Send.
* Description   : prepares buffer to be sent containing Joystick event infos.
* Input         : Keys: keys received from terminal.
* Output        : None.
* Return value  : None.
*******************************************************************************/

//void USB_Cable_Config (FunctionalState NewState)
//{
//  if (NewState != DISABLE)
//  {
//    LED1=1;
//  }
//  else
//  {
//    LED1=0;
//  }
//u16 MAL_Init(u8 lun)
//{
//  u16 status = MAL_OK;  
//  switch (lun)
//  {
//    case 0:                
//      break;               
//    case 1:                      
//      break;          
//    default:
//      return MAL_FAIL;
//  }
//  return status;
//}
void MAL_Config(void)
{
    MAL_Init(0);      
}

void USB_Disconnect_Config(void)
{                                             
//  GPIO_InitTypeDef GPIO_InitStructure;
//
//  /* Enable USB_DISCONNECT GPIO clock */
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_DISCONNECT, ENABLE);
//
//  /* USB_DISCONNECT_PIN used as USB pull-up */
//  GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
//  GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);
}
void Joystick_Send(u8 buf0, u8* buf1)
{
  //u8 Buffer[8] = {1, 8, 0, 1, 0, 0, 0, 0};
  
 // Buffer[0] = buf0;
  /* prepare buffer to send */
 // Buffer[2] = buf1;

  if(buf1[0]==0)    //键盘
  {
      buf1[0]=buf1[1];
      /*copy mouse position info in ENDP1 Tx Packet Memory Area*/
      UserToPMABufferCopy(buf1, GetEPTxAddr(ENDP1), 8);
      /* enable endpoint for transmission */
      SetEPTxValid(ENDP1);
  }
  else                //鼠标
  {
      
      UserToPMABufferCopy(buf1, GetEPTxAddr(ENDP2), 5);
      SetEPTxValid(ENDP2);
  }
                                                         
}                                                                 

/*******************************************************************************
* Function Name  : Get_SerialNum.
* Description    : Create the serial number string descriptor.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Get_SerialNum(void)
{
//  u32 Device_Serial0, Device_Serial1, Device_Serial2;

//  Device_Serial0 = *(u32*)(0x1FFFF7E8);
//  Device_Serial1 = *(u32*)(0x1FFFF7EC);
//  Device_Serial2 = *(u32*)(0x1FFFF7F0);

//  if (Device_Serial0 != 0)
//  {
//    Joystick_StringSerial[2] = (u8)(Device_Serial0 & 0x000000FF);
//    Joystick_StringSerial[4] = (u8)((Device_Serial0 & 0x0000FF00) >> 8);
//    Joystick_StringSerial[6] = (u8)((Device_Serial0 & 0x00FF0000) >> 16);
//    Joystick_StringSerial[8] = (u8)((Device_Serial0 & 0xFF000000) >> 24);

//    Joystick_StringSerial[10] = (u8)(Device_Serial1 & 0x000000FF);
//    Joystick_StringSerial[12] = (u8)((Device_Serial1 & 0x0000FF00) >> 8);
//    Joystick_StringSerial[14] = (u8)((Device_Serial1 & 0x00FF0000) >> 16);
//    Joystick_StringSerial[16] = (u8)((Device_Serial1 & 0xFF000000) >> 24);

//    Joystick_StringSerial[18] = (u8)(Device_Serial2 & 0x000000FF);
//    Joystick_StringSerial[20] = (u8)((Device_Serial2 & 0x0000FF00) >> 8);
//    Joystick_StringSerial[22] = (u8)((Device_Serial2 & 0x00FF0000) >> 16);
//    Joystick_StringSerial[24] = (u8)((Device_Serial2 & 0xFF000000) >> 24);
//  }
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
