/**
*****************************************************************************
**
**  File        : stm32f4xx_it.c
**
**  Abstract    : Main Interrupt Service Routines.
**                This file provides template for all exceptions handler and
**                peripherals interrupt service routine.
**
**  Environment : Atollic TrueSTUDIO(R)
**                STMicroelectronics STM32F4xx Standard Peripherals Library
**
**  Distribution: The file is distributed
**                of any kind.
**
**  (c)Copyright Atollic AB.
**  You may use this file as-is or modify it according to the needs of your
**  project. Distribution of this file (unmodified or modified) is not
**  permitted. Atollic AB permit registered Atollic TrueSTUDIO(R) users the
**  rights to distribute the assembled, compiled & linked contents of this
**  file as part of an application binary file, provided that it is built
**  using the Atollic TrueSTUDIO(R) toolchain.
**
**
*****************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "usb_bsp.h"
#include "usb_hcd_int.h"
#include "usbh_core.h"
#include "stm32f4xx_it.h"
#include <rtthread.h>
#include "board.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern USB_OTG_CORE_HANDLE          USB_OTG_Core_dev;
extern USBH_HOST                    USB_Host;
/* Private function prototypes -----------------------------------------------*/

extern void TimingDelay_Decrement(void);
extern void USB_OTG_BSP_TimerIRQ (void);

/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
//void HardFault_Handler(void)
//{
//  /* Go to infinite loop when Hard Fault exception occurs */
//  while (1)
//  {
//  }
//}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//void SysTick_Handler(void)
//{
//    TimingDelay_Decrement();
//}
/**
  * @brief  TIM2_IRQHandler
  *         This function handles Timer2 Handler.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
  //USB_OTG_BSP_TimerIRQ(&USB_OTG_Core_dev);
	USB_OTG_BSP_TimerIRQ();
}

/**
  * @brief  OTG_FS_IRQHandler
  *          This function handles USB-On-The-Go FS global interrupt request.
  *          requests.
  * @param  None
  * @retval None
  */
#ifdef USE_USB_OTG_FS
void OTG_FS_IRQHandler(void) {
#else
void OTG_HS_IRQHandler(void) {
#endif
	
  USBH_OTG_ISR_Handler(&USB_OTG_Core_dev);
}
void USART1_IRQHandler(void)
{
#ifdef RT_USING_UART1
    extern struct rt_device uart1_device;
	extern void rt_hw_serial_isr(struct rt_device *device);

    /* enter interrupt */
    rt_interrupt_enter();

    rt_hw_serial_isr(&uart1_device);

    /* leave interrupt */
    rt_interrupt_leave();
#endif
}
extern u32 test_data;
extern u32 commu_recv_handle(u8 data);
void USART2_IRQHandler(void)
{
#ifdef RT_USING_UART2
    extern struct rt_device uart2_device;
	extern void rt_hw_serial_isr(struct rt_device *device);

    /* enter interrupt */
    rt_interrupt_enter();
	
//  rt_hw_serial_isr(&uart2_device);
//	rt_kprintf("%X",USART2->SR);
	
	if(USART2->SR&(1<<5))//接收到数据
	{	 
		commu_recv_handle(USART2->DR);
	}
    /* leave interrupt */
    rt_interrupt_leave();
#endif
}

void USART3_IRQHandler(void)
{
#ifdef RT_USING_UART3
    extern struct rt_device uart3_device;
	extern void rt_hw_serial_isr(struct rt_device *device);

    /* enter interrupt */
    rt_interrupt_enter();

    rt_hw_serial_isr(&uart3_device);

    /* leave interrupt */
    rt_interrupt_leave();
#endif
}
#define RT_USING_UART6
void USART6_IRQHandler(void)
{
#ifdef RT_USING_UART6
    extern struct rt_device uart6_device;
	extern void rt_hw_serial_isr(struct rt_device *device);
    
    /* enter interrupt */
    rt_interrupt_enter();

    rt_hw_serial_isr(&uart6_device);

    /* leave interrupt */
    rt_interrupt_leave();
#endif
}










