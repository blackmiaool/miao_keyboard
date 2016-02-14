//#include "ahk.h"

//#define isCtrlKey(a) (a=='#'?true:a=='!'?true:a=='^'?true:a=='+'?true:false)

//u32 line_cnt;
//void no_use_printf(char *shuru,...)
//{

//}
//void no_use_putchar(u8 ch){
//}

//int ahk_init(char *path)
//{
//    FIL file; 
//    u32 cnt=0;

//    #define ahk_print(arg...) {do{if(true){DBG(arg);}else{ no_use_printf(arg);}}while(0);}
//    #define ahk_putchar(arg) {do{if(false){putchar(arg);}else{no_use_putchar(arg);}}while(0);}
//	
//    ahk_print("ahk-open:%d\r\n",f_open(&file,(const char*)path,FA_OPEN_EXISTING|FA_WRITE|FA_READ|FA_OPEN_ALWAYS|FA__WRITTEN));
//	read_buf=(u8*)malloc(file.fsize);
//    ahk_print("read:%d\r\n",f_read(&file,read_buf,file.fsize,&cnt));
//    ahk_print("size=%d\r\n",(int)file.fsize);
//	f_close(&file);
//	
////    for(i=0;i<file.fsize;i++)
////    {
////        ahk_putchar(read_buf[i]);
////    }
//    
//    if(key_mode_process(read_buf,file.fsize))
//    {
//        DBG("Key mode error");
//    }
//    else
//    {
////        DBG("Key mode done");
//    }
//	free(read_buf);
//    return 0;
//}
//u8 control_key_decode(u8 key)
//{
//    switch(key)
//    {
//    case '#':
//        return 1<<3;
//    case '!':
//        return 1<<2;
//    case '^':
//        return 1<<0;
//    case '+':
//        return 1<<1;
//    }
//    return 0xff;

//}
//u8 filter_add_string(struct control_key_filter* filter,const char *string )
//{
//    u8 key_value=1<<control_key_index(string);
//    filter->control_filter[filter->control_filter_cnt]=key_value;
//    if(filter->control_filter_cnt<7)
//    {
//        filter->control_filter_cnt++;
//    }
//    else
//    {
//        return 1;
//    }
//    return 0;
//}
//u8 filter_add(struct control_key_filter* filter,u8* filter_info)
//{
//    u8 key_value=control_key_decode(filter_info[1]);
//    switch(filter_info[0])
//    {
//    case 0:
//        filter->control_filter[filter->control_filter_cnt]=key_value|(key_value<<4);
//        break;
//    case 1:
//        filter->control_filter[filter->control_filter_cnt]=key_value;
//        break;
//    case 2:
//        filter->control_filter[filter->control_filter_cnt]=key_value<<4;
//        break;

//    }
//    if(filter->control_filter_cnt<7)
//    {
//        filter->control_filter_cnt++;
//    }
//    else
//    {
//        return 1;
//    }
//    return 0;
//}

//struct control_info
//{
//    enum ctrl_dir dir;
//    u8 value;
//    u8 state;//0:nothing 1:newest only dir 2:complete
//};
//typedef struct control_info ctrl_info;
//enum block_state{block_raw,contrl_key_gotton,full_quick_key_gotton,colon_gotton};

//enum token_class{token_raw,
//                 token_both_dir_ctrl,
//                 token_dir_ctrl,
//                 token_colon,
//                 token_key,
//                 token_long_key
//                };
//enum token_state{ts_raw,//wait for anything
//                 ts_wait_ctrl_key,//wait for ctrl key
//                 ts_wait_2nd_colon,
//                 ts_wait_key_value,
//                 ts_finish};
//struct token_struct
//{
//    enum token_class token_class;
//    u32 content;
//    enum token_state state;
//};
//typedef struct token_struct token;
//void token_Init(token* token_this)
//{
//    token_this->content=0;
//    token_this->token_class=token_raw;
//    token_this->state=ts_raw;
//}

//struct token_queue_struct
//{
//    token queue[80];//change when needed
//    u8 state;
//    u8 lenth;

//};
//typedef struct token_queue_struct token_q;
//void token_q_Init(token_q* q)
//{
//    u16 i=0;
//    for(i=0;i<80;i++)
//    {
//        token_Init(&q->queue[i]);
//    }
//    q->state=0;
//    q->lenth=0;
//}
//void ctrl_info_Init(ctrl_info* info)
//{
//    info->dir=ctrl_raw;
//    info->value=0;
//    info->state=0;
//}
//void filter_Init(ctrl_filter* filter)
//{
//    u8 i=0;
//    for(i=0;i<8;i++)
//    {
//        filter->control_filter[i]=0;
//    }
//    filter->control_filter_cnt=0;
//    filter->key=0;

