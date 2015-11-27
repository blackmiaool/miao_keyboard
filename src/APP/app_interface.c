#include "app_interface.h"
#include "string.h"
#include "ahk_analyser.h"
#include "key_remap.h"
#include "inifile.h"
#include "pwd.h"
#include "lua_app.h"
#include "commu.h"
rt_device_t OLED_dev;
/******************macro area******************/
u8 macro_play_flag;
void macro_play_prepare(cap * cap_this);
static u8 macro_data[5][10];
u8 macro_flag;
u8 macro_lenth;
void macro_record(u8*);
void macro_set(cap * cap_this);
bool macro_play(void);
/******************macro area end******************/
u8 read_buf[5000];
u8 buf_out[10];
void KEYBRD_Decode(uint8_t *pbuf);
void buf_clear(void);
rt_mailbox_t mb_app;
extern rt_mailbox_t mb_commu;
cap key_cap_free[key_cap_cnt_all];
//st_key_cap key_cap_use[200];
cap* cap_use_head=RT_NULL;
cap* cap_free_head=key_cap_free;
rt_uint16_t key_cap_cnt=0;

const u8  ascii2usb[]={
    0,0,0,0,  0,0,0,0,  0,0,0,0,  0,40,0,0, \
    0,0,0,0,  0,0,0,0,  0,0,0,0,  0,0,0,0, \
    44,46,0,0,  0,0,0,0,  0,0,0,0,  54,0,55,0, \
    39,30,31,32,  33,34,35,36,  37,38,51,0,  0,0,0,56, \

    31,4,5,6,  7,8,9,10,  11,12,13,14,  15,16,17,18, \
    19,20,21,22,  23,24,25,26,  27,28,29,0,  0,0,0,0, \
    0,4,5,6,  7,8,9,10,  11,12,13,14,  15,16,17,18, \
    19,20,21,22,  23,24,25,26,  27,28,29,0,  50,0,53,0, \
};
const u8  *const p_ascii2usb=ascii2usb;
const u8 shift_table[]={
    0,0,0,0,    0,0,0,0,    0,0,0,0,    0,0,0,0,
    0,0,0,0,    0,0,0,0,    0,0,0,0,    0,0,0,0,
    0,0,0,0,    0,0,0,0,    0,0,0,0,    0,0,0,0,
    0,0,0,0,    0,0,0,0,    0,0,1,0,    0,0,0,1,

    1,1,1,1,    1,1,1,1,    1,1,1,1,    1,1,1,1,
    1,1,1,1,    1,1,1,1,    1,1,1,0,    0,0,0,0,
    0,0,0,0,    0,0,0,0,    0,0,0,0,    0,0,0,0,
    0,0,0,0,    0,0,0,0,    0,0,0,0,    1,0,1,0,

};

#define min(a,b) (a>b?b:a)
void cmd_line(u8* content);
void cmd(u8* content)
{
//    u32 content_char_cnt=0;
//    u8 content_line_cnt=0;
//    u8 i=0;XX
    return ;
//    while(content[content_char_cnt]!=0)
//    {
//        content_char_cnt++;
//    }
//    content_line_cnt=(content_char_cnt-1)/16+1;
//    for(i=0;i<content_line_cnt;i++)
//    {
//        cmd_line(content+i*16);
//    }
}
void buf_clear()
{
    u8 i=0;
    for(i=2;i<9;i++)
    {
        buf_out[i]=0;
    }
    buf_out[0]=6;
    press;

}
void cmd_line(u8* content)
{
    u32 i=0;
    u32 content_char_cnt=0;
    //	u32 content_line_cnt;
    static u32 line_cnt=0;
    static u8 OLED_buf[4*16];
    while(content[content_char_cnt]!=0)
    {
        content_char_cnt++;
        if(content_char_cnt==16)
        {
            break;
        }

    }
    if(line_cnt<4)
    {
        u8 j=0;
        for(j=0;j<min(16,content_char_cnt);j++)
        {
            OLED_buf[(line_cnt<<4)+j]=content[j];
        }
    }
    else
    {
        u8 i=0;
        for(i=0;i<3;i++)
        {
            u8 j=0;
            for(j=0;j<16;j++)
            {
                OLED_buf[(i<<4)+j]=OLED_buf[((i+1)<<4)+j];
            }
        }

        for(i=0;i<16;i++)
        {
            OLED_buf[(3<<4)+i]=0;
        }
        for(i=0;i<min(16,content_char_cnt);i++)
        {
            OLED_buf[(3<<4)+i]=content[i];
        }

    }


    OLED_dev->control(OLED_dev,0,0);
    for(i=0;i<4;i++)
    {
        //		u8 k=0;
        OLED_dev->write(OLED_dev,(i*2),&OLED_buf[i<<4],16);
        //		for(k=0;k<16;k++)
        //		DBG("OLED:%d",OLED_buf[i][k]);
    }


    line_cnt++;
}


