#include <stm32f10x_lib.h>
#include "sys.h"
#include "delay.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "usart.h"
#include "stm32lib.h"
#include "usb_process.h"
#include "mass_mal.h"
#include "keyboard.h"
#include "w25q16.h"
#include "led.h"
#include "app.h"
#include "wdg.h"

extern u8 EP1BUSY; // kb busy flag
extern u8 EP2BUSY; // mouse busy flag
extern u8 INIT_OK;

extern long long Mass_Memory_Size[MAX_LUN + 1];
extern u32 Mass_Block_Size[MAX_LUN + 1];
extern u32 Mass_Block_Count[MAX_LUN + 1];
extern u8 USB_STATUS_REG;

int calc_free_memory(u32 last, u32 base)
{
    for (int i = 0; i <= 10; i++)
    {
        char *area = (char *)malloc(i * base + last);
        if (!area)
        {
            if (base != 1)
            {
                return calc_free_memory(last + (i - 1) * base, base / 10);
            }
            else
            {
                return last + i;
            }
        }
        else
        {
            free(area);
        }
    }
    return 0;
}
void print_free_memory(const char *title)
{
    printf("%s memory= %.3f KB\r\n", title, (float)calc_free_memory(0, 100000) / 1000);
}
static void timer_init()
{
    SCPE(PERTIMER1);
    TIM1->ARR = 10000; //1s per second
    TIM1->PSC = 7199;  //0.1ms per tick
    TIM1->CR1 |= 0x01;
}
extern void init_USB_config(void);
int main(void)
{
    Stm32_Clock_Init(9);
    delay_init(72);
    uart_init(72, 115200);

    // free JTAG pins
    JTAG_Set(1);

    // init usb hardware

    USB_Cable_Init();
    USB_Cable_Config(ENABLE);

    timer_init();

    // wait for flash
    delay_ms(200);
    SPI_Flash_Init();

    printf("start\r\n");

    // init flash descriptor for udisk
    Mass_Memory_Size[0] = 500 * 4096;
    Mass_Block_Size[0] = 4096;
    Mass_Block_Count[0] = 500;

    // init usb
    keyboard_init();
		keyboard_scan();
    init_USB_config();
    led_init();
    //exec before app_init to check if parse bmk files
    

    Set_USBClock();
    USB_Interrupts_Config();
    USB_Init();

    // wait for usb
    while (!INIT_OK)
        ;

    // wait for udisk mounting
    delay_ms(2000);
    print_free_memory("before app");
    
    app_init();
    IWDG_Init(4,500);
    while (1)
    {
		IWDG_Feed();
        led_handle();
        keyboard_scan();
        delay_us(100);
    }
}
