#include "keyboard.h"
#include "stdlib.h"
#include <rtthread.h>
#include "tm_stm32f4_usb_hid_device.h"
void put_num(int d){
    rt_kprintf("%d \n",d);
}
struct GPIO_struct{
    GPIO_TypeDef* port;
    uint16_t pin;
};
struct GPIO_struct keyboard_gpio_rows[5];
struct GPIO_struct keyboard_gpio_cols[14];
void keyboard_init(void){
    TM_GPIO_Init(GPIOC, GPIO_PIN_4, TM_GPIO_Mode_IN, TM_GPIO_OType_PP, TM_GPIO_PuPd_DOWN, TM_GPIO_Speed_Low);
    TM_GPIO_Init(GPIOC, GPIO_PIN_5, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Low);
    //	TM_GPIO_SetPullResistor(GPIOC, GPIO_PIN_1, TM_GPIO_PuPd_UP);
    TM_GPIO_SetPinHigh(GPIOC,GPIO_PIN_5);
}
GPIO_TypeDef* char2port(char ch){
    switch(ch){
    case 'A':
        return (GPIO_TypeDef*)GPIOA;
    case 'B':
        return (GPIO_TypeDef*)GPIOB;
    case 'C':
        return (GPIO_TypeDef*)GPIOC;
    case 'D':
        return (GPIO_TypeDef*)GPIOD;
    case 'E':
        return (GPIO_TypeDef*)GPIOE;
    case 'F':
        return (GPIO_TypeDef*)GPIOF;
    case 'G':
        return (GPIO_TypeDef*)GPIOG;
    case 'H':
        return (GPIO_TypeDef*)GPIOH;
    case 'I':
        return (GPIO_TypeDef*)GPIOI;
    default:
        rt_kprintf("wrong port!!!\n");
        return (GPIO_TypeDef*)GPIOA;
        break;
    }
}
u32 str2pin(char *str){
    return 1<<atoi(str);
}
u8 col_len=0;
u8 row_len=0;
u16 key_val[5];
static int c_set_key_pos(lua_State *L){
    rt_kprintf("key = %s,%d,%s\n", lua_tostring(L, 1),lua_tointeger(L, 2),lua_tostring(L, 3));
    struct GPIO_struct * arr;
    if(lua_tostring(L, 1)[0]=='c'){
        arr=keyboard_gpio_cols;
        col_len=lua_tointeger(L, 2);
    }else{
        arr=keyboard_gpio_rows;
        row_len=lua_tointeger(L, 2);
    }
    arr[lua_tointeger(L, 2)-1].port=char2port(lua_tostring(L, 3)[0]);
    arr[lua_tointeger(L, 2)-1].pin=str2pin((char *)lua_tostring(L, 3)+1);
    return 0;

}
//TM_GPIO_Init(GPIOC, GPIO_PIN_4, TM_GPIO_Mode_IN, TM_GPIO_OType_PP, TM_GPIO_PuPd_DOWN, TM_GPIO_Speed_Low);
//TM_GPIO_Init(GPIOC, GPIO_PIN_5, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Low);
////	TM_GPIO_SetPullResistor(GPIOC, GPIO_PIN_1, TM_GPIO_PuPd_UP);
//TM_GPIO_SetPinHigh(GPIOC,GPIO_PIN_5);