const u8 key_string[key_string_num][key_stinrg_max_word]={
    "lctrl",
    "lshift",
    "lalt",
    "lgui",
    "rctrl",
    "rshift",
    "ralt",
    "rgui"};
u8  key_changetable[]={
    0,1,2,3,    4,5,6,7
};
s8 control_key_index(const char buf[])
{
    u8 i=0;
    for(i=0;i<8;i++)
    {
        if(!strcmp((const char *)buf,(const char *)key_string[i]))
            return i;
    }
    return -1;
}

u8 key_process(u8 control_key)
{
    u8 i=0;
    u8 return_key=0;
    for(i=0;i<8;i++)
    {
        if(control_key&(1<<i))
        {
            return_key|=(1<<key_changetable[i]);
        }
    }
    //key_changetable[]
    return return_key;
}
u8 control_key_filt(u8 key,ctrl_filter* filter)
{
    u8 i=0;
    for(i=0;i<filter->control_filter_cnt;i++)
    {
       // printf("filter%d",filter->control_filter[i]);
        if(key&filter->control_filter[i])
        {
            key&=(~filter->control_filter[i]);
        }
        else
            return 1;
    }
    if(key==0)
        return 0;
    else
        return 1;
}
void key_cap_Init()
{
    u16 i=0;
    for(i=0;i<key_cap_cnt_all-1;i++)
    {
        key_cap_free[i].next=&key_cap_free[i+1];
    }
    key_cap_free[key_cap_cnt-1].next=RT_NULL;
}
void key_cap_del(u16 index)
{

}
void press_string_pure(u16 *buf,u32 lenth)
{
    u32 i=0;
    u8 control_key=0;
    buf_clear();
    buf_out[1]=0;
    for(i=0;i<lenth;i+=1)
    {
        
        if(buf[i]&(1<<9))
        {
            control_key|=buf[i];
            buf_out[1]=control_key;
            press;
        }
        else if(buf[i]&(1<<10))
        {
            control_key&=(~buf[i]);
            buf_out[1]=control_key;
            press;
        }
        else if(buf[i]&(1<<8))
        {
            buf_out[1]=LShift;
            buf_out[3]=buf[i];
            press;
            buf_out[1]=0;
            
        }
        else
        {
            buf_out[3]=buf[i];
            buf_out[1]=control_key;
            press;
        }
        
        
        //printf("press%d\r\n",buf_out[3]);
        if(i!=lenth-1)
        buf_clear();
    }
}
void press_string(cap * cap_this)
{
    press_string_pure(cap_this->string,cap_this->string_lenth); 
}
bool key_handle(u8 *buf)
{
    u8 i=0;
    static  u8 hotkey_flag=0;
    static  u8 hotkey_value=0;
    u8 control_key_this;
    bool result=true;
    //static u8 buf_pre[8]={0};
    

    //DBG("%c",HID_KEYBRD_Key[HID_KEYBRD_Codes[buf[2]]]);
//    if(buf[0]&(1<<6))
//    {
//        buf[0]&=(0xff-(1<<6));W
//        buf[0]|=(1<<4) ;
//    }

	if(macro_flag)
    {
        macro_record(buf);
    }
    
    buf[0]=key_process(buf[0]);//TODO
    control_key_this=buf[0];
 
    if(hotkey_flag)
    {

        if(buf[2]==hotkey_value&&hotkey_flag==1)
        {
            DBG("I block it!\r\n");
            buf[2]=0;
            buf[0]=0;
            result= true;
            goto result;
        }
        else if(buf[0]!=0)
        {
            buf[0]=0;
            hotkey_flag=2;
            
        }
        else
        {
            hotkey_flag=0;
        }
    }
    if(lua_flag&LUA_KEYBOARD_ENABLE_MASK)
    {
        ((u8*)buf)[9]=1;//keyboard event
        rt_mq_send (mq_lua, (void*)buf, 10);
    }
    
    if(mb_app!=0)
    {
        KEYBRD_Decode(buf);
        result= false;
        goto result;
    }
    for(i=0;i<key_cap_cnt;i++)
    {
        cap* cap_this;

        if(i==0)
        {
            cap_this=cap_use_head;
        }
        else
            cap_this=cap_this->next;


        if(control_key_filt(control_key_this,&(cap_this->filter))==0)
        {
            //DBG("(%d----%d)",key_cap_cnt,cap_this->filter.key);
            if( buf[2]==cap_this->filter.key)
            {
//                DBG("I got it!\r\n");
                (cap_this->key_exe)(cap_this);
                hotkey_flag=1;
                hotkey_value=buf[2];

                result= false;
                goto result;
            }
        }
    }
result:
//    for(i=0;i<8;i++)
//        buf_pre[i]=buf[i];
    return result;
}
bool key_capture(u8 *buf)
{
    int retval;
    retval=key_handle(buf);
//    if(lua_flag&LUA_MACRO_PLAY_MASK)
//    {
//        lua_flag&=(~LUA_MACRO_PLAY_MASK);
//        return macro_play();

//    }
    return retval;
    
}

