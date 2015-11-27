/**
  ******************************************************************************
  * @file    usbh_hid_mouse.c 
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    19-March-2012
  * @brief   This file is the application layer for USB Host HID Mouse Handling.                  
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "usbh_hid_mouse.h"


/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_CLASS
  * @{
  */

/** @addtogroup USBH_HID_CLASS
  * @{
  */
  
/** @defgroup USBH_HID_MOUSE 
  * @brief    This file includes HID Layer Handlers for USB Host HID class.
  * @{
  */ 

/** @defgroup USBH_HID_MOUSE_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBH_HID_MOUSE_Private_Defines
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBH_HID_MOUSE_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USBH_HID_MOUSE_Private_FunctionPrototypes
  * @{
  */ 
static void  MOUSE_Init (void);
static void  MOUSE_Decode(uint8_t *data);
/**
  * @}
  */ 


/** @defgroup USBH_HID_MOUSE_Private_Variables
  * @{
  */
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
 #if defined   (__CC_ARM) /*!< ARM Compiler */
  __align(4) 
 #elif defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4
 #elif defined (__GNUC__) /*!< GNU Compiler */
 #pragma pack(4) 
 #elif defined  (__TASKING__) /*!< TASKING Compiler */                           
  __align(4) 
 #endif /* __CC_ARM */
#endif
 
    
/////////////////////////////////
volatile uint8_t data_mouse_x = 0;
volatile uint8_t  data_mouse_y = 0;
volatile uint8_t  data_mouse_buttons = 0;
/////////////////////////////////    
 
HID_MOUSE_Data_TypeDef HID_MOUSE_Data;
HID_cb_TypeDef HID_MOUSE_cb = 
{
  MOUSE_Init,
  MOUSE_Decode,
};
/**
  * @}
  */ 


/** @defgroup USBH_HID_MOUSE_Private_Functions
  * @{
  */ 

/**
* @brief  MOUSE_Init
*         Init Mouse State.
* @param  None
* @retval None
*/
static void  MOUSE_Init ( void)
{
 /* Call User Init*/
 USR_MOUSE_Init();
}
            
/**
* @brief  MOUSE_Decode
*         Decode Mouse data
* @param  data : Pointer to Mouse HID data buffer
* @retval None 
*/

static void  MOUSE_Decode(uint8_t *data)
{
    char buff[4];
    buff[0]=data[1];
    buff[1]=data[3];
    if(((u8)data[4]&0x0f)==14||(((u8)data[4]&0x0f)==15&&buff[1]<128))
    {
    buff[1]=130;
    }
    else if(((u8)data[4]&0x0f)==1||(((u8)data[4]&0x0f)==0&&buff[1]>127))
    {
    buff[1]=124;

    }
    if(((signed char)(data[5]))>=0)
    {
    (data[5])&=(u8)(0xff-(1<<3));
    }
    else
    {
    (data[5])|=(u8)((1<<3));
    }
    buff[2]=((u8)data[4]>>4)+((u8)(data[5])<<4);
    buff[3]=data[6];
    
    
  HID_MOUSE_Data.button = (s8)buff[0];

  HID_MOUSE_Data.x      = (s8)buff[1];
  HID_MOUSE_Data.y      = (s8)buff[2];
  HID_MOUSE_Data.z      = (s8)buff[3];
     
  USR_MOUSE_ProcessData(&HID_MOUSE_Data);

}
void mouse_code(u8 * mouse_data)
{
    mouse_data[0]=mouse_data[0];
    mouse_data[1]=mouse_data[2];
    mouse_data[2]=mouse_data[4];
    mouse_data[3]=mouse_data[6];
    mouse_data[4]=0;
    mouse_data[5]=0;
    mouse_data[6]=0;
    mouse_data[7]=90;
}
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */


/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
