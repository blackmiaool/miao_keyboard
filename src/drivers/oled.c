#include "oled.h"
#include "debug.h"


#define OLED_DC_H       IO1(base[0],PIN[0])
#define OLED_DC_L       IO0(base[0],PIN[0])
#define OLED_CS_H       IO1(base[1],PIN[1]);
#define OLED_CS_L       do{IO0(base[1],PIN[1]);IO1(IOCB,PIN1);}while(0)
#define OLED_MOSI_H     IO1(base[2],PIN[2])
#define OLED_MOSI_L     IO0(base[2],PIN[2])
#define OLED_SCK_H      IO1(base[3],PIN[3])
#define OLED_SCK_L      IO0(base[3],PIN[3])
#define OLED_RST_H      IO1(base[4],PIN[4])
#define OLED_RST_L      IO0(base[4],PIN[4])
#define delay_long      delay_ms2(30)
#define delay_short     delay_us2(1)
//static struct OLED_IO_info* info;
static u16 clk[5]={PERIOA,PERIOA,PERIOA,PERIOA,PERIOA};//DC CS MOSI SCK RST
static GPIO_TypeDef* base[5]={IOAB,IOAB,IOAB,IOAB,IOAB};
static u32 PIN[5]={PIN3,PIN4,PIN7,PIN5,PIN2};
typedef  unsigned char byte;
static void OLED_Init(void);
static void OLED_CLS(void);
static void OLED_Flush(void);
static u8 OLED_mode;
#include "OLED12864.h"
#include "ziku.h"


#define X_WIDTH 128
#define Y_WIDTH 64
#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel		((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xCF
static u8 OLED_buf[8][128];
static void OLED_WrCmd(unsigned char cmd);
static void OLED_WrDat(unsigned char dat);
static void OLED_Set_Pos(unsigned char x, unsigned char y);
static void OLED_IO_Init(void);


//static void OLED_write_char(u8 x,u8 y,char aa);
//static void OLED_P6x8Str(byte x,byte y,const byte ch[]);
static void OLED_P8x16Str(byte x,byte y,byte ch[]);
//static void OLED_P14x16Ch(byte x,byte y,byte N);
//static void OLED_DrawPoint(u8 x,u8 y);
//static void  OLED_ShowNum(u16 x,u16 y,long num,u8 len);
static void delay_ms2(u32 ms)
{
	volatile u32 i=0;
	volatile u32 j=0;
	volatile u32 k=0;
	for(i=0;i<ms;i++)
	{
		for(j=0;j<1000;j++)
		for(k=0;k<72;k++);
	}
}
static rt_err_t rt_OLED_init (rt_device_t dev)
{


    if (!(dev->flag & RT_DEVICE_FLAG_ACTIVATED))
    {
#if MCU_this==STM32F4

#endif
        /* ?¨¨???¨¨¡À????¡è??¡Á??? */
        dev->flag |= RT_DEVICE_FLAG_ACTIVATED;
    }
    return RT_EOK;
}
static rt_err_t rt_OLED_open(rt_device_t dev, rt_uint16_t oflag)
{
    return RT_EOK;
}
/* ??¡À??¨¨¡À? */
static rt_err_t rt_OLED_close(rt_device_t dev)
{

    return RT_EOK;
}
/* ???¨¨¡À??????????? */
static rt_size_t rt_OLED_read (rt_device_t dev, rt_off_t pos,
                               void* buffer, rt_size_t size)
{
    rt_err_t err_code;
    err_code = RT_EOK;
    rt_set_errno(err_code);
    return 0;
}



