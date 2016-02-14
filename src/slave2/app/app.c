#include "app.h"
#include "string.h"
#include "ahk.h"
#include "keyboard.h"
#include "lua_inf.h"
/******************macro area******************/
u8 macro_play_flag;
void macro_play_prepare(cap * cap_this);

u8 macro_flag;
u8 macro_lenth;
void macro_record(u8*);
void macro_set(cap * cap_this);
bool macro_play(void);
/******************macro area end******************/
u8 *read_buf;
u8 buf_out[10];
void KEYBRD_Decode(uint8_t *pbuf);
void buf_clear(void);
cap key_cap_free[key_cap_cnt_all];
//st_key_cap key_cap_use[200];
cap* cap_use_head=0;
cap* cap_free_head=key_cap_free;
u16 key_cap_cnt=0;
void reset_system(struct st_key_cap* a);
const u8  ascii2usb[]={
    0,0,0,0,  0,0,0,0,  0,0,0,0,  0,40,0,0, \
    0,0,0,0,  0,0,0,0,  0,0,0,0,  0,0,0,0, \
    44,30,52,32,  33,34,36,52,  38,39,37,46,  54,86,55,56, \
    39,30,31,32,  33,34,35,36,  37,38,51,51,  54,46,55,56, \

    31,4,5,6,  7,8,9,10,  11,12,13,14,  15,16,111,18, \
    19,20,21,22,  23,24,25,26,  27,28,29,0,  0,0,0,0, \
    53,4,5,6,  7,8,9,10,  11,12,13,14,  15,16,17,18, \
    19,20,21,22,  23,24,25,26,  27,28,29,0,  50,0,53,0, \
};
const u8  *const p_ascii2usb=ascii2usb;
const u8 shift_table[]={
    0,0,0,0,    0,0,0,0,    0,0,0,0,    0,0,0,0,
    0,0,0,0,    0,0,0,0,    0,0,0,0,    0,0,0,0,
    0,1,1,1,    1,1,1,1,    1,1,1,1,    0,0,0,0,
    0,0,0,0,    0,0,0,0,    0,0,1,0,    1,0,1,1,

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
    key_cap_free[key_cap_cnt-1].next=0;
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


void  key_cap_add(cap* cap_this)
{
//	printf("add%d %d %d %d",cap_free_head,cap_free_head->next,key_cap_cnt,key_cap_cnt_all);
    u16 i=0;
    cap * cap_new;
    //DBG("++++++++++");
    if(cap_free_head->next==0||key_cap_cnt==key_cap_cnt_all)
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



void reset_system(struct st_key_cap* a)
{
//NVIC_SystemReset();
    u16 output=0;
    press_string_pure(&output,1);
    press_string_pure(&output,1);
	printf("resetting\r\n");
	SCB->AIRCR = 0x05FA0000| (u32)0x04;
}


u8 system_init=false;

u8 getkey()
{
    return 0;
}



//static void KEYBRD_Decode(uint8_t *pbuf)
//{
//  static  uint8_t   shift;
//  static  uint8_t   keys[6];
//  static  uint8_t   keys_new[6];
//  static  uint8_t   keys_last[6];
//  static  uint8_t   key_newest;
//  static  uint8_t   nbr_keys;
//  static  uint8_t   nbr_keys_new;
//  static  uint8_t   nbr_keys_last;
//  u8 i=0;
//  uint8_t   ix;
//  uint8_t   jx;
//  uint8_t   error;
//  uint8_t   output;            
//  
//  nbr_keys      = 0;
//  nbr_keys_new  = 0;
//  //nbr_keys_last = 0;
//  key_newest    = 0x00;
//                                                                       
//  if ((pbuf[0] == KBD_LEFT_SHIFT) || (pbuf[0] == KBD_RIGHT_SHIFT)) {
//    shift = TRUE;
//  } else {
//    shift = FALSE;
//  }
//  
//  error = FALSE;
//  
//  /* Check for the value of pressed key */
//  for (ix = 2; ix < 2 + KBR_MAX_NBR_PRESSED; ix++) {                       
//    if ((pbuf[ix] == 0x01) ||
//        (pbuf[ix] == 0x02) ||
//          (pbuf[ix] == 0x03)) {
//            error = TRUE;
//          }
//  }
//  
//  if (error == TRUE) {
//    return;
//  }
//  
//  nbr_keys     = 0;
//  nbr_keys_new = 0;
//  for (ix = 2; ix < 2 + KBR_MAX_NBR_PRESSED; ix++) {
//    if (pbuf[ix] != 0) {
//      keys[nbr_keys] = pbuf[ix];                                       
//      nbr_keys++;
//      for (jx = 0; jx < nbr_keys_last; jx++) {                         
//        if (pbuf[ix] == keys_last[jx]) {
//          break;
//        }
//      }
//      
//      if (jx == nbr_keys_last) {
//        keys_new[nbr_keys_new] = pbuf[ix];
//        nbr_keys_new++;
//      }
//    }
//  }
//  
//  for(i=0;i<nbr_keys_new;i++) {
//    key_newest = keys_new[i];
//    
//    if (shift == TRUE) {
//      output =  HID_KEYBRD_ShiftKey[HID_KEYBRD_Codes[key_newest]];
//    } else {
//      output =  HID_KEYBRD_Key[HID_KEYBRD_Codes[key_newest]];
//    }
//    
//    /* call user process handle */
//    

//    rt_mb_send(mb_app, (rt_uint32_t )output);

//  }
//  
//  
//  nbr_keys_last  = nbr_keys;
//  for (ix = 0; ix < KBR_MAX_NBR_PRESSED; ix++) {
//    keys_last[ix] = keys[ix];
//  }
//}

u8 key_handle(u8 *buf)
{
//    printf("%d",key_cap_cnt);
    u8 i=0;
    static  u8 hotkey_flag=0;
    static  u8 hotkey_value=0;
    u8 control_key_this;
    u8 result=0;
    
    control_key_this=buf[0];
 
    if(hotkey_flag)
    {

        if(buf[2]==hotkey_value&&hotkey_flag==1)
        {
            DBG("I block it!\r\n");// still press
            buf[2]=0;
            buf[0]=0;
            result= false;
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

//    if(mb_app!=0)
//    {
//        KEYBRD_Decode(buf);
//        result= false;
//        goto result;
//    }
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

                result= true;
                goto result;
            }
        }
    }
result:
//    for(i=0;i<8;i++)
//        buf_pre[i]=buf[i];
    return result;
}
extern u8 use_lua;
u8 key_capture(u8 *buf,key_t* bufp)
{
    int through1,through2;
    
    through2=key_handle(buf);
    if(use_lua){
        through2|=lua_handle(bufp);
    }
//    if(lua_flag&LUA_MACRO_PLAY_MASK)
//    {
//        lua_flag&=(~LUA_MACRO_PLAY_MASK);
//        return macro_play();

//    }
    return through2;
    
}
FATFS fs;
int fgetc(FILE *f){
	return 1;
}
//int fputc (int ch, FILE *f) {

//  return (ch);
//}


void app_init()
{
	
	if(clean_mode)
		return;
	u16 i=0;
    for(i=0;i<key_cap_cnt_all-1;i++)
    {
        key_cap_free[i].next=&key_cap_free[i+1];
    }
    key_cap_free[key_cap_cnt-1].next=0;
	
	
	
    u8 key_temp[2];
    cap  cap_this2;
    ctrl_filter filter;
    block_info block;
	u8 result=f_mount(&fs,"/",1);
    if(result)
    {
        DBG("FS mount failed!!\r\n%d",result);
		return;
    }
//    ahk_init((char*)"/mode_1");



    
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
	
	lua_init();
}
void app_handle(u8 *buf,key_t* bufp){
//	DBG("CNT=%d\r\n",key_cap_cnt);
//	for(u8 i=0;i<8;i++)
//	{
//		DBG("+%d",buf[i]);
//	}
	if(clean_mode){
		app_press(buf);
	}
	else{
		if(!key_capture(buf, bufp))
        { 
            app_press(buf);
            
        }else{
//            DBG("capture");
        }
	}
	
}

