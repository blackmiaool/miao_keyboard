#include "draw_bmp.h"
struct tagBITMAPFILEHEADER  
{    
    u16 bfType;     
    u32 bfSize;  
    u16 bfReserved1;  
    u16 bfReserved2;  
    u32 bfOffBits; 
}bitmap_file_header;
struct tagBITMAPINFOHEADER  
{  
    u32 biSize;  
    u32 biWidth;  
    u32 biHeight;  
    u16 biPlanes;  
    u16 biBitCount;  
    u32 biCompression;  
    u32 biSizeImage;  
    u32 biXPelsPerMeter;  
    u32 biYPelsPerMeter;  
    u32 biClrUsed;  
    u32 biClrImportant; 
} bitmap_info_header;
struct colorBOARD{
    u32 color1;
    u32 color2;
}color_board;
void num_change(u32 *num)
{
    u32 num_temp;
    num_temp=((*num&0xff)<<24)|((*num&0xff00)<<8)|((*num&0xff0000)>>8)|((*num&0xff000000)>>24);
    *num=num_temp;
}
int draw_bmp(u16 x,u16 y,char *buf)
{
    
    FIL file; 
    u32 cnt=0;
    u32 i=0;

    u32 file_header_size;
    u32 map_info_size;
    u8 err;
    
    y=63-y;
    debug_en=1;
    rt_sem_take(sem_flash,RT_WAITING_FOREVER);
    err=f_open(&file,(const char*)buf,FA_READ);
    if(err)
    {
        DBG("open err:%d\r\n",err);
        rt_sem_release(sem_flash);
        return -1;
    }
   // if(x==0)
    f_read(&file,read_buf,file.fsize,&cnt);
    
    f_close(&file);
    rt_sem_release(sem_flash);
    
    //DBG("size=%d\r\n",file.fsize);
    file_header_size=14;
    map_info_size=sizeof(bitmap_info_header)/8*8;
    for(i=0;i<file_header_size;i++)
    {
        *((u8 *)(&bitmap_file_header)+i)=read_buf[i];
        //putchar(read_buf[i]);
    }
    bitmap_file_header.bfOffBits=0x3e;
    for(i=file_header_size;i<file_header_size+map_info_size;i++)
    {
        *((u8 *)(&bitmap_info_header)+i-file_header_size)=read_buf[i];
        //putchar(read_buf[i]);
    }
    for(i=bitmap_file_header.bfOffBits-8;i<bitmap_file_header.bfOffBits;i++)
    {
        *((u8 *)(&color_board)+i-(bitmap_file_header.bfOffBits-8))=read_buf[i];
        //putchar(read_buf[i]);
    }

    //num_change(&(bitmap_info_header.biHeight));
    //num_change(&(bitmap_file_header.bfOffBits));
//    DBG("height:%d,width:%d\r\n",bitmap_info_header.biHeight,bitmap_info_header.biWidth);
//    DBG("height:%d,width:%d\r\n",bitmap_info_header.biHeight,bitmap_info_header.biWidth);
//    DBG("size:%d,offset:%d\r\n",sizeof(bitmap_file_header),bitmap_file_header.bfOffBits);
//    DBG("height:%d,width:%d\r\n",bitmap_info_header.biHeight,bitmap_info_header.biWidth);
    
    if(color_board.color1>0xffff)
        rt_device_control(OLED_dev,4, ( void *)0);
    else
        rt_device_control(OLED_dev,3, ( void *)0);
    for(i=0;i<bitmap_info_header.biHeight;i++)
    {
        long pos;
        if(ini.OLED.orientation==0)
        {
            pos=((63-(y+i))<<7)+x;
        }
        else
        {
            pos=((y+i)<<7)+x;
        }
        if(pos>=0&&pos<=(128*64-1))
        {
            rt_device_write(OLED_dev, \
                pos, \
                &(read_buf[bitmap_file_header.bfOffBits+(bitmap_info_header.biWidth/32*4+((bitmap_info_header.biWidth%32)>0)*4)*i]), \
                bitmap_info_header.biWidth);
            //DBG("--%d--",bitmap_file_header.bfOffBits+(bitmap_info_header.biWidth/32*4+((bitmap_info_header.biWidth%32)>0)*4)*i);
        }
        
    }
    
    rt_device_control(OLED_dev,1, ( void *)0);  
    rt_device_control(OLED_dev,2, ( void *)0);        
    
    return 0;
}
