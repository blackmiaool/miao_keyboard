#include "pwd.h"
ALIGN(RT_ALIGN_SIZE)
rt_thread_t thread_pwd;
extern rt_mailbox_t mb_app;
static rt_mailbox_t mb_key;
u8 getkbchar()
{
    u8 key;
    rt_mb_recv(mb_key, (rt_uint32_t *)&key, RT_WAITING_FOREVER);
    return key;
}
static void rt_thread_entry_pwd(void* parameter)
{
    u8 key_value;
    mb_key=(rt_mailbox_t )parameter;
    key_value=getkbchar();
    if(key_value=='g')
    {
        putchar('1');
    }
    else if(key_value=='s')
    {
        putchar('2');
    }
    
    
    
    rt_mb_delete(mb_key);
    mb_app=0;
}
void pwd_start(struct st_key_cap* key_cap)
{
    rt_thread_t thread_pwd;
    mb_app=rt_mb_create("mb_key", 10,RT_IPC_FLAG_FIFO);
    thread_pwd= rt_thread_create(
                   "app",
                   rt_thread_entry_pwd,
                   (void *)mb_app,
                   1024,15,5);
    rt_thread_startup(thread_pwd);
    
}

