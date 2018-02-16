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
#include "stdlib.h"

u8 dev_sel = 0; //0:mouse 1:sd
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
        0x00, /*bDeviceClass*/
        0x00, /*bDeviceSubClass*/
        0x00, /*bDeviceProtocol*/
        0x40, /*bMaxPacketSize40*/
        0x83, /*idVendor (0x1234)*/
        0x04,
        0x50, /*idProduct = 0x4321*/
        0x57,
        0x00, /*bcdDevice rel. 2.00*/
        0x02,
        1,   /*Index of string descriptor describing
                                              manufacturer */
        2,   /*Index of string descriptor describing
                                             product*/
        3,   /*Index of string descriptor describing the
                                             device serial number */
        0x01 /*bNumConfigurations*/
};           /* Joystick_DeviceDescriptor */
u8 *Joystick_ConfigDescriptor;
// const u8 Joystick_ConfigDescriptor[JOYSTICK_SIZ_CONFIG_DESC] =
//     {
//         /***************配置描述符***********************/
//         0x09,                              //bLength字段
//         USB_CONFIGURATION_DESCRIPTOR_TYPE, //bDescriptorType字段
//         //wTotalLength字段
//         JOYSTICK_SIZ_CONFIG_DESC % 128,
//         /* wTotalLength: Bytes returned */
//         JOYSTICK_SIZ_CONFIG_DESC / 128,

//         0x03, //bNumInterfaces字段
//         0x01, //bConfiguration字段
//         0x00, //iConfigurationz字段
//         0x80, //bmAttributes字段
//         0x32, //bMaxPower字段

//         /*******************第一个接口描述符*********************/
//         0x09, //bLength字段
//         0x04, //bDescriptorType字段
//         0x00, //bInterfaceNumber字段
//         0x00, //bAlternateSetting字段
//         0x02, //bNumEndpoints字段
//         0x03, //bInterfaceClass字段
//         0x01, //bInterfaceSubClass字段
//         0x01, //bInterfaceProtocol字段
//         0x00, //iConfiguration字段

//         /******************HID描述符************************/
//         0x09, //bLength字段
//         0x21, //bDescriptorType字段
//         0x10, //bcdHID字段
//         0x01,
//         0x21, //bCountyCode字段
//         0x01, //bNumDescriptors字段
//         0x22, //bDescriptorType字段

//         //bDescriptorLength字段。
//         //下级描述符的长度。下级描述符为键盘报告描述符。
//         sizeof(KeyboardReportDescriptor) & 0xFF,
//         (sizeof(KeyboardReportDescriptor) >> 8) & 0xFF,

//         /**********************输入端点描述符***********************/
//         0x07, //bLength字段
//         0x05, //bDescriptorType字段
//         0x81, //bEndpointAddress字段
//         0x03, //bmAttributes字段
//         0x10, //wMaxPacketSize字段
//         0x00,
//         0x0A, //bInterval字段

//         /**********************输出端点描述符***********************/
//         0x07, //bLength字段
//         0x05, //bDescriptorType字段
//         0x01, //bEndpointAddress字段
//         0x03, //bmAttributes字段
//         0x10, //wMaxPacketSize字段
//         0x00,
//         0x0A, //bInterval字段

//         /*******************第二个接口描述符*********************/
//         0x09, //bLength字段
//         0x04, //bDescriptorType字段
//         0x01, //bInterfaceNumber字段
//         0x00, //bAlternateSetting字段
//         0x01, //bNumEndpoints字段
//         0x03, //bInterfaceClass字段
//         0x01, //bInterfaceSubClass字段
//         0x02, //bInterfaceProtocol字段
//         0x00, //iConfiguration字段

//         /******************HID描述符************************/
//         0x09, //bLength字段
//         0x21, //bDescriptorType字段
//         0x10, //bcdHID字段
//         0x01,
//         0x21,                                 //bCountyCode字段
//         0x01,                                 //bNumDescriptors字段
//         0x22,                                 //bDescriptorType字段
//         sizeof(MouseReportDescriptor) & 0xFF, //bDescriptorLength字段
//         (sizeof(MouseReportDescriptor) >> 8) & 0xFF,