static u8 key_index[5][14];
static void scan(){
    u8 i=0,j=0;
    for(j=0;j<row_len;j++){
        key_val[j]=0;
        TM_GPIO_SetPinHigh(keyboard_gpio_rows[j].port,keyboard_gpio_rows[j].pin);
    }

    for(j=0;j<row_len;j++){
        TM_GPIO_SetPinLow(keyboard_gpio_rows[j].port,keyboard_gpio_rows[j].pin);
        rt_thread_delay(1);
        for(i=0;i<col_len;i++){
            key_val[j]+=((TM_GPIO_GetInputPinValue(keyboard_gpio_cols[i].port, keyboard_gpio_cols[i].pin))<<i);
        }
        u16 mask=1<<col_len;
        mask--;
        key_val[j]=mask-key_val[j];
        TM_GPIO_SetPinHigh(keyboard_gpio_rows[j].port,keyboard_gpio_rows[j].pin);
    }

//    for(j=0;j<row_len;j++){
//        rt_kprintf("key value %d %d",j,key_val[j]);
//    }



    u8 key_data_index=0;
    static u8 pre_press=0;
    static u8 key_press=0;
    TM_USB_HIDDEVICE_Keyboard_t Keyboard;
    TM_USB_HIDDEVICE_KeyboardStructInit(&Keyboard);
    for(j=0;j<row_len;j++){
        for(i=0;i<col_len;i++){
            if(key_val[j]&(1<<i))
            {
                switch(key_index[j][i]){
                case 225:
                    Keyboard.L_SHIFT=1;
                    break;
                case 224:
                    Keyboard.L_CTRL=1;
                    break;
                case 227:
                    Keyboard.L_GUI=1;
                    break;
                case 226:
                    Keyboard.L_ALT=1;
                    break;
                case 228:
                    Keyboard.R_CTRL=1;
                    break;
                case 229:
                    Keyboard.R_SHIFT=1;
                    break;
                case 230:
                    Keyboard.R_ALT=1;
                    break;
                default:
                    Keyboard.key[key_data_index++] = key_index[j][i];
                }
                rt_kprintf("key press %d  %d %d\n",j,i, key_index[j][i]);
                pre_press=1;
                key_press=1;
                if(key_data_index==6){
                   goto end;
                }
            }
        }
    }

    if(key_press){
        TM_USB_HIDDEVICE_KeyboardSend(&Keyboard);
//        rt_kprintf("key press %d  %d %d\n",j,i,Keyboard.key[0]);
    }
    else if(pre_press){
        pre_press=0;
        TM_USB_HIDDEVICE_KeyboardSend(&Keyboard);
    }

end:

    rt_kprintf("\n");

}
void keyboard_io_init(){
    u8 i=0,j=0;
    for(i=0;i<col_len;i++){
        TM_GPIO_Init(keyboard_gpio_cols[i].port, keyboard_gpio_cols[i].pin, TM_GPIO_Mode_IN, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_High);
    }
    for(j=0;j<row_len;j++){
        TM_GPIO_Init(keyboard_gpio_rows[j].port, keyboard_gpio_rows[j].pin, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_High);
    }
}

static int c_set_key_index(lua_State *L){
    u8 row=lua_tointeger(L, 1)-1,col=lua_tointeger(L, 2)-1,index=lua_tointeger(L, 3);
    rt_kprintf("set key index row%d col%d index%d \n", row,col,index);
    key_index[row][col]=index;
    return 0;
}

void rt_lua_thread_entry(void* parameter){

    lua_State *L = luaL_newstate();
    //    lua_register(L, "key_pos_init", key_pos_init);
    lua_register(L, "c_set_key_pos", c_set_key_pos);
    lua_register(L, "c_set_key_index", c_set_key_index);
    //    luaL_dostring(L, "key_pos_init({{'A3'},{'A5'}});");
    u8 ret=luaL_dostring(L,lua_string);
    ret=ret;
    rt_kprintf("run lua result %s", lua_tostring(L, 1));
    u8 j=0,i=0;
    for(j=0;j<row_len;j++){
        rt_kprintf("row=%2d,port=%d,pin=%d",j,keyboard_gpio_rows[j].port, keyboard_gpio_rows[j].pin);
    }
    for(i=0;i<col_len;i++){
        rt_kprintf("col=%2d,port=%d,pin=%d",i,keyboard_gpio_cols[i].port, keyboard_gpio_cols[i].pin);
    }
    keyboard_io_init();

    while(1){
        scan();
        rt_thread_delay(10);
    }

    lua_close(L);


}