void blue_set(cap * cap_this)
{
    u8 buf[9];
    u8 i=0;
    buf[0]=10;
    for(i=1;i<9;i++)
    {
        buf[i]=0;
    }
    rt_mq_send(mq_commu,(void *)buf,9);
    blue_choose=1-blue_choose;
    if(blue_choose)
        cmd("Blue remote");
    else
    {
        cmd("Blue closed");
    }
}
void  key_cap_add(cap* cap_this)
{
    u16 i=0;
    cap * cap_new;
    //DBG("++++++++++");
    if(cap_free_head->next==RT_NULL||key_cap_cnt==key_cap_cnt_all)
    {
        return ;
    }
    else
    {
        cap_new=cap_free_head;
        cap_free_head=cap_free_head->next;
    }

    cap_new->filter=cap_this->filter;
    cap_new->key_exe=cap_this->key_exe;
    cap_new->string=cap_this->string;
    cap_new->string_lenth=cap_this->string_lenth;
    cap_new->flag=cap_this->flag;
    if(key_cap_cnt==0)
    {
        cap_use_head=cap_new;
    }
    else
    {
        cap** cap_current=&cap_use_head;
        for(i=0;i<key_cap_cnt;i++)
        {
            cap_current=&((*cap_current)->next);
        }
        *cap_current=cap_new;
    }
    
    key_cap_cnt++;

}

void ultrasonic_init()
{
    SCPE(PERIOA);
    IOConfig(IOAB,PIN0,tuiwanshuchu);
    IOConfig(IOAB,PIN1,xialashuru);
    DBG("ultrasonic_init");
    
}