//         /**********************输入端点描述符***********************/
//         0x07, //bLength字段
//         0x05, //bDescriptorType字段
//         0x82, //bEndpointAddress字段
//         0x03, //bmAttributes字段。D1~D0为端点传输类型选择
//         0x40, //wMaxPacketSize字段
//         0x00,
//         0x0A, //bInterval字段

//         /*******************第三个接口描述符*********************/
//         0x09, /* bLength: Interface Descriptor size */
//         0x04, /* bDescriptorType: */
//         /*      Interface descriptor type */
//         0x02, /* bInterfaceNumber: Number of Interface */
//         0x00, /* bAlternateSetting: Alternate setting */
//         0x02, /* bNumEndpoints*/
//         0x08, /* bInterfaceClass: MASS STORAGE Class */
//         0x06, /* bInterfaceSubClass : SCSI transparent*/
//         0x50, /* nInterfaceProtocol */
//         4,    /* iInterface: */
//         /* 18 */
//         0x07, /*Endpoint descriptor length = 7*/
//         0x05, /*Endpoint descriptor type */
//         0x83, /*Endpoint address (IN, address 1) */
//         0x02, /*Bulk endpoint type */
//         0x40, /*Maximum packet size (64 bytes) */
//         0x00,
//         0x00, /*Polling interval in milliseconds */
//         /* 25 */
//         0x07, /*Endpoint descriptor length = 7 */
//         0x05, /*Endpoint descriptor type */
//         0x04, /*Endpoint address (OUT, address 2) */
//         0x02, /*Bulk endpoint type */
//         0x40, /*Maximum packet size (64 bytes) */
//         0x00,
//         0x00, /*Polling interval in milliseconds*/
//               /*32*/

// };
////////////////////////配置描述符集合完毕//////////////////////////
//USB键盘报告描述符的定义
const u8 KeyboardReportDescriptor[KP_ReportDescriptor_Size] =
    {
        0x05, 0x01,    // USAGE_PAGE (Generic Desktop)    //63
        0x09, 0x06,    // USAGE (Keyboard)
        0xa1, 0x01,    // COLLECTION (Application)
        0x05, 0x07,    //   USAGE_PAGE (Keyboard)
        0x19, 0xe0,    //   USAGE_MINIMUM (Keyboard LeftControl)
        0x29, 0xe7,    //   USAGE_MAXIMUM (Keyboard Right GUI)
        0x15, 0x00,    //   LOGICAL_MINIMUM (0)
        0x25, 0x01,    //   LOGICAL_MAXIMUM (1)
        0x75, 0x01,    //   REPORT_SIZE (1)
        0x95, 0x08,    //   REPORT_COUNT (8)
        0x81, 0x02,    //   INPUT (Data,Var,Abs)
        0x95, 0x01,    //   REPORT_COUNT (1)
        0x75, 0x08,    //   REPORT_SIZE (8)
        0x81, 0x03,    //   INPUT (Cnst,Var,Abs)
        0x95, 0x05,    //   REPORT_COUNT (5)
        0x75, 0x01,    //   REPORT_SIZE (1)
        0x05, 0x08,    //   USAGE_PAGE (LEDs)
        0x19, 0x01,    //   USAGE_MINIMUM (Num Lock)
        0x29, 0x05,    //   USAGE_MAXIMUM (Kana)
        0x91, 0x02,    //   OUTPUT (Data,Var,Abs)
        0x95, 0x01,    //   REPORT_COUNT (1)
        0x75, 0x03,    //   REPORT_SIZE (3)
        0x91, 0x03,    //   OUTPUT (Cnst,Var,Abs)
        0x95, 0x06,    //   REPORT_COUNT (6)
        0x75, 0x08,    //   REPORT_SIZE (8)
        0x15, 0x00,    //   LOGICAL_MINIMUM (0)
        0x26, 0xff, 0, //   LOGICAL_MAXIMUM (101)
        0x05, 0x07,    //   USAGE_PAGE (Keyboard)
        0x19, 0x00,    //   USAGE_MINIMUM (Reserved (no event indicated))
        0x29, 0xff,    //   USAGE_MAXIMUM (Keyboard Application)
        0x81, 0x00,    //   INPUT (Data,Ary,Abs)
        0xc0,          // END_COLLECTION
                       //0xc0,
};                     /* Joystick_ReportDescriptor */
///////////////////////////键盘报告描述符完毕////////////////////////////

