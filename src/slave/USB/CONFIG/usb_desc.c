/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_desc.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Descriptors for Joystick Mouse Demo
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h"
u8 dev_sel=0;//0:mouse 1:sd
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* USB Standard Device Descriptor */
const u8 Joystick_DeviceDescriptor[JOYSTICK_SIZ_DEVICE_DESC] =
  {
    0x12,                       /*bLength */
    USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType*/
    0x00,                       /*bcdUSB */
    0x02,
    0x00,                       /*bDeviceClass*/
    0x00,                       /*bDeviceSubClass*/
    0x00,                       /*bDeviceProtocol*/
    0x40,                       /*bMaxPacketSize40*/
    0x83,                       /*idVendor (0x1234)*/
    0x04,
    0x50,                       /*idProduct = 0x4321*/
    0x57,
    0x00,                       /*bcdDevice rel. 2.00*/
    0x02,
    1,                          /*Index of string descriptor describing
                                              manufacturer */
    2,                          /*Index of string descriptor describing
                                             product*/
    3,                          /*Index of string descriptor describing the
                                             device serial number */
    0x01                        /*bNumConfigurations*/
  }
  ; /* Joystick_DeviceDescriptor */


const u8 Joystick_ConfigDescriptor[JOYSTICK_SIZ_CONFIG_DESC] =
{
    /***************����������***********************/
    0x09,		//bLength�ֶ�
    USB_CONFIGURATION_DESCRIPTOR_TYPE,		//bDescriptorType�ֶ�
    //wTotalLength�ֶ�
    JOYSTICK_SIZ_CONFIG_DESC%128,
    /* wTotalLength: Bytes returned */
    JOYSTICK_SIZ_CONFIG_DESC/128,

    0x03,	//bNumInterfaces�ֶ�
    0x01,	//bConfiguration�ֶ�
    0x00,	//iConfigurationz�ֶ�
    0x80,	//bmAttributes�ֶ�
    0x32,	//bMaxPower�ֶ�

    /*******************��һ���ӿ�������*********************/
    0x09,	//bLength�ֶ�
    0x04,	//bDescriptorType�ֶ�
    0x00,	//bInterfaceNumber�ֶ�
    0x00,	//bAlternateSetting�ֶ�
    0x02,	//bNumEndpoints�ֶ�
    0x03,	//bInterfaceClass�ֶ�
    0x01,	//bInterfaceSubClass�ֶ�
    0x01,	//bInterfaceProtocol�ֶ�
    0x00,	//iConfiguration�ֶ�

    /******************HID������************************/
    0x09,	//bLength�ֶ�
    0x21,	//bDescriptorType�ֶ�
    0x10,	//bcdHID�ֶ�
    0x01,
    0x21,	//bCountyCode�ֶ�
    0x01,	//bNumDescriptors�ֶ�
    0x22,	//bDescriptorType�ֶ�

    //bDescriptorLength�ֶΡ�
    //�¼��������ĳ��ȡ��¼�������Ϊ���̱�����������
    sizeof(KeyboardReportDescriptor)&0xFF,
    (sizeof(KeyboardReportDescriptor)>>8)&0xFF,

    /**********************����˵�������***********************/
    0x07,	//bLength�ֶ�
    0x05,	//bDescriptorType�ֶ�
    0x81,	//bEndpointAddress�ֶ�
    0x03,	//bmAttributes�ֶ�
    0x10,	//wMaxPacketSize�ֶ�
    0x00,
    0x0A,	//bInterval�ֶ�

    /**********************����˵�������***********************/
    0x07,	//bLength�ֶ�
    0x05,	//bDescriptorType�ֶ�
    0x01,	//bEndpointAddress�ֶ�
    0x03,	//bmAttributes�ֶ�
    0x10,	//wMaxPacketSize�ֶ�
    0x00,
    0x0A,	//bInterval�ֶ�

    /*******************�ڶ����ӿ�������*********************/
    0x09,	//bLength�ֶ�
    0x04,	//bDescriptorType�ֶ�
    0x01,	//bInterfaceNumber�ֶ�
    0x00,	//bAlternateSetting�ֶ�
    0x01,	//bNumEndpoints�ֶ�
    0x03,	//bInterfaceClass�ֶ�
    0x01,	//bInterfaceSubClass�ֶ�
    0x02,	//bInterfaceProtocol�ֶ�
    0x00,	//iConfiguration�ֶ�

    /******************HID������************************/
    0x09,	//bLength�ֶ�
    0x21,	//bDescriptorType�ֶ�
    0x10,	//bcdHID�ֶ�
    0x01,
    0x21,	//bCountyCode�ֶ�
    0x01,	//bNumDescriptors�ֶ�
    0x22,	//bDescriptorType�ֶ�
    sizeof(MouseReportDescriptor)&0xFF,		//bDescriptorLength�ֶ�
    (sizeof(MouseReportDescriptor)>>8)&0xFF,

    /**********************����˵�������***********************/
    0x07,	//bLength�ֶ�
    0x05,	//bDescriptorType�ֶ�
    0x82,	//bEndpointAddress�ֶ�
    0x03,	//bmAttributes�ֶΡ�D1~D0Ϊ�˵㴫������ѡ��
    0x40,	//wMaxPacketSize�ֶ�
    0x00,
    0x0A, 	//bInterval�ֶ�

    /*******************�������ӿ�������*********************/
    0x09,   /* bLength: Interface Descriptor size */
    0x04,   /* bDescriptorType: */
    /*      Interface descriptor type */
    0x02,   /* bInterfaceNumber: Number of Interface */
    0x00,   /* bAlternateSetting: Alternate setting */
    0x02,   /* bNumEndpoints*/
    0x08,   /* bInterfaceClass: MASS STORAGE Class */
    0x06,   /* bInterfaceSubClass : SCSI transparent*/
    0x50,   /* nInterfaceProtocol */
    4,          /* iInterface: */
    /* 18 */
    0x07,   /*Endpoint descriptor length = 7*/
    0x05,   /*Endpoint descriptor type */
    0x83,   /*Endpoint address (IN, address 1) */
    0x02,   /*Bulk endpoint type */
    0x40,   /*Maximum packet size (64 bytes) */
    0x00,
    0x00,   /*Polling interval in milliseconds */
    /* 25 */
    0x07,   /*Endpoint descriptor length = 7 */
    0x05,   /*Endpoint descriptor type */
    0x04,   /*Endpoint address (OUT, address 2) */
    0x02,   /*Bulk endpoint type */
    0x40,   /*Maximum packet size (64 bytes) */
    0x00,
    0x00,     /*Polling interval in milliseconds*/
    /*32*/
    
    
    
    
    
    
    
    
    
//    
//    0x09,   /* bLength: Interface Descriptor size */
//    USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: Interface */
//    /* Interface descriptor type */
//    0x03,   /* bInterfaceNumber: Number of Interface */
//    0x00,   /* bAlternateSetting: Alternate setting */
//    0x01,   /* bNumEndpoints: One endpoints used */
//    0x02,   /* bInterfaceClass: Communication Interface Class */
//    0x02,   /* bInterfaceSubClass: Abstract Control Model */
//    0x01,   /* bInterfaceProtocol: Common AT commands */
//    0x00,   /* iInterface: */
//    
//    /*Header Functional Descriptor*/
//    0x05,   /* bLength: Endpoint Descriptor size */
//    0x24,   /* bDescriptorType: CS_INTERFACE */
//    0x00,   /* bDescriptorSubtype: Header Func Desc */
//    0x10,   /* bcdCDC: spec release number */
//    0x01,
//    
//    /*Call Managment Functional Descriptor*/
//    0x05,   /* bFunctionLength */
//    0x24,   /* bDescriptorType: CS_INTERFACE */
//    0x01,   /* bDescriptorSubtype: Call Management Func Desc */
//    0x00,   /* bmCapabilities: D0+D1 */
//    0x04,   /* bDataInterface: 1 */
//    
//    /*ACM Functional Descriptor*/
//    0x04,   /* bFunctionLength */
//    0x24,   /* bDescriptorType: CS_INTERFACE */
//    0x02,   /* bDescriptorSubtype: Abstract Control Management desc */
//    0x02,   /* bmCapabilities */
//    
//    
//    /*Union Functional Descriptor*/
//    0x05,   /* bFunctionLength */
//    0x24,   /* bDescriptorType: CS_INTERFACE */
//    0x06,   /* bDescriptorSubtype: Union func desc */
//    0x03,   /* bMasterInterface: Communication class interface */
//    0x04,   /* bSlaveInterface0: Data Class Interface */
//    
//    
//    /*Endpoint 2 Descriptor*/
//    0x07,   /* bLength: Endpoint Descriptor size */
//    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
//    0x86,   /* bEndpointAddress: (IN2) */
//    0x03,   /* bmAttributes: Interrupt */
//    VIRTUAL_COM_PORT_INT_SIZE,      /* wMaxPacketSize: */
//    0x00,
//    0xFF,   /* bInterval: */
//    
//    
//    
//    
//    /*Data class interface descriptor*/
//    0x09,   /* bLength: Endpoint Descriptor size */
//    USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: */
//    0x04,   /* bInterfaceNumber: Number of Interface */
//    0x00,   /* bAlternateSetting: Alternate setting */
//    0x02,   /* bNumEndpoints: Two endpoints used */
//    0x0A,   /* bInterfaceClass: CDC */
//    0x00,   /* bInterfaceSubClass: */
//    0x00,   /* bInterfaceProtocol: */
//    0x01,   /* iInterface: */
//    
//    /*Endpoint 3 Descriptor*/
//    0x07,   /* bLength: Endpoint Descriptor size */
//    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
//    0x07,   /* bEndpointAddress: (OUT3) */
//    0x02,   /* bmAttributes: Bulk */
//    VIRTUAL_COM_PORT_DATA_SIZE,             /* wMaxPacketSize: */
//    0x00,
//    0x00,   /* bInterval: ignore for Bulk transfer */
//    
//    /*Endpoint 1 Descriptor*/
//    0x07,   /* bLength: Endpoint Descriptor size */
//    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
//    0x85,   /* bEndpointAddress: (IN1) */
//    0x02,   /* bmAttributes: Bulk */
//    VIRTUAL_COM_PORT_DATA_SIZE,             /* wMaxPacketSize: */
//    0x00,
//    0x00    /* bInterval */
};
////////////////////////�����������������//////////////////////////
//USB���̱����������Ķ���
const u8 KeyboardReportDescriptor[KP_ReportDescriptor_Size]=
{
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)	//63
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
    0x95, 0x05,                    //   REPORT_COUNT (5)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x03,                    //   REPORT_SIZE (3)
    0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xc0,                           // END_COLLECTION
	//0xc0,
  }; /* Joystick_ReportDescriptor */