static rt_size_t rt_OLED_write (rt_device_t dev, rt_off_t pos,
                                const void* buffer, rt_size_t size)
{
    u16 i=0;
    
    switch(OLED_mode)
    {
    case 0:{
        OLED_P8x16Str((pos>>8)<<3,pos&0xff,( u8*)buffer);
        break;
    }
    case 2:
    case 1:{
        u8 y_offset=0;
        u8 x_offset=0;
        u8 *data_ptr;
        x_offset=pos%128;//0~127
        
        y_offset=pos>>7;//0~63
        //DBG("---write:%d,%d,%d&",x_offset,y_offset,size);

        for(i=0;i<size;i++)
        {
            if(x_offset+i<127)
            {
                data_ptr=&(OLED_buf[y_offset>>3][x_offset+i]);
                if(OLED_mode==1)
                if(!(((u8*)buffer)[(i)/8]&(1<<(7-(i)%8))))
                {
                    *data_ptr|=(1<<((y_offset)%8));
                }
                else
                {
                    *data_ptr&=~(1<<((y_offset)%8));
                }
                else
                {
                    if((((u8*)buffer)[(i)/8]&(1<<(7-(i)%8))))
                    {
                        *data_ptr|=(1<<((y_offset)%8));
                    }
                    else
                    {
                        *data_ptr&=~(1<<((y_offset)%8));
                    }
                }
            }
            
            //DBG("%d",!(((u8*)buffer)[(i)/8]&(1<<((i)%8))));
        }
           
        break;
    }
    }

    return size;
}


///* ?¨¨¡À???????¡Á¡Â */
static rt_err_t rt_OLED_control (rt_device_t dev,
                                 rt_uint8_t cmd, void *args)
{
    //DBG("---control:%d&",cmd);
    RT_ASSERT(dev != RT_NULL);
	switch(cmd)
	{
	case 0:
	{
		OLED_CLS();
		break;
	}
	case 1:
	{
		OLED_Flush();
		break;
	}
    case 2:
    {
        OLED_mode=0;
        break;
    }
    case 3:
    {
        OLED_mode=1;
        break;
    }
    case 4:
    {
        OLED_mode=2;
        break;
    }
	}
    return RT_EOK;
}
///*
//* ?¨°??????¡Á??¨¢?????¨¨¡À?
//*/
rt_err_t rt_hw_OLED_register(rt_device_t device, const char* name,
                             rt_uint32_t flag, struct OLED_IO_info *info2)
{
    RT_ASSERT(device != RT_NULL);

    /* ?¨¨???¨¨¡À??????¨¤?? */
    device->type = RT_Device_Class_Char;
    device->rx_indicate = RT_NULL;
    device->tx_complete = RT_NULL;
    /* ?¨¨???¨¨¡À????????????????? */
    device->init = rt_OLED_init;
    device->open = rt_OLED_open;
    device->close = rt_OLED_close;
    device->read = rt_OLED_read;
    device->write = rt_OLED_write;
    device->control = rt_OLED_control;
    device->user_data = info2;
    /* ¡Á??¨¢????¡Á?¡¤??¨¨¡À? */
    return rt_device_register(device, name, RT_DEVICE_FLAG_WRONLY  | flag);
}
///* ISR for serial interrupt */


static struct rt_device OLED_device;
static struct OLED_IO_info info_base;

void rt_hw_OLED_init()
{


    OLED_Init();
//    info= &info_base;
    rt_hw_OLED_register(&OLED_device, "OLED",
                        RT_DEVICE_FLAG_WRONLY ,
                        &info_base);

}




















static void delay_us2(u16 us)
{
    u32 j=0;
    for(j=0;j<us*2;j++);
}














/***************transplant*****************/



static void OLED_IO_Init()
{
    u8 i=0;
	SCPE(PERIOC);
	IOConfig(IOCB,PIN1,tuiwanshuchu);
    for(i=0;i<5;i++)
    {
        SCPE(clk[i]);
        IOConfig(base[i],PIN[i],tuiwanshuchu);
    }
	OLED_CS_H;
	IO1(IOCB,PIN1);
    OLED_DC_H;
    OLED_RST_H;
    OLED_MOSI_H;
    OLED_SCK_H;
}

/***************transplant end*****************/




