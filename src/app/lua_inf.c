#include "app.h"
#include "string.h"
#include "lua_inf.h"
#include "keyboard.h"
#include "usb_process.h"
#include "lua.h"
#include "lapi.h"
#include "lauxlib.h"
#include "lstate.h"
#include <lualib.h>
#include <string.h>

static lua_State *current_Lua;
u8 use_lua = false;

static int restart_keyboard(lua_State *L)
{
    printf("resetting\r\n");
    SCB->AIRCR = 0x05FA0000 | (u32)0x04;
    return 0;
}
static void handle_lua_err(const char *title)
{
    lua_type(current_Lua, -1);
    const char *err = lua_tostring(current_Lua, -1);
    printf("Lua Error(%s):\n%s\n", title, err);
}

u8 lua_handle(key_t *buf)
{
    int ret;
    lua_getglobal(current_Lua, "key_input");
    lua_pushnumber(current_Lua, buf->control);
    lua_pushnumber(current_Lua, buf->cnt);
    for (int i = 0; i < buf->cnt; i++)
    {
        lua_pushnumber(current_Lua, (buf->key[i].pos[0] << 4) + buf->key[i].pos[1]);
    }
    int result = lua_pcall(current_Lua, buf->cnt + 2, 1, 0);

    if (result == 0)
    {
        ret = lua_toboolean(current_Lua, -1);
        lua_pop(current_Lua, 1);
    }
    else
    {
        ret = 0;
        handle_lua_err("key_input");
    }

    return ret > 0;
}
#define GENERAL_KEY_CNT 35
extern const u8 general_key_value[GENERAL_KEY_CNT];
extern const char *general_key_map[GENERAL_KEY_CNT];
//plus 128 to have shift
const u8 general_key_value[] = {0,
                                40, 41, 42, 43,
                                44, 57, 58, 59,
                                60, 61, 62, 63,
                                64, 65, 66, 67,

                                68, 69, 70, 71,
                                72, 73, 74, 75,
                                76, 77, 78, 79,
                                80, 81, 82, 83,
                                47 + 128, 48 + 128};
const char *general_key_map[] = {"",
                                 "enter", "esc", "delete", "tab",
                                 "space", "capsLock", "f1", "f2",
                                 "f3", "f4", "f5", "f6",
                                 "f7", "f8", "f9", "f10",

                                 "f11", "f12", "printscreen", "scrollLock",
                                 "pause", "insert", "home", "pageup",
                                 "deleteforward", "end", "pagedown", "right",
                                 "left", "down", "up", "numLock",
                                 "leftbracket", "rightbracket"};
const u8 shift_table[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0};
static int get_shift(lua_State *L)
{
    int index = lua_tointeger(L, 1);
    lua_pushnumber(L, shift_table[index]);
    return 1;
}
static int get_key_index(lua_State *L)
{
    const char *str = lua_tostring(L, 1);
    u8 cnt = GENERAL_KEY_CNT;
    for (int i = 1; i < cnt; i++)
    {
        if (strcmp((const char *)general_key_map[i], str) == 0)
        {
            lua_pushnumber(L, general_key_value[i]);
            return 1;
        }
    }
    lua_pushnumber(L, 0);
    return 1;
}