void reset_system(struct st_key_cap* a)
{
NVIC_SystemReset();
}
ALIGN(RT_ALIGN_SIZE)
char thread_app_stack[10196];
struct rt_thread thread_app;
extern rt_mq_t mq_key_ms;
bool system_init=false;
rt_mq_t mq_key_sm;
void rt_thread_entry_app(void* parameter)
{
//    static FATFS fs;  		//�߼����̹�����.
//    static FIL file;	  		//�ļ�1
	while(!system_init){
		rt_thread_delay(50);
	}
    u8 key_temp[2];
    cap  cap_this2;
    ctrl_filter filter;
    block_info block;
    {
        u8 buf[9]={0};
        rt_mq_send (mq_commu, (void*)buf, 9);
    }
    if(ini.Service.key_remap)
        key_remap_init();
    if(ini.Service.ahk)
        ahk_init((char*)"/mode_1");
    if(ini.Service.ultrasonic)
        ultrasonic_init();
    if(ini.Service.lua_script)
        lua_init();


    
    key_temp[0]=0;
    key_temp[1]='^';
    filter_Init(&filter);
    filter_add(&filter,key_temp);
    filter.key=53;
    block.filter=filter;
    cap_this2.filter=block.filter;
    cap_this2.key_exe=blue_set;
    key_cap_add(&cap_this2);
    
    key_temp[0]=0;
    key_temp[1]='!';
    filter_Init(&filter);
    filter_add(&filter,key_temp);
    filter.key=53;
    block.filter=filter;
    cap_this2.filter=block.filter;
    cap_this2.key_exe=blue_set;
    key_cap_add(&cap_this2);
    
    key_temp[0]=0;
    key_temp[1]='!';
    filter_Init(&filter);
    filter_add(&filter,key_temp);
    key_temp[0]=0;
    key_temp[1]='^';
    filter_add(&filter,key_temp);
    filter.key=ascii2usb['k'];
    block.filter=filter;
    cap_this2.filter=block.filter;
    cap_this2.key_exe=pwd_start;
    key_cap_add(&cap_this2);
    
    key_temp[0]=0;
    key_temp[1]='!';
    filter_Init(&filter);
    filter_add(&filter,key_temp);
    key_temp[0]=0;
    key_temp[1]='^';
    filter_add(&filter,key_temp);
    filter.key=ascii2usb['r'];
    block.filter=filter;
    cap_this2.filter=block.filter;
    cap_this2.key_exe=reset_system;
    key_cap_add(&cap_this2);
    while(1){
		u8 buf[9];
		u8 buf2[9];
		rt_mq_recv(mq_key_ms,buf,8,RT_WAITING_FOREVER);
		for(u8 i=0;i<8;i++){
			buf2[i]=buf[i];
		}
		buf2[8]=0;//normal
		buf[8]=0;
		rt_kprintf("CNT=%d\r\n",key_cap_cnt);
		for(u8 i=0;i<8;i++)
		{
			rt_kprintf("+%d\r\n",buf[i]);
		}
		
		if(key_capture(buf))
		{ 
//			common_commu_send(buf2,8,COMMU_TYPE(KEYBOARD_SM));
			if(ini.Debug.keyboard)
			{
				rt_mq_send(mq_key_sm, (void*)buf, 9);
			}
			
		  //   rt_mb_send (mb_commu, (rt_uint32_t)buf);
			rt_kprintf("not");
		}else{
			rt_kprintf("capture");
		}
	}
    
    
//    key_temp[0]=2;//right ctrl
//    key_temp[1]='^';
//    filter_Init(&filter);
//    filter_add(&filter,key_temp);
//    filter.key=ascii2usb['o'];
//    block.filter=filter;
//    cap_this2.filter=block.filter;
//    cap_this2.key_exe=macro_set;
//    key_cap_add(&cap_this2);
//    
//    key_temp[0]=2;//right ctrl
//    key_temp[1]='^';
//    filter_Init(&filter);
//    filter_add(&filter,key_temp);
//    filter.key=ascii2usb['i'];
//    block.filter=filter;
//    cap_this2.filter=block.filter;
//    cap_this2.key_exe=macro_play_prepare;
//    key_cap_add(&cap_this2);

}
u8 getkey()
{
    return 0;
}
static   const  uint8_t  HID_KEYBRD_Codes[] = {
    0,     0,    0,    0,   31,   50,   48,   33,
    19,   34,   35,   36,   24,   37,   38,   39,       /* 0x00 - 0x0F */
    52,    51,   25,   26,   17,   20,   32,   21,
    23,   49,   18,   47,   22,   46,    2,    3,       /* 0x10 - 0x1F */
    4,    5,    6,    7,    8,    9,   10,   11,
    43,  110,   15,   16,   61,   12,   13,   27,       /* 0x20 - 0x2F */
    28,   29,   42,   40,   41,    1,   53,   54,
    55,   30,  112,  113,  114,  115,  116,  117,       /* 0x30 - 0x3F */
    118,  119,  120,  121,  122,  123,  124,  125,
    126,   75,   80,   85,   76,   81,   86,   89,       /* 0x40 - 0x4F */
    79,   84,   83,   90,   95,  100,  105,  106,
    108,   93,   98,  103,   92,   97,  102,   91,       /* 0x50 - 0x5F */
    96,  101,   99,  104,   45,  129,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,       /* 0x60 - 0x6F */
    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,       /* 0x70 - 0x7F */
    0,    0,    0,    0,    0,  107,    0,   56,
    0,    0,    0,    0,    0,    0,    0,    0,       /* 0x80 - 0x8F */
    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,       /* 0x90 - 0x9F */
    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,       /* 0xA0 - 0xAF */
    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,       /* 0xB0 - 0xBF */
    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,       /* 0xC0 - 0xCF */
    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,       /* 0xD0 - 0xDF */
    58,   44,   60,  127,   64,   57,   62,  128        /* 0xE0 - 0xE7 */
};
static  const  int8_t  HID_KEYBRD_Key[] = {
  '\0',  '`',  '1',  '2',  '3',  '4',  '5',  '6',
  '7',  '8',  '9',  '0',  '-',  '=',  '\0', '\r',
  '\t',  'q',  'w',  'e',  'r',  't',  'y',  'u', 
  'i',  'o',  'p',  '[',  ']',  '\\',
  '\0',  'a',  's',  'd',  'f',  'g',  'h',  'j',  
  'k',  'l',  ';',  '\'', '\0', '\n',
  '\0',  '\0', 'z',  'x',  'c',  'v',  'b',  'n', 
  'm',  ',',  '.',  '/',  '\0', '\0',
  '\0',  '\0', '\0', ' ',  '\0', '\0', '\0', '\0', 
  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
  '\0',  '\0', '\0', '\0', '\0', '\r', '\0', '\0', 
  '\0', '\0', '\0', '\0', '\0', '\0',
  '\0',  '\0', '7',  '4',  '1',
  '\0',  '/',  '8',  '5',  '2',
  '0',   '*',  '9',  '6',  '3',
  '.',   '-',  '+',  '\0', '\n', '\0', '\0', '\0', '\0', '\0', '\0',
  '\0',  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', 
  '\0', '\0', '\0', '\0'
};