//}
//void block_Init(block_info* block)
//{
//    block->state=block_raw;
//    filter_Init(&(block->filter));
//}
//enum char_class{
//    angle,//<>
//    ctrl_key,
//    colon,
//    general_key,
//    brace
//};
//enum char_class char_ana(u8 char_this)
//{
//    if(char_this=='<'||char_this=='>')
//    {
//        return angle;
//    }
//    else if(char_this=='#'||char_this=='!'||char_this=='^'||char_this=='+')
//    {
//        return ctrl_key;
//    }
//    else if(char_this==':')
//    {
//        return colon;
//    }
//    else if(char_this=='{'||char_this=='}')
//    {
//        return brace;
//    }
//    else
//        return general_key;

//}
//typedef enum token_state ts;
//const u8 general_key_value[]={0,
//                              40,41,42,43,
//                              44,57,58,59,
//                              60,61,62,63,
//                              64,65,66,67,

//                              68,69,70,71,
//                              72,73,74,75,
//                              76,77,78,79,
//                              80,81,82,83
//                             };
//const u8* general_key_map[]={"",
//                             "enter",            "esc",      "delete",       "tab",
//                             "space",            "capsLock","f1",           "f2",
//                             "f3",               "f4",       "f5",           "f6",
//                             "f7",               "f8",       "f9",           "f10",

//                             "f11",              "f12",      "printscreen",  "scrollLock",
//                             "pause",            "insert",   "home",         "pageup",
//                             "deleteforward",   "end",      "pagedown",     "right",
//                             "left",             "down",     "up",           "numLock"
//                            };
//u8 general_key_ana(u8* read_buf,u32 general_key_pos,u8 general_key_cnt)
//{
//    u8 string[21];
//    u8 i=0;
//    for(i=0;i<general_key_cnt;i++)
//    {
//        string[i]=read_buf[general_key_pos+i];
//    }
//    string[general_key_cnt]='\0';
//    //printf("----%s----",string);
//    for(i=0;i<sizeof(general_key_map);i++)
//    {
//        if(strcmp((const char*)string,(const char*)general_key_map[i])==0)
//        {
//            return general_key_value[i];
//        }
//    }
//    return 0;

//}
//u8 token_ana(token_q* token_queue,u8* read_buf,u32 ptr[2])
//{
//    //  u8* s;
//    u32 i=0;
//    u8 general_key_cnt=0;
//    u32 general_key_pos=0;

//    // enum token_state ts=ts_raw;

//    token_q_Init(token_queue);
//    for(i=ptr[0];i<ptr[1];i++)
//    {
//        u8 char_this=read_buf[i];
//        token* token_this=&(token_queue->queue[token_queue->lenth]);
//#ifdef ANA_DBG
//        DBG("(%c->%d)\r\n",char_this,token_this->state);
//#endif
//        switch(token_this->state)
//        {
//        case ts_raw:
//            switch(char_ana(char_this))
//            {
//            case angle:
//                token_this->state=ts_wait_ctrl_key;
//                if(char_this=='<')
//                    token_this->content|=(1<<7);
//                break;
//            case ctrl_key:
//                token_queue->lenth++;
//                token_this->state=ts_finish;
//                token_this->content=char_this;
//                token_this->token_class=token_both_dir_ctrl;
//                break;
//            case colon:
//                token_this->state=ts_wait_2nd_colon;
//                break;
//            case general_key:
//                token_queue->lenth++;
//                token_this->state=ts_finish;
//                token_this->content=char_this;
//                token_this->token_class=token_key;
//                break;
//            case brace:
//                token_this->state=ts_wait_key_value;
//                break;
//            default:
//                DBG("error:line%d---unknown char type found!\r\n",line_cnt);
//                return 1;