void OLED_Init(void)
{
    OLED_IO_Init();
    OLED_RST_L;
    delay_long;
    OLED_RST_H;
    //??????????????????????????¡Á??????¡À??????????RC?????¨º¡À?

    OLED_WrCmd(0xae);//--turn off oled panel
    OLED_WrCmd(0x00);//---set low column address
    OLED_WrCmd(0x10);//---set high column address
    OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WrCmd(0x81);//--set contrast control register
    OLED_WrCmd(0xcf); // Set SEG Output Current Brightness
    OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0¡Á¨®??¡¤??? 0xa1????
    OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0????¡¤??? 0xc8????
    OLED_WrCmd(0xa6);//--set normal display
    OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
    OLED_WrCmd(0x3f);//--1/64 duty
    OLED_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WrCmd(0x00);//-not offset
    OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
    OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WrCmd(0xd9);//--set pre-charge period
    OLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WrCmd(0xda);//--set com pins hardware configuration
    OLED_WrCmd(0x12);
    OLED_WrCmd(0xdb);//--set vcomh
    OLED_WrCmd(0x40);//Set VCOM Deselect Level
    OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_WrCmd(0x02);//
    OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
    OLED_WrCmd(0x14);//--set(0x10) disable
    OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
    OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7)
    OLED_WrCmd(0xaf);//--turn on oled panel
    // OLED_Fill(0x00);  //????????
    //OLED_CLS();
    OLED_Set_Pos(0,0);
    //OLED_write_char(1,1,49);
}
#include "stm32f4lib.h"
#define LD_CS_H	IO1(IOCB,PIN1);
#define LD_CS_L	do{IO1(IOAB,PIN4);IO0(IOCB,PIN1);}while(0)
#define LD3320_MISO PAin(6)
u8 LD3320_WrDat(unsigned char dat)
{
    unsigned char i=8;
    u8 j=100;
    //OLED_CS=0;;
   // LD_CS_L;
    OLED_SCK_L;
    //;;
    while(i--)
    {
        (dat&0x80)?OLED_MOSI_H:OLED_MOSI_L;
        OLED_SCK_H;
        while(j--);
        j=100;

        //;;
        OLED_SCK_L;
        dat<<=1;
        dat|= LD3320_MISO;
        delay_short;
    }
 //   LD_CS_H;
 return dat;
}
void OLED_WrDat(unsigned char dat)
{
    unsigned char i=8;
    //OLED_CS=0;;
    OLED_CS_L;
    OLED_DC_H;
    OLED_SCK_L;
    //;;
    while(i--)
    {
        (dat&0x80)?OLED_MOSI_H:OLED_MOSI_L;
        OLED_SCK_H;

        //;;
        OLED_SCK_L;
        dat<<=1;
        delay_short;
    }
    //OLED_CS=1;
    OLED_CS_H;
}
void OLED_WrCmd(unsigned char cmd)
{
    unsigned char i=8;
    OLED_CS_L;
    OLED_DC_L;

    OLED_SCK_L;
    //;;
    while(i--)
    {
        (cmd&0x80)?OLED_MOSI_H:OLED_MOSI_L;
        OLED_SCK_H;
        OLED_SCK_L;
        cmd<<=1;
    }
    OLED_CS_H;
}
void OLED_Set_Pos(unsigned char x, unsigned char y)
{
    OLED_WrCmd(0xb0+y);
    OLED_WrCmd(((x&0xf0)>>4)|0x10);
    OLED_WrCmd((x&0x0f)|0x01);
}
void OLED_Flush()
{
    unsigned char y,x;

    for(y=0;y<8;y++)
    {
        OLED_WrCmd(0xb0+y);
        OLED_WrCmd(0x01);
        OLED_WrCmd(0x10);
        for(x=0;x<X_WIDTH;x++)
        {	OLED_WrDat(OLED_buf[y][x]);}
    }
}
void OLED_CLS()
{
    unsigned char y,x;
    for(y=0;y<8;y++)
    {
        OLED_WrCmd(0xb0+y);
        OLED_WrCmd(0x01);
        OLED_WrCmd(0x10);
        for(x=0;x<X_WIDTH;x++)
            OLED_WrDat(0);
    }

    for(x=0;x<8;x++)
    {
        for(y=0;y<128;y++)
            OLED_buf[x][y]=0;
    }
}