static int lua_delay_ms(lua_State *L)
{
    int ms = lua_tointeger(L, 1);
    for (int i = 0; i < ms / 1000; i++)
        delay_ms(1000);
    delay_ms(ms % 1000);
    return 0;
}
extern void mouse_process(u8 *buf);
static int lua_mouse_output(lua_State *L)
{
    u8 buf[5];
    for (u8 i = 0; i < 5; i++)
    {
        buf[i] = lua_tointeger(L, i + 1);
    }
    mouse_process(buf);
    return 0;
}
static int lua_output(lua_State *L)
{
    u8 buf[8];
    for (u8 i = 0; i < 8; i++)
    {
        buf[i] = lua_tointeger(L, i + 1);
        // printf("%d,",buf[i]);
    }
    // printf("\n");
    keyboard_process(buf);
    return 0;
}
static void str_trim(char *pStr)
{
    char *pTmp = pStr;

    while (*pStr != '\0')
    {
        if (*pStr != ' ' && *pStr != '\n' && *pStr != '\r')
        {
            *pTmp++ = *pStr;
        }
        ++pStr;
    }
    *pTmp = '\0';
}
static int read_file(lua_State *L)
{
    FIL file;
    int n = lua_gettop(L);
    const char *file_name = lua_tostring(L, 1);
    u8 trim = 0;
    if (n == 2)
        trim = lua_tointeger(L, 2);
    if (!f_open(&file, file_name, FA_OPEN_EXISTING | FA_WRITE | FA_READ | FA__WRITTEN))
    {
        char *read_buf = (char *)malloc(file.fsize + 1);
        u32 cnt = 0;
        f_read(&file, read_buf, file.fsize, &cnt);
        f_close(&file);
        if (trim)
        {
            str_trim(read_buf);
        }
        lua_pushlstring(L, read_buf, cnt);
        free(read_buf);
        return 1;
    }
    return 0;
}
extern u8 led_buf[14][5];
extern u8 led_mode;
static int led_set_bit(lua_State *L)
{
    u8 x = 13 - (u8)lua_tointeger(L, 1);
    u8 y = lua_tointeger(L, 2);
    u8 v = lua_tointeger(L, 3);
    led_buf[x][y] = !v;
    return 0;
}
static int led_clear(lua_State *L)
{
    for (u8 i = 0; i < 14; i++)
    {
        for (u8 j = 0; j < 5; j++)
        {
            led_buf[i][j] = 1;
        }
    }
    return 0;
}
static int led_set_mode(lua_State *L)
{
    led_mode = (u8)lua_tointeger(L, 1);
    return 0;
}
static int led_fill(lua_State *L)
{
    for (u8 i = 0; i < 14; i++)
    {
        for (u8 j = 0; j < 5; j++)
        {
            led_buf[i][j] = 0;
        }
    }
    return 0;
}
static u8 *handle_datasheet(char *buf, u16 size, u8 width)
{
    u16 cnt = size / (width + 1);
    u8 *data_base = (u8 *)malloc(cnt);
    for (int i = 0; i < cnt; i++)
    {
        *(data_base + i) = (u8)atoi((buf + i * (width + 1)));
    }
    return data_base;
}
static int read_datasheet(lua_State *L)
{
    u8 *arr = (u8 *)lua_tointeger(L, 1);
    int index = lua_tointeger(L, 2);
    lua_pushinteger(L, arr[index]);
    return 1;
}
u8 *key_index_lua_buf;
static int init_datasheet(lua_State *L)
{
    FIL file;

    const char *file_name = lua_tostring(L, 1);
    u8 width = (u8)lua_tointeger(L, 2);
    int mode = (u8)lua_tointeger(L, 3);           // optional
    int input_num_cnt = (u16)lua_tointeger(L, 4); // optional
    if (!mode)
    {
        if (!f_open(&file, file_name, FA_OPEN_EXISTING | FA_WRITE | FA_READ | FA__WRITTEN))
        {
            char *read_buf = (char *)malloc((u16)(file.fsize + 1));
            u32 cnt = 0;
            f_read(&file, read_buf, file.fsize, &cnt);
            f_close(&file);
            str_trim(read_buf);
            u8 *buf_p = handle_datasheet(read_buf, (u16)cnt, width);
            if (!strcmp(file_name, "config/key_index.txt"))
            {
                key_index_lua_buf = (u8 *)buf_p;
            }
            lua_pushinteger(L, (int)buf_p);
            free(read_buf);
            return 1;
        }
    }
    else
    {
        u8 *buf_p = handle_datasheet((char *)file_name, (u16)input_num_cnt, width);
        lua_pushinteger(L, (int)buf_p);
        return 1;
    }

    return 0;
}

static u8 lua_invoke_main()
{
    lua_getglobal(current_Lua, "main");

    int result = lua_pcall(current_Lua, 0, 0, 0);

    if (result == 0)
    {
        return 0;
    }
    else
    {
        handle_lua_err("main");
        return 1;
    }
}

static int traceback(lua_State *L)
{
    printf("traceback!!\n");
    lua_getglobal(L, "debug");
    lua_getfield(L, -1, "traceback");
    lua_pushvalue(L, 1);
    lua_pushinteger(L, 2);
    lua_call(L, 2, 1);
    printf("lua error:%s\n", lua_tostring(L, -1));
    return 1;
}

extern void print_free_memory(const char *);
void lua_init()
{
    if (current_Lua)
        lua_close(current_Lua);
    current_Lua = (lua_State *)luaL_newstate();
    lua_State *L = current_Lua;
    luaL_openlibs(L);
    lua_pushcfunction(L, traceback);
    print_free_memory("before register c functions1");
    lua_register(L, "output", lua_output);
    lua_register(L, "mouse_output", lua_mouse_output);
    lua_register(L, "delay", lua_delay_ms);
    lua_register(L, "get_shift", get_shift);
    lua_register(L, "get_key_index", get_key_index);
    lua_register(L, "read_file", read_file);
    lua_register(L, "restart_keyboard", restart_keyboard);
    lua_register(L, "led_set_bit", led_set_bit);
    lua_register(L, "led_clear", led_clear);
    lua_register(L, "led_set_mode", led_set_mode);
    lua_register(L, "led_fill", led_fill);
    lua_register(L, "read_datasheet", read_datasheet);
    lua_register(L, "init_datasheet", init_datasheet);
    lua_pop(L, 1); // remove _PRELOAD table

    print_free_memory("before init");
    int result = luaL_dofile(L, "main.lua");
    print_free_memory("after init");
    if (result != 0)
    {
        handle_lua_err("init");
    }
    lua_invoke_main();
    print_free_memory("after main");
    use_lua = true;
}