//            }
//            break;
//        case ts_wait_ctrl_key:
//            if(char_ana(char_this)==ctrl_key)
//            {
//                token_queue->lenth++;
//                token_this->state=ts_finish;
//                token_this->content|=char_this;
//                token_this->token_class=token_dir_ctrl;
//            }
//            else
//            {
//                DBG("error:line%d---wait for ctrl_key but found%c\r\n",line_cnt,char_this);
//                return 2;
//            }
//            break;
//        case ts_wait_2nd_colon:
//            if(char_ana(char_this)==colon)
//            {
//                token_queue->lenth++;
//                token_this->state=ts_finish;
//                token_this->content=char_this;
//                token_this->token_class=token_colon;
//            }
//            else
//            {
//                DBG("error:line%d---wait for colon but found%c\r\n",line_cnt,char_this);
//                return 3;
//            }
//            break;
//        case ts_wait_key_value:
//            if(char_ana(char_this)==general_key)
//            {
//                if(general_key_cnt==0)
//                {
//                    general_key_pos=i;
//                }
//                if(general_key_cnt<20)
//                {
//                    general_key_cnt++;
//                }
//                else
//                {
//                    DBG("error:line%d---unknown general key(too long)%c\r\n",line_cnt,char_this);
//                    return 4;
//                }



//            }
//            else if(char_ana(char_this)==brace)
//            {
//                u8 general_key_index;

//                general_key_index=general_key_ana(read_buf,general_key_pos,general_key_cnt);
//                general_key_cnt=0;
//                general_key_pos=0;
//                if(general_key_index==0)
//                {
//                    DBG("error:line%d---unknown general key%c\r\n",line_cnt,char_this);
//                    return 5;
//                }
//                else
//                {
//                    token_queue->lenth++;
//                    token_this->state=ts_finish;
//                    token_this->content=general_key_index;
//                    token_this->token_class=token_long_key;
//                }

//            }
//            else
//            {
//                DBG("error:line%d---unknown word found when wait for general key%c\r\n",line_cnt,char_this);
//                return 6;
//            }
//            break;

//        }

//        // printf("ana=%c(%d)",char_this,char_ana(char_this));
//    }
////	DBG("%d ",token_queue->lenth);
//    if(token_queue->queue[token_queue->lenth].state==token_raw)
//        return 0;
//    else
//    {
//        DBG("error:line%d---unexpected end",line_cnt);
//        return 7;
//    }
//}
//#define compile_DBG(a) do{DBG("error:line%d---",line_cnt);DBG(a);DBG("\r\n");}while(0)
//u16 token2usb(token* token_get)
//{
//    if(token_get->token_class==token_key)
//    {
//        return (shift_table[token_get->content]<<8)|ascii2usb[token_get->content];
//    }
//    else //if(token_get->token_class==token_long_key)
//    {
//        return token_get->content;
//    }
//}


//void press_string(cap * cap_this);
//void  key_cap_add(cap* cap_this);

//u16 string_index=0;
//u8 mode_line_process(u8* read_buf,u32 ptr[2])
//{
//    u32 i=0;
////    static u8 ana_state=0;// 0:find quick key 1:find"::" 2:?
//    //static u8 control_info[2];//[0]:1:< 2:> 0:both dir  [1]:whitch key
////    static u8 control_key_state=0;//1:dir word found,wait for key  0:wait for dir or key
////    ctrl_info info_temp;
//    static block_info block;
//    static token_q token_queue;
//    cap cap_this;

////                   0,
////                   press_string,
////                   RT_NULL,
////                   my_id,
////                   sizeof(my_id)};
////    u8 filter_cnt=0;

//    //DBG("line_cnt=%d~\r\n",line_cnt);
//    if(read_buf[ptr[0]]==';')
//    {
//        line_cnt++;
//        return 0;
//    }

