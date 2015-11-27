#include "key_remap.h"

void table_line_process(u8* read_buf,u32* token_ptr)
{
    u32 i=0,j=0;
    u8 index_left;
    u8 index_right;
    for(i=0;i<key_string_num;i++)
    {
        for(j=token_ptr[0];j<token_ptr[1];j++)
        {
            if(read_buf[j]!=key_string[i][j-token_ptr[0]])
            {
                goto fail;
            }

        }
        index_left=i;
        //DBG("find_index=%d\r\n",index_left);
        break;
fail:;
    }

    for(i=0;i<key_string_num;i++)
    {
        for(j=token_ptr[1]+1;j<token_ptr[2];j++)
        {
            if(read_buf[j]!=key_string[i][j-token_ptr[1]-1])
            {
                //DBG("alhpa%c!=%c!",read_buf[j],key_string[i][j]);
                goto fail2;
            }

        }
        index_right=i;
        //printf("find_index2=%d\r\n",index_right);
        break;
fail2:;
    }
    key_changetable[index_left]=index_right;


}
u8 key_remap_init()
{
     FIL file;
     u32 cnt=0;
     u32 i=0;
    #define key_remap_print(arg...) {do{if(ini.Debug.key_remap){DBG(arg);}else{ no_use_printf(arg);}}while(0);}
    #define key_remap_putchar(arg) {do{if(ini.Debug.key_remap){putchar(arg);}else{no_use_putchar(arg);}}while(0);}
    if(ini.Service.key_remap)    
    {
   
        printf("open:%d",f_open(&file,"/key_t",FA_OPEN_EXISTING|FA_WRITE|FA_READ|FA_OPEN_ALWAYS|FA__WRITTEN));
        printf("read:%d",f_read(&file,read_buf,file.fsize,&cnt));
        printf("size=%d\r\n",file.fsize);
        for(i=0;i<file.fsize;i++)
        {
        key_remap_putchar(read_buf[i]);
        }
        f_close(&file);
        if(key_table_process(read_buf,file.fsize))
        {
                cmd("Key table error");
        }
        else
        {
                cmd("Key table done");
        }
    }    
    return 0;
}
u8 key_table_process(u8* read_buf,u32 size)
{
    u32 i=0;
    u32 write_cnt=0;
    u8 read_state=0;
    u32 line_ptr[3];
    for(i=0;i<size;i++)
    {
        char char_this=read_buf[i];
        if(((char_this>='a')&&(char_this<='z'))||((char_this>='A')&&(char_this<='Z'))||char_this=='='||char_this==0x0a)
        {
            if((char_this>='A')&&(char_this<='Z'))
            {
                char_this+=('a'-'A');
            }
            read_buf[write_cnt]=char_this;
            write_cnt++;
        }
        else
        {
            
        }
    }
    for(i=0;i<write_cnt;i++)
    {
        putchar(read_buf[i]);
    }

    for(i=0;i<=write_cnt;i++)
    {
        //		u8 char_this=read_buf[i];
        //printf("read_state=%d,%c\r\n",read_state,read_buf[i]);
        if(i==write_cnt)
        {
            if(read_state!=2&&read_state!=0)
            {
                cmd("read_state error!");
            }
            else
            {
                if(read_state==2)
                {
                    line_ptr[2]=i;
                    read_state=0;
                    table_line_process(read_buf,line_ptr);
                }
                break;

            }
        }
        switch(read_state)
        {
        case 0:
        {

            if(read_buf[i]!=0x0a)
            {
                line_ptr[0]=i;
                read_state=1;
            }
            //			if(read_buf[i]=='l'||read_buf[i]=='r')
            //			{
            //				cmd("Table file is OK");
            //				read_state=1;
            //			}
            //			else
            //			{
            //				u8 send_buf[20];
            //				cmd("Table file is wrong!");
            //				sprintf(send_buf,"find %c!!",read_buf[write_cnt]);
            //				cmd(send_buf);
            //				goto end;
            //			}
            break;
        }
        case 1:
        {

            if(read_buf[i]=='=')
            {
                read_state=2;
                line_ptr[1]=i;
            }
            break;
        }
        case 2://wait for end
        {

            if(read_buf[i]==0x0a)
            {
                line_ptr[2]=i;
                read_state=0;
                table_line_process(read_buf,line_ptr);
            }
            break;
        }
        }
    }
    //	end:;
    return 0;
}
