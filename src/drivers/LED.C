#include "led.h"
#define MCU_this STM32F4
#if MCU_this==STM32F4
struct LED_IO_info* info;
static u16 clk[LED_CNT]={PERIOD,PERIOD,PERIOD,PERIOD};
static GPIO_TypeDef* base[LED_CNT]={IODB,IODB,IODB,IODB};
static u32 PIN[LED_CNT]={PIN12,PIN13,PIN14,PIN15};
#endif

void delay_ms2(u32 ms);

static rt_err_t rt_LED_init (rt_device_t dev)
{
    // struct LED_IO_info* info;

    u8 i=0;
    /* 获得真实的UART设备对象 */
    // info= (struct LED_IO_info*)dev->user_data;
    /* 判断设备是否已经激活了 */
    if (!(dev->flag & RT_DEVICE_FLAG_ACTIVATED))
    {
#if MCU_this==STM32F4
        for(i=0;i<LED_CNT;i++)
            IO1(info->base[i],info->PIN[i]);
#endif

        /* 设置设备为激活状态 */
        dev->flag |= RT_DEVICE_FLAG_ACTIVATED;
    }
    return RT_EOK;
}
static rt_err_t rt_LED_open(rt_device_t dev, rt_uint16_t oflag)
{
    u16 i=0;
    for(i=0;i<LED_CNT;i++)
    {
        IO1(base[i],PIN[i]);
    }
    return RT_EOK;
}
/* 关闭设备 */
static rt_err_t rt_LED_close(rt_device_t dev)
{
    u16 i=0;
    for(i=0;i<LED_CNT;i++)
    {
        IO0(base[i],PIN[i]);
    }
    return RT_EOK;
}
/* 从设备中读取数据 */
static rt_size_t rt_LED_read (rt_device_t dev, rt_off_t pos,
                              void* buffer, rt_size_t size)
{
    rt_err_t err_code;
    err_code = RT_EOK;
    rt_set_errno(err_code);
    return 0;
}
/* 向设备中写入数据 */
static rt_size_t rt_LED_write (rt_device_t dev, rt_off_t pos,
                               const void* buffer, rt_size_t size)
{
    rt_uint8_t* ptr;
    rt_err_t err_code;
    rt_uint16_t i=0;
    RT_ASSERT(dev != RT_NULL);
    err_code = RT_EOK;

    for(i=0;i<LED_CNT;i++)
    {
        IOset(info->base[i],info->PIN[i],ptr2bit(buffer,i));
    }
    //IOset(IODB,info->PIN[i],(*(u8 *)buffer)&1);

    /* 设置错误号 */
    rt_set_errno(err_code);

    /* 返回写入成功的字节数 */
    return (rt_uint32_t)ptr - (rt_uint32_t)buffer;
}
/* 设备控制操作 */
static rt_err_t rt_LED_control (rt_device_t dev,
                                rt_uint8_t cmd, void *args)
{
    RT_ASSERT(dev != RT_NULL);

    return RT_EOK;
}
/*
* 向系统中注册串口设备
*/
rt_err_t rt_hw_LED_register(rt_device_t device, const char* name,
                            rt_uint32_t flag, struct LED_IO_info *info2)
{
    RT_ASSERT(device != RT_NULL);

    /* 设置设备驱动类型 */
    device->type = RT_Device_Class_Char;
    device->rx_indicate = RT_NULL;
    device->tx_complete = RT_NULL;
    /* 设置设备驱动公共接口函数 */
    device->init = rt_LED_init;
    device->open = rt_LED_open;
    device->close = rt_LED_close;
    device->read = rt_LED_read;
    device->write = rt_LED_write;
    device->control = rt_LED_control;
    device->user_data = info2;
    /* 注册一个字符设备 */
    return rt_device_register(device, name, RT_DEVICE_FLAG_WRONLY  | flag);
}
/* ISR for serial interrupt */
static void RCC_Configuration2()
{

    u16 i=0;
    for(i=0;i<LED_CNT;i++)
    {
        SCPE(info->clk[i]);
    }
}

static void GPIO_Configuration2()
{
    u16 i=0;
    for(i=0;i<LED_CNT;i++)
    {
        IOConfig(info->base[i],info->PIN[i],tuiwanshuchu);
    }
}

struct rt_device LED_device;
struct LED_IO_info info_base;
void rt_hw_LED_init()
{
    u16 i=0;
    delay_ms2(2);
    for(i=0;i<LED_CNT;i++)
    {
        info_base.base[i]=base[i];
        info_base.clk[i]=clk[i];
        info_base.PIN[i]=PIN[i];
    }

    info= &info_base;
    RCC_Configuration2();

    GPIO_Configuration2();

    rt_hw_LED_register(&LED_device, "LED",
                       RT_DEVICE_FLAG_WRONLY ,
                       &info_base);

}