//==============================================================
//????????OLED_P6x8Str(unsigned char x,unsigned char y,unsigned char *p)
//?????¨¨??????????¡Á¨¦¡À¨º¡Á?ASCII¡Á?¡¤???
//?????????????????¡§x,y????y????¡¤??¡ì0??7??????????¡Á?¡¤???
//¡¤???????
//==============================================================
//void OLED_P6x8Str(unsigned char x,unsigned char y,const unsigned char ch[])
//{
//    unsigned char c=0,i=0,j=0;
//    while (ch[j]!='\0')
//    {
//        c =ch[j]-32;
//        if(x>126){x=0;y++;}
//        OLED_Set_Pos(x,y);
//        for(i=0;i<6;i++)
//            OLED_WrDat(F6x8[c][i]);
//        x+=6;
//        j++;
//    }
//}
//void OLED_DrawPoint(u8 x,u8 y)
//{
//    u8 row;

//    row = y >> 3;
//    OLED_Set_Pos(x,row);												//???????¨´??????
//    OLED_WrDat(OLED_buf[row][x] |= (1 << (y % 8)));
//}
//==============================================================
//????????OLED_P8x16Str(unsigned char x,unsigned char y,unsigned char *p)
//?????¨¨??????????¡Á¨¦¡À¨º¡Á?ASCII¡Á?¡¤???
//?????????????????¡§x,y????y????¡¤??¡ì0??7??????????¡Á?¡¤???
//¡¤???????
//==============================================================
//void  OLED_ShowNum(u16 x,u16 y,long num,u8 len)
//{
//    u8 i=0;
//    char shuchuxulie[10];
//    for(i=0;i<10;i++)
//    {
//        shuchuxulie[i]=32;
//    }
//    //	OLED_Set_Pos(x,y);
//    if(num<0)
//    {
//        OLED_write_char(x,y,'-');
//        num=-num;
//        len-=1;
//        x+=8;
//    }
//    if(num==0)
//    {
//        x+=(len-1)*8;
//        OLED_write_char(x,y,'0');

//    }
//    else
//    {

//        i=0;
//        while(num)
//        {
//            shuchuxulie[i]=num%10+'0';
//            i++;
//            num/=10;
//        }
//        for(i=len;i>0;i--)
//        {
//            OLED_write_char(x,y,shuchuxulie[i-1]);
//            x+=8;
//        }
//    }
//}
//????¡Á?¡¤?????????y¡Á??¨¤??6
//void OLED_write_char(u8 x,u8 y,char aa)
//{
//    u16 i=0;
//    OLED_Set_Pos(x,y);
//    for(i=0;i<8;i++)
//        OLED_WrDat(F8X16[(aa-32)*16+i]);
//    OLED_Set_Pos(x,y+1);
//    for(i=0;i<8;i++)
//        OLED_WrDat(F8X16[(aa-32)*16+i+8]);
//}
void OLED_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[])
{
    unsigned char c=0,i=0,j=0;

    while (ch[j]!='\0')
    {
        c =ch[j]-32;
        if(x>120){return;}//x=0;y++;}
        OLED_Set_Pos(x,y);
        for(i=0;i<8;i++)
            OLED_WrDat(F8X16[c*16+i]);
        OLED_Set_Pos(x,y+1);
        for(i=0;i<8;i++)
            OLED_WrDat(F8X16[c*16+i+8]);
        x+=8;
        j++;
    }
}

//??????¡Á?¡Á?¡¤???
//void OLED_P14x16Ch(unsigned char x,unsigned char y,unsigned char N)
//{
//    unsigned char wm=0;
//    unsigned int adder=28*N;
//    OLED_Set_Pos(x , y);
//    for(wm = 0;wm < 14;wm++)
//    {
//        OLED_WrDat(F14x16[adder]);
//        adder += 1;
//    }
//    OLED_Set_Pos(x,y + 1);
//    for(wm = 0;wm < 14;wm++)
//    {
//        OLED_WrDat(F14x16[adder]);
//        adder += 1;
//    }
//}