///////////////////////////���̱������������////////////////////////////

//USB��걨���������Ķ���
const u8 MouseReportDescriptor[Mouse_ReportDescriptor_Size]=
{
 0x05, 0x01, // USAGE_PAGE (Generic Desktop)
 0x09, 0x02, // USAGE (Mouse)
 0xa1, 0x01, // COLLECTION (Application)
 0x85, 0x01, //Report ID (1)
 0x09, 0x01, //   USAGE (Pointer)
 0xa1, 0x00, //   COLLECTION (Physical)
 0x05, 0x09, //     USAGE_PAGE (Button)
 0x19, 0x01, //     USAGE_MINIMUM (Button 1)
 0x29, 0x03, //     USAGE_MAXIMUM (Button 3)
 0x15, 0x00, //     LOGICAL_MINIMUM (0)
 0x25, 0x01, //     LOGICAL_MAXIMUM (1)
 0x95, 0x03, //     REPORT_COUNT (3)
 0x75, 0x01, //     REPORT_SIZE (1)
 0x81, 0x02, //     INPUT (Data,Var,Abs)
 0x95, 0x01, //     REPORT_COUNT (1)
 0x75, 0x05, //     REPORT_SIZE (5)
 0x81, 0x03, //     INPUT (Cnst,Var,Abs)
 0x05, 0x01, //     USAGE_PAGE (Generic Desktop)
 0x09, 0x30, //     USAGE (X)
 0x09, 0x31, //     USAGE (Y)
 0x09, 0x38, //     USAGE (Wheel)
 0x15, 0x81, //     LOGICAL_MINIMUM (-127)
 0x25, 0x7f, //     LOGICAL_MAXIMUM (127)
 0x75, 0x08, //     REPORT_SIZE (8)
 0x95, 0x03, //     REPORT_COUNT (3)
 0x81, 0x06, //     INPUT (Data,Var,Rel)
 0xc0,       //   END_COLLECTION
 0xc0        // END_COLLECTION
};
///////////////////////////�������������////////////////////////////
/* USB String Descriptors (optional) */
const u8 Joystick_StringLangID[JOYSTICK_SIZ_STRING_LANGID] =
  {
    JOYSTICK_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04
  }
  ; /* LangID = 0x0409: U.S. English */

const u8 Joystick_StringVendor[JOYSTICK_SIZ_STRING_VENDOR] =
  {
    JOYSTICK_SIZ_STRING_VENDOR, /* Size of Vendor string */
    USB_STRING_DESCRIPTOR_TYPE,  /* bDescriptorType*/
    /* Manufacturer: "STMicroelectronics" */
    'S', 0, 'T', 0, 'M', 0, 'i', 0, 'c', 0, 'r', 0, 'o', 0, 'e', 0,
    'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0, 'o', 0, 'n', 0, 'i', 0,
    'c', 0, 's', 0
  };

const u8 Joystick_StringProduct[JOYSTICK_SIZ_STRING_PRODUCT] =
  {
    JOYSTICK_SIZ_STRING_PRODUCT,          /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, ' ', 0, 'J', 0,
    'o', 0, 'y', 0, 's', 0, 't', 0, 'i', 0, 'c', 0, 'k', 0
  };
u8 Joystick_StringSerial[JOYSTICK_SIZ_STRING_SERIAL] =
  {
    JOYSTICK_SIZ_STRING_SERIAL,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, '1', 0, '0', 0
  };


/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

