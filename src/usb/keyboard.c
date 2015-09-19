#include "keyboard.h"
#include "stdlib.h"
void put_num(d){
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
GPIO_TypeDef* char2port(ch){
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
GPIO_TypeDef* str2pin(str){
    return atoi(str);
}
static void c_set_key_pos(lua_State *L){
    rt_kprintf("key = %s,%d,%s\n", lua_tostring(L, 1),lua_tointeger(L, 2),lua_tostring(L, 3));
    struct GPIO_struct * arr;
    if(lua_tostring(L, 1)[0]=='c'){
        arr=keyboard_gpio_cols;
    }else{
        arr=keyboard_gpio_rows;
    }
    arr[lua_tointeger(L, 2)-1].port=char2port(lua_tostring(L, 3)[0]);
    arr[lua_tointeger(L, 2)-1].pin=str2pin(lua_tostring(L, 3)+1);

}

void rt_lua_thread_entry(void* parameter){

    lua_State *L = luaL_newstate();
    //    lua_register(L, "key_pos_init", key_pos_init);
    lua_register(L, "c_set_key_pos", c_set_key_pos);
    //    luaL_dostring(L, "key_pos_init({{'A3'},{'A5'}});");
    luaL_dostring(L,lua_string);
    rt_kprintf("run lua result %s", lua_tostring(L, 1));
    rt_kprintf("done\n");

    lua_close(L);

    return 0;
}