//    if(token_ana(&token_queue,read_buf,ptr))
//        return 1;
//  //  DBG("ANASUCCESS");
//#undef ANA_DEBUG
//#ifdef ANA_DEBUG
//    for(i=0;i<token_queue.lenth;i++)
//    {
//        printf("queue[%d],class=",i);
//        switch(token_queue.queue[i].token_class)
//        {
//        case token_raw:
//            printf("token_raw");
//            break;
//        case token_both_dir_ctrl:
//            printf("token_both_dir_ctrl");
//            break;
//        case token_dir_ctrl:
//            printf("token_dir_ctrl");
//            break;
//        case token_colon:
//            printf("token_colon");
//            break;
//        case token_key:
//            printf("token_key");
//            break;
//        }
//        printf(",content=%d\r\n",token_queue.queue[i].content);
//    }
//#endif
//    //    enum token_class{token_raw,
//    //                     token_both_dir_ctrl,
//    //                     token_dir_ctrl,
//    //                     token_colon,
//    //                     token_key};
//    // enum block_state{raw,contrl_key_gotton,full_quick_key_gotton,colon_gotton};
//    for(i=0;i<token_queue.lenth;i++)
//    {
//        token* token_this=&(token_queue.queue[i]);
//        enum token_class class_this=token_this->token_class;
//        switch(block.state)
//        {
//        case block_raw:
//            block_Init(&block);
//        case contrl_key_gotton:
//            if(class_this==token_both_dir_ctrl)
//            {
//                block.filter.control_filter[block.filter.control_filter_cnt++]=
//                        control_key_decode(token_this->content)|
//                        (control_key_decode(token_this->content)<<4);
//                //DBG("block_raw\r\n");
//                block.state=contrl_key_gotton;
//            }
//            else if(class_this==token_dir_ctrl)
//            {
//                if(token_this->content&0x80)
//                {
//                    block.filter.control_filter[block.filter.control_filter_cnt++]=
//                            control_key_decode(token_this->content&(0x7f));
//                }
//                else
//                {
//                    block.filter.control_filter[block.filter.control_filter_cnt++]=
//                            control_key_decode(token_this->content&(0x7f))<<4;
//                }
//                //DBG("token_dir_ctrl\r\n");
//                block.state=contrl_key_gotton;
//            }
//            else if(block.state==block_raw)
//            {
//                compile_DBG("Control key not found!");
//            }
//            else if(class_this==token_key||class_this==token_long_key)
//            {
//                block.filter.key=token2usb(token_this);
//                block.state=full_quick_key_gotton;
//            }
//            else
//            {
//                compile_DBG("Unexpected token_class found when make control key!");
//            }
//            break;
//        case full_quick_key_gotton:
//            if(class_this!=token_colon)
//            {
//                compile_DBG("Didn't find colon!");
//            }
//            else
//            {
//                block.state=colon_gotton;
//            }
//            break;
//        case colon_gotton:
//        {
//            u32 k=0;
//            u32 key_cnt=0;
//            u16* key_array;
//            for(k=i;k<token_queue.lenth;k++)
//            {
//                token* token_this=&(token_queue.queue[k]);
//                enum token_class class_this=token_this->token_class;
//                if(class_this==token_key||class_this==token_long_key)
//                {
//                    key_cnt++;
//                }
//                else
//                {
//                    compile_DBG("Unexpected token_class found when get general key!");
//                    return 1;
//                }
//            }

//			key_array=malloc(key_cnt*2);
//            for(k=i;k<token_queue.lenth;k++)
//            {
//                key_array[k-i]=token2usb(&token_queue.queue[k]);
//            }

//            cap_this.filter=block.filter;
//            cap_this.key_exe=press_string;
//            cap_this.string=key_array;
//            cap_this.string_lenth=key_cnt;
//            key_cap_add(&cap_this);

//            block.state=block_raw;
//            goto end;
//         
//        }



//        }
//    }
//end:;


//    line_cnt++;

//	return 0;
//}
//u8 key_mode_process(u8* read_buf,u32 size)
//{
//    u32 i=0;
//    u32 write_cnt=0;
//    u8 read_state=0;
//    u32 line_ptr[2];

//    for(i=0;i<size;i++)
//    {
//        char char_this=read_buf[i];
//        if(char_this!=0x0d&&char_this!=' '&&char_this!='\t')
//        {
//            if((char_this>='A')&&(char_this<='Z'))
//            {
//                char_this+=('a'-'A');
//            }
//            read_buf[write_cnt]=char_this;
//            write_cnt++;
//        }
//        else
//        {

//        }
//    }

//    for(i=0;i<=write_cnt;i++)
//    {
//        //		u8 char_this=read_buf[i];
//        //printf("read_state=%d,%c\r\n",read_state,read_buf[i]);
//        if(i==write_cnt)
//        {
//            if(read_state!=2&&read_state!=0)
//            {
//                cmd("read_state error!");
//                break;
//            }
//            else
//            {
//                if(read_state==2)
//                {
//                    line_ptr[1]=i;
//                    mode_line_process(read_buf,line_ptr);
//                }
//                break;
//            }
//        }
//        switch(read_state)
//        {
//        case 0:
//        {

//            if(read_buf[i]!=0x0a)
//            {
//                line_ptr[0]=i;
//                read_state=1;
//            }
//            break;
//        }
//        case 1:
//        {

//            if(read_buf[i]==0x0a)
//            {
//                read_state=0;
//                line_ptr[1]=i;
//                mode_line_process(read_buf,line_ptr);
//            }

//            break;
//        }
//        }
//    }
//    return 0;
//}


//////buf 0~7:USBcode 8:if press LShift 9:control_key add 10:control_key del
