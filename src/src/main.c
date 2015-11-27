/**
  ******************************************************************************
  * FILE      main.c
  * AUTH      M.Phuong
  * Revision  v1.0
  * DATE      14/01/2013
  * DESC      USB Host HID Mouse, Keyboard demo
  ******************************************************************************
  */

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "usb_bsp.h"
#include "usbh_core.h"
#include "usbh_usr.h"
#include "usbh_hid_core.h"
#include "uart_debug.h"
#include "stm32f4xx_conf.h"
#include "debug.h"
#include "stm32f4lib.h"
#include "commu.h"
#include "rtthread.h"
/* Private macro */
/* Private variables */





#ifdef USE_FULL_ASSERT
/**
* @brief  assert_failed
*         Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  File: pointer to the source file name
* @param  Line: assert_param error line source number
* @retval None
*/


#endif

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 1 ms.
  * @retval None
  */
//void Delay(__IO uint32_t nTime)
//{
//    TimingDelay = nTime;

//    while(nTime != 0)
//	{
//		nTime--;
////TimingDelay_Decrement();
//	}
//}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
//void TimingDelay_Decrement(void)
//{
//    if (TimingDelay != 0x00)
//    {
//        TimingDelay--;
//    }
//}