//USB鼠标报告描述符的定义
const u8 MouseReportDescriptor[Mouse_ReportDescriptor_Size] =
    {
        0x05, 0x01,       // USAGE_PAGE (Generic Desktop)
        0x09, 0x02,       // USAGE (Mouse)
        0xa1, 0x01,       // COLLECTION (Application)
        0x85, 1,          //   REPORT_ID (1)
        0x09, 0x01,       //   USAGE (Pointer)
        0xa1, 0x00,       //   COLLECTION (Physical)
                          // ----------------------------  Buttons
        0x05, 0x09,       //     USAGE_PAGE (Button)
        0x19, 0x01,       //     USAGE_MINIMUM (Button 1)
        0x29, 0x05,       //     USAGE_MAXIMUM (Button 5)
        0x15, 0x00,       //     LOGICAL_MINIMUM (0)
        0x25, 0x01,       //     LOGICAL_MAXIMUM (1)
        0x75, 0x01,       //     REPORT_SIZE (1)
        0x95, 0x05,       //     REPORT_COUNT (5)
        0x81, 0x02,       //     INPUT (Data,Var,Abs)
        0x75, 0x03,       //     REPORT_SIZE (3)
        0x95, 0x01,       //     REPORT_COUNT (1)
        0x81, 0x03,       //     INPUT (Cnst,Var,Abs)
                          // ----------------------------  X,Y position
        0x05, 0x01,       //     USAGE_PAGE (Generic Desktop)
        0x09, 0x30,       //     USAGE (X)
        0x09, 0x31,       //     USAGE (Y)
        0x15, 0x81,       //     LOGICAL_MINIMUM (-127)
        0x25, 0x7f,       //     LOGICAL_MAXIMUM (127)
        0x75, 0x08,       //     REPORT_SIZE (8)
        0x95, 0x02,       //     REPORT_COUNT (2)
        0x81, 0x06,       //     INPUT (Data,Var,Rel)
                          // ----------------------------  Vertical wheel
        0x09, 0x38,       //     USAGE (Wheel)
        0x15, 0x81,       //     LOGICAL_MINIMUM (-127)
        0x25, 0x7f,       //     LOGICAL_MAXIMUM (127)
        0x35, 0x00,       //     PHYSICAL_MINIMUM (0)        - reset physical
        0x45, 0x00,       //     PHYSICAL_MAXIMUM (0)
        0x75, 0x08,       //     REPORT_SIZE (8)
        0x95, 0x01,       //     REPORT_COUNT (1)
        0x81, 0x06,       //     INPUT (Data,Var,Rel)
                          // ----------------------------  Horizontal wheel
        0x05, 0x0c,       //     USAGE_PAGE (Consumer Devices)
        0x0a, 0x38, 0x02, //     USAGE (AC Pan)
        0x15, 0x81,       //     LOGICAL_MINIMUM (-127)
        0x25, 0x7f,       //     LOGICAL_MAXIMUM (127)
        0x75, 0x08,       //     REPORT_SIZE (8)
        0x95, 0x01,       //     REPORT_COUNT (1)
        0x81, 0x06,       //     INPUT (Data,Var,Rel)
        0xc0,             //   END_COLLECTION
        0xc0,             // END_COLLECTION
        /* system control */
        0x05, 0x01,       // USAGE_PAGE (Generic Desktop)
        0x09, 0x80,       // USAGE (System Control)
        0xa1, 0x01,       // COLLECTION (Application)
        0x85, 2,          //   REPORT_ID (2)
        0x15, 0x01,       //   LOGICAL_MINIMUM (0x1)
        0x26, 0xb7, 0x00, //   LOGICAL_MAXIMUM (0xb7)
        0x19, 0x01,       //   USAGE_MINIMUM (0x1)
        0x29, 0xb7,       //   USAGE_MAXIMUM (0xb7)
        0x75, 0x10,       //   REPORT_SIZE (16)
        0x95, 0x01,       //   REPORT_COUNT (1)
        0x81, 0x00,       //   INPUT (Data,Array,Abs)
        0xc0,             // END_COLLECTION
        /* consumer */
        0x05, 0x0c,       // USAGE_PAGE (Consumer Devices)
        0x09, 0x01,       // USAGE (Consumer Control)
        0xa1, 0x01,       // COLLECTION (Application)
        0x85, 3,          //   REPORT_ID (3)
        0x15, 0x01,       //   LOGICAL_MINIMUM (0x1)
        0x26, 0x9c, 0x02, //   LOGICAL_MAXIMUM (0x29c)
        0x19, 0x01,       //   USAGE_MINIMUM (0x1)
        0x2a, 0x9c, 0x02, //   USAGE_MAXIMUM (0x29c)
        0x75, 0x10,       //   REPORT_SIZE (16)
        0x95, 0x01,       //   REPORT_COUNT (1)
        0x81, 0x00,       //   INPUT (Data,Array,Abs)
        0xc0,             // END_COLLECTION
};
///////////////////////////报告描述符完毕////////////////////////////
/* USB String Descriptors (optional) */
ONE_DESCRIPTOR *get_string_descriptor(char *str, u8 len)
{
  u8 desc_len = (len)*2 + 2;
  char *buf = (char *)malloc(desc_len);
  ONE_DESCRIPTOR *ret = (ONE_DESCRIPTOR *)malloc(sizeof(ONE_DESCRIPTOR));
  buf[0] = desc_len;
  buf[1] = USB_STRING_DESCRIPTOR_TYPE;
  for (int i = 0; i < len; i++)
  {
    buf[(i << 1) + 2] = str[i];
    buf[(i << 1) + 3] = 0;
  }
  ret->Descriptor_Size = desc_len;
  ret->Descriptor = (u8 *)buf;
  return ret;
}
extern ONE_DESCRIPTOR String_Descriptor[5];
extern ONE_DESCRIPTOR Config_Descriptor;
void init_string_descriptor()
{
  char lang_string[] = {0x09, 0x04}; /* LangID = 0x0409: U.S. English */
  String_Descriptor[0] = *get_string_descriptor(lang_string, sizeof lang_string);
  char vender_string[] = "blackmiaool";
  String_Descriptor[1] = *get_string_descriptor(vender_string, sizeof vender_string);
  char product_string[] = "miao keyboard";
  String_Descriptor[2] = *get_string_descriptor(product_string, sizeof product_string);
  char serial_string[] = "kb10";
  String_Descriptor[3] = *get_string_descriptor(serial_string, sizeof serial_string);
  String_Descriptor[4] = *get_string_descriptor(serial_string, sizeof serial_string);
}
static u8 KP_OFF_HID_DESC;
static u8 Mouse_OFF_HID_DESC;
extern u8 udisk_mode;
void init_config_descriptor()
{
  const u8 interface_kb[] = {
      0x09, //bLength字段
      0x04, //bDescriptorType字段
      0x00, //bInterfaceNumber字段
      0x00, //bAlternateSetting字段
      0x02, //bNumEndpoints字段
      0x03, //bInterfaceClass字段
      0x01, //bInterfaceSubClass字段
      0x01, //bInterfaceProtocol字段
      0x00, //iConfiguration字段

      /******************HID描述符************************/
      0x09, //bLength字段
      0x21, //bDescriptorType字段
      0x10, //bcdHID字段
      0x01,
      0x21, //bCountyCode字段
      0x01, //bNumDescriptors字段
      0x22, //bDescriptorType字段

      //bDescriptorLength字段。
      //下级描述符的长度。下级描述符为键盘报告描述符。
      sizeof(KeyboardReportDescriptor) & 0xFF,
      (sizeof(KeyboardReportDescriptor) >> 8) & 0xFF,

      /**********************输入端点描述符***********************/
      0x07, //bLength字段
      0x05, //bDescriptorType字段
      0x81, //bEndpointAddress字段
      0x03, //bmAttributes字段
      0x10, //wMaxPacketSize字段
      0x00,
      0x0A, //bInterval字段

      /**********************输出端点描述符***********************/
      0x07, //bLength字段
      0x05, //bDescriptorType字段
      0x01, //bEndpointAddress字段
      0x03, //bmAttributes字段
      0x10, //wMaxPacketSize字段
      0x00,
      0x0A, //bInterval字段
  };
  const u8 interface_mouse[] = {
      0x09, //bLength字段
      0x04, //bDescriptorType字段
      0x01, //bInterfaceNumber字段
      0x00, //bAlternateSetting字段
      0x01, //bNumEndpoints字段
      0x03, //bInterfaceClass字段
      0x01, //bInterfaceSubClass字段
      0x02, //bInterfaceProtocol字段
      0x00, //iConfiguration字段

      /******************HID描述符************************/
      0x09, //bLength字段
      0x21, //bDescriptorType字段
      0x10, //bcdHID字段
      0x01,
      0x21,                                 //bCountyCode字段
      0x01,                                 //bNumDescriptors字段
      0x22,                                 //bDescriptorType字段
      sizeof(MouseReportDescriptor) & 0xFF, //bDescriptorLength字段
      (sizeof(MouseReportDescriptor) >> 8) & 0xFF,

      /**********************输入端点描述符***********************/
      0x07, //bLength字段
      0x05, //bDescriptorType字段
      0x82, //bEndpointAddress字段
      0x03, //bmAttributes字段。D1~D0为端点传输类型选择
      0x40, //wMaxPacketSize字段
      0x00,
      0x0A, //bInterval字段
  };
  const u8 interface_mass[] = {
      0x09, /* bLength: Interface Descriptor size */
      0x04, /* bDescriptorType: */
      /*      Interface descriptor type */
      0x02, /* bInterfaceNumber: Number of Interface */
      0x00, /* bAlternateSetting: Alternate setting */
      0x02, /* bNumEndpoints*/
      0x08, /* bInterfaceClass: MASS STORAGE Class */
      0x06, /* bInterfaceSubClass : SCSI transparent*/
      0x50, /* nInterfaceProtocol */
      4,    /* iInterface: */

      0x07,       /*Endpoint descriptor length = 7*/
      0x05,       /*Endpoint descriptor type */
      0x83,       /*Endpoint address (IN, address 1) */
      0x02,       /*Bulk endpoint type */
      0x40,       /*Maximum packet size (64 bytes) */
      0x00, 0x00, /*Polling interval in milliseconds */

      0x07,       /*Endpoint descriptor length = 7 */
      0x05,       /*Endpoint descriptor type */
      0x04,       /*Endpoint address (OUT, address 2) */
      0x02,       /*Bulk endpoint type */
      0x40,       /*Maximum packet size (64 bytes) */
      0x00, 0x00, /*Polling interval in milliseconds*/
  };
  const u8 interface_com[] = {
      0x09, /* bLength: Interface Descriptor size */
      0x04, /* bDescriptorType: Interface */
      /* Interface descriptor type */
      0x02, /* bInterfaceNumber: Number of Interface */
      0x00, /* bAlternateSetting: Alternate setting */
      0x02, /* bNumEndpoints: One endpoints used */
      0x00, /* bInterfaceClass: Communication Interface Class */
      0x00, /* bInterfaceSubClass: Abstract Control Model */
      0x00, /* bInterfaceProtocol: Common AT commands */
      0x00, /* iInterface: */

      /*Endpoint 2in Descriptor*/
      0x07, /* bLength: Endpoint Descriptor size */
      0x05, /* bDescriptorType: Endpoint */
      0x83, /* bEndpointAddress: (IN2) */
      0x02, /* bmAttributes: bulk */
      0x40,
      0x00, /* wMaxPacketSize: */
      0x00, /* bInterval: */

      /*Endpoint 2out Descriptor*/
      0x07, /* bLength: Endpoint Descriptor size */
      0x05, /* bDescriptorType: Endpoint */
      0x04, /* bEndpointAddress: (out2) */
      0x02, /* bmAttributes: bulk */
      0x40,
      0x00, /* wMaxPacketSize: */
      0x00, /* bInterval: */
  };
  const u8 *third_interface;
  u8 third_interface_length;
	printf("udisk_mode%d",udisk_mode);
  if (udisk_mode)
  {
    third_interface = interface_mass;
    third_interface_length = sizeof interface_mass;
  }
  else
  {
    third_interface = interface_com;
    third_interface_length = sizeof interface_com;
  }
  u8 length = sizeof interface_kb + sizeof interface_mouse + third_interface_length + 9;

  u8 head[] = {
      0x09,                              //bLength字段
      USB_CONFIGURATION_DESCRIPTOR_TYPE, //bDescriptorType字段
      //wTotalLength字段
      length % 128,
      /* wTotalLength: Bytes returned */
      length / 128,

      0x03, //bNumInterfaces字段
      0x01, //bConfiguration字段
      0x00, //iConfigurationz字段
      0x80, //bmAttributes字段
      0x32, //bMaxPower字段
  };
  KP_OFF_HID_DESC = sizeof head + 9;
  Mouse_OFF_HID_DESC = sizeof head + sizeof interface_kb + 9;

  const u8 *const sections[] = {
      head, interface_kb, interface_mouse, third_interface};
  u8 sections_length[] = {
      sizeof head,
      sizeof interface_kb,
      sizeof interface_mouse,
      third_interface_length};

  u8 *const buf = (u8 *)malloc(length);
  u8 index = 0;
  const u8 section_len = sizeof(sections) / sizeof(sections[1]);
  for (int i = 0; i < section_len; i++)
  {
    for (int j = 0; j < sections_length[i]; j++)
    {
      buf[index] = sections[i][j];
      index++;
    }
  }

  Joystick_ConfigDescriptor = buf;
  ONE_DESCRIPTOR *ret = (ONE_DESCRIPTOR *)malloc(sizeof(ONE_DESCRIPTOR));
  ret->Descriptor = buf;
  ret->Descriptor_Size = length;
  Config_Descriptor = *ret;
}
extern ONE_DESCRIPTOR KP_Hid_Descriptor;
extern ONE_DESCRIPTOR Mouse_Hid_Descriptor;

void init_hid_descriptor()
{
  KP_Hid_Descriptor.Descriptor = (u8 *)Joystick_ConfigDescriptor + KP_OFF_HID_DESC;
  KP_Hid_Descriptor.Descriptor_Size = JOYSTICK_SIZ_HID_DESC;
  Mouse_Hid_Descriptor.Descriptor = (u8 *)Joystick_ConfigDescriptor + Mouse_OFF_HID_DESC;
  Mouse_Hid_Descriptor.Descriptor_Size = JOYSTICK_SIZ_HID_DESC;
};

void init_USB_config()
{
  init_string_descriptor();
  init_config_descriptor();
  init_hid_descriptor();
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
