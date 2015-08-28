/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 * 2014-04-27     Bernard      make code cleanup. 
 */

#include <board.h>
#include <rtthread.h>

#ifdef RT_USING_LWIP
#include <lwip/sys.h>
#include <lwip/api.h>
#include <netif/ethernetif.h>
#include "stm32f4xx_eth.h"
#endif

#ifdef RT_USING_GDB
#include <gdb_stub.h>
#endif
#include "defines.h"
#include "tm_stm32f4_usb_hid_device.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_disco.h"
#include "keyboard.h"
void delay(u32 t){
    rt_thread_delay(t);
}
void rt_usb_thread_entry(void* parameter){
//    SystemInit();
    uint8_t already = 0;

    TM_USB_HIDDEVICE_Keyboard_t Keyboard;
    TM_USB_HIDDEVICE_Gamepad_t Gamepad1, Gamepad2;
    TM_USB_HIDDEVICE_Mouse_t Mouse;
    /* Initialize leds */
    TM_DISCO_LedInit();

    /* Initialize button */
    TM_DISCO_ButtonInit();

    /* Initialize delay */
    TM_DELAY_Init();

    /* Initialize USB HID Device */
    TM_USB_HIDDEVICE_Init();

    /* Set default values for mouse struct */
    TM_USB_HIDDEVICE_MouseStructInit(&Mouse);
    /* Set default values for keyboard struct */
    TM_USB_HIDDEVICE_KeyboardStructInit(&Keyboard);
    /* Set default values for gamepad structs */
    TM_USB_HIDDEVICE_GamepadStructInit(&Gamepad1);
    TM_USB_HIDDEVICE_GamepadStructInit(&Gamepad2);
    keyboard_init();
    while (1) {
        /* If we are connected and drivers are OK */
        if (TM_USB_HIDDEVICE_GetStatus() == TM_USB_HIDDEVICE_Status_Connected) {
            /* Turn on green LED */
/* Simple sketch start */
            rt_kprintf("sldkjfsldkfj");
            /* If you pressed button right now and was not already pressed */
            if ((TM_GPIO_GetInputPinValue(GPIOA, GPIO_PIN_0) != 0) && already == 0) { /* Button on press */
                already = 1;
                rt_kprintf("111111");
                /* Set pressed keys = WIN + R */
//				Keyboard.L_GUI = TM_USB_HIDDEVICE_Button_Pressed;	/* Win button */
                Keyboard.Key1 = 0x15; 								/* R */
                /* Result = "Run" command */

                /* Send keyboard report */
                TM_USB_HIDDEVICE_KeyboardSend(&Keyboard);
            } else if (!TM_DISCO_ButtonPressed() && already == 1) { /* Button on release */
                already = 0;

                /* Release all buttons */
                Keyboard.L_GUI = TM_USB_HIDDEVICE_Button_Released;	/* No button */
                Keyboard.Key1 = 0x00; 								/* No key */
                /* Result = Released everything */

                /* Send keyboard report */
                TM_USB_HIDDEVICE_KeyboardSend(&Keyboard);
            }
            delay(25);
/* Simple sketch end */

        } else {
            /* Turn off green LED */
            TM_DISCO_LedOff(LED_GREEN);
        }
    }
}
void rt_init_thread_entry(void* parameter)
{
    /* GDB STUB */
#ifdef RT_USING_GDB
    gdb_set_device("uart6");
    gdb_start();
#endif

    /* LwIP Initialization */
#ifdef RT_USING_LWIP
    {
        extern void lwip_sys_init(void);

        /* register ethernetif device */
        eth_system_device_init();

        rt_hw_stm32_eth_init();

        /* init lwip system */
        lwip_sys_init();
        rt_kprintf("TCP/IP initialized!\n");
    }
#endif
rt_kprintf("initialized!\n");
}

int rt_application_init()
{
    rt_thread_t tid;

    tid = rt_thread_create("init",
        rt_init_thread_entry, RT_NULL,
        2048, RT_THREAD_PRIORITY_MAX/3, 20);
    rt_thread_t tusb;
    tusb = rt_thread_create("usb",
        rt_usb_thread_entry, RT_NULL,
        4096, 15, 20);
    if (tid != RT_NULL)
        rt_thread_startup(tid);
    if (tusb != RT_NULL)
        rt_thread_startup(tusb);

    return 0;
}

/*@}*/