static  const  int8_t  HID_KEYBRD_ShiftKey[] = {
  '\0', '~',  '!',  '@',  '#',  '$',  '%',  '^',  '&',  '*',  '(',  ')',
  '_',  '+',  '\0', '\0', '\0', 'Q',  'W',  'E',  'R',  'T',  'Y',  'U', 
  'I',  'O',  'P',  '{',  '}',  '|',  '\0', 'A',  'S',  'D',  'F',  'G', 
  'H',  'J',  'K',  'L',  ':',  '"',  '\0', '\n', '\0', '\0', 'Z',  'X',  
  'C',  'V',  'B',  'N',  'M',  '<',  '>',  '?',  '\0', '\0',  '\0', '\0',
  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', 
  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', 
  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
  '\0', '\0', '\0', '\0', '\0', '\0', '\0',    '\0', '\0', '\0', '\0', '\0',
  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'
};
#define  KBD_LEFT_CTRL                                  0x01
#define  KBD_LEFT_SHIFT                                 0x02
#define  KBD_LEFT_ALT                                   0x04
#define  KBD_LEFT_GUI                                   0x08
#define  KBD_RIGHT_CTRL                                 0x10
#define  KBD_RIGHT_SHIFT                                0x20
#define  KBD_RIGHT_ALT                                  0x40
#define  KBD_RIGHT_GUI                                  0x80
#define  KBR_MAX_NBR_PRESSED                            6
#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif
static void KEYBRD_Decode(uint8_t *pbuf)
{
  static  uint8_t   shift;
  static  uint8_t   keys[6];
  static  uint8_t   keys_new[6];
  static  uint8_t   keys_last[6];
  static  uint8_t   key_newest;
  static  uint8_t   nbr_keys;
  static  uint8_t   nbr_keys_new;
  static  uint8_t   nbr_keys_last;
  u8 i=0;
  uint8_t   ix;
  uint8_t   jx;
  uint8_t   error;
  uint8_t   output;            
  
  nbr_keys      = 0;
  nbr_keys_new  = 0;
  //nbr_keys_last = 0;
  key_newest    = 0x00;
                                                                       
  if ((pbuf[0] == KBD_LEFT_SHIFT) || (pbuf[0] == KBD_RIGHT_SHIFT)) {
    shift = TRUE;
  } else {
    shift = FALSE;
  }
  
  error = FALSE;
  
  /* Check for the value of pressed key */
  for (ix = 2; ix < 2 + KBR_MAX_NBR_PRESSED; ix++) {                       
    if ((pbuf[ix] == 0x01) ||
        (pbuf[ix] == 0x02) ||
          (pbuf[ix] == 0x03)) {
            error = TRUE;
          }
  }
  
  if (error == TRUE) {
    return;
  }
  
  nbr_keys     = 0;
  nbr_keys_new = 0;
  for (ix = 2; ix < 2 + KBR_MAX_NBR_PRESSED; ix++) {
    if (pbuf[ix] != 0) {
      keys[nbr_keys] = pbuf[ix];                                       
      nbr_keys++;
      for (jx = 0; jx < nbr_keys_last; jx++) {                         
        if (pbuf[ix] == keys_last[jx]) {
          break;
        }
      }
      
      if (jx == nbr_keys_last) {
        keys_new[nbr_keys_new] = pbuf[ix];
        nbr_keys_new++;
      }
    }
  }
  
  for(i=0;i<nbr_keys_new;i++) {
    key_newest = keys_new[i];
    
    if (shift == TRUE) {
      output =  HID_KEYBRD_ShiftKey[HID_KEYBRD_Codes[key_newest]];
    } else {
      output =  HID_KEYBRD_Key[HID_KEYBRD_Codes[key_newest]];
    }
    
    /* call user process handle */
    

    rt_mb_send(mb_app, (rt_uint32_t )output);

  }
  
  
  nbr_keys_last  = nbr_keys;
  for (ix = 0; ix < KBR_MAX_NBR_PRESSED; ix++) {
    keys_last[ix] = keys[ix];
  }
}
void macro_record(u8 *data)
{
    int i=0;
    for(i=0;i<10;i++)
    {
        macro_data[macro_lenth][i]=data[i];
    }
    macro_lenth++;
}
void macro_set(cap * cap_this)
{
    if(!macro_flag)
    {
        macro_flag=1;
        macro_lenth=0;
    }
    else
    {
        macro_flag=0;
        macro_lenth--;
    } 
}
void macro_play_prepare(cap * cap_this)
{
    macro_play_flag=1;
}
bool macro_play()
{
    int i=0,j=0;
    u8 mail[10];
    macro_flag=0;
    printf("play%d\r\n",macro_lenth);
    for(i=0;i<macro_lenth;i++)
    {
        if(key_handle(macro_data[i]))
        {
            mail[0]=0;
            for(j=0;j<8;j++)
            {
                mail[j+1]=macro_data[i][j];
            }
            mail[8]=92;
            commu_send(mail,9); 
        }
        rt_thread_delay(10);
    }
   

    return false;
}




