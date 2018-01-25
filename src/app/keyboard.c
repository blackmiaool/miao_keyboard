#include "keyboard.h"
#include <stm32f10x_lib.h>
#include "stm32lib.h"
#include "hw_config.h"
#include "sys.h"
#include "stm32f10x_gpio.h"
#include "usart.h"
#include "lua_inf.h"

typedef u16 uint16_t;

struct GPIO_struct keyboard_gpio_rows[ROW_LEN];
struct GPIO_struct keyboard_gpio_cols[COL_LEN];

GPIO_TypeDef *char2port(char ch);
u32 str2pin(char *str);
u8 clean_mode = 0; //disable app programs
u8 udisk_mode = 0; //mount as usb disk
u8 clean_key = 41; //press esc to enter clean mode
u8 udisk_key = 30; //press esc to enter clean mode
u8 fn1 = 135;
u8 fn2 = 136;

const u8 key_map[3][ROW_LEN][COL_LEN] = {{
											 {41, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 45, 46, 42},
											 {43, 20, 26, 8, 21, 23, 28, 24, 12, 18, 19, 47, 48, 49},
											 {57, 4, 22, 7, 9, 10, 11, 13, 14, 15, 51, 52, 40, 40},
											 {225, 225, 29, 27, 6, 25, 5, 17, 16, 54, 55, 56, 0, 229},
											 {224, 227, 82, 226, 44, 44, 44, 44, 228, 80, 79, 80, 135, 136},
										 },

										 {
											 {41, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 42}, //active when pressing fn1
											 {43, 20, 26, 8, 21, 23, 28, 24, 12, 18, 19, 47, 48, 49},
											 {57, 4, 22, 7, 9, 10, 11, 13, 14, 15, 51, 52, 40, 40},
											 {225, 225, 29, 27, 6, 25, 5, 17, 16, 54, 55, 56, 0, 229},
											 {224, 227, 82, 226, 44, 44, 44, 44, 228, 80, 79, 80, 0, 0},
										 },

										 {
											 {41, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 45, 46, 42}, //active when pressed fn2
											 {43, 20, 26, 8, 21, 23, 28, 24, 82, 18, 19, 47, 48, 49},
											 {57, 4, 22 /*s*/, 7, 9, 10 /*g*/, 11, 80 /*j*/, 81, 79, 51 /*;*/, 52, 40, 40},
											 {225, 225, 29, 27, 6, 25, 5, 17, 16, 54, 55, 56, 0, 229},
											 {224, 227, 82, 226, 44, 44, 44, 44, 228, 80, 79, 80, 0, 0},
										 }};

//const u8 key_map[3][ROW_LEN][COL_LEN]={{{41 ,30 ,31 ,32 ,33 ,34 ,35 ,36 ,37 ,38 ,39 ,45 ,46 ,42},
//{43 ,20 ,26 ,8  ,21 ,23 ,28 ,24 ,12 ,18 ,19 ,47 ,48 ,49},
//{224,4  ,22 ,7  ,9  ,10 ,11 ,13 ,14 ,15 ,51 ,52 ,40 ,40},
//{225,225,29 ,27 ,6  ,25 ,5  ,17 ,16 ,54 ,55 ,56 ,0  ,229},
//{135,226,227 ,44 ,44,44 ,44  ,44 ,44,44 ,231,230,228,136},},

//{{41 ,58 ,59 ,60 ,61 ,62 ,63 ,64 ,65 ,66 ,67 ,68 ,69 ,42},//active when pressing fn1
//{43 ,20 ,26 ,8  ,21 ,23 ,28 ,24 ,12 ,18 ,19 ,47 ,48 ,49},
//{224 ,4  ,22 ,7  ,9  ,10 ,11 ,13 ,14 ,15 ,51 ,52 ,40 ,40},
//{225,225,29 ,27 ,6  ,25 ,5  ,17 ,16 ,54 ,55 ,56 ,0  ,229},
//{0,226,227 ,44 ,44,44 ,44  ,44 ,44,44 ,231,230,228,0},},

//{{41 ,30 ,31 ,32 ,33 ,34 ,35 ,36 ,37 ,38 ,39 ,45 ,46 ,42},//active when pressed fn2
//{43 ,20 ,26 ,8  ,21 ,23 ,28 ,24 ,82 ,18 ,19 ,47 ,48 ,49},
//{224 ,4  ,22/*s*/ ,7  ,9  ,10 /*g*/,11 ,80/*j*/ ,81 ,79 ,51/*;*/ ,52 ,40 ,40},
//{225,225,29 ,27 ,6  ,25 ,5  ,17 ,16 ,54 ,55 ,56 ,0  ,229},
//{0,226,227 ,44 ,44,44 ,44  ,44 ,44,44 ,231,230,228,0},}};

//const u8 key_index[ROW_LEN][14]={{41 ,30 ,31 ,32 ,33 ,34 ,35 ,36 ,37 ,38 ,39 ,45 ,46 ,42},//long space
//{43 ,20 ,26 ,8  ,21 ,23 ,28 ,24 ,12 ,18 ,19 ,47 ,48 ,49},
//{57 ,4  ,22 ,7  ,9  ,10 ,11 ,13 ,14 ,15 ,51 ,52 j,40 ,40},
//{225,225,29 ,27 ,6  ,25 ,5  ,17 ,16 ,54 ,55 ,56 ,0  ,229},
//{224,227,226 ,44 ,44,44 ,44  ,44  ,44,44,228,230,80,79},};

extern void keyboard_process(u8 *buf);
#define COLS_IO_CONFIG "C0C1C2C3C4C5C6C7C8C9C10C11C12C13"
#define ROWS_IO_CONFIG "B5B6B7B8B9"

/**
 * get key code from key map
 */
static u8 get_key(u8 index, u8 row, u8 col)
{
	if (key_index_lua_buf)
	{
		return key_index_lua_buf[index * ROW_LEN * 16 + row * 16 + col];
	}
	else
	{
		return key_map[index][row][col];
	}
}
static void parse_fill_io_config(struct GPIO_struct *target, const char *config, u8 config_len)
{
	u8 io_index = 0;
	u8 char_index = 0;
	while (char_index < config_len - 1)
	{
		struct GPIO_struct *io = &target[io_index];
		io->port = char2port(config[char_index]);
		//		printf(" p:%c",cols[i]);
		char_index++;
		long num = atoi((char *)config + char_index);
		io->pin = 1 << (num % 8 * 4);
		io->num = (u8)num;
		if (io->num > 9)
		{
			char_index++;
		}
		char_index++;
		io_index++;
	}
}
void keyboard_init(void)
{
	SCPE(PERIOA);
	SCPE(PERIOB);
	SCPE(PERIOC);
	SCPE(PERIOD);

	parse_fill_io_config(keyboard_gpio_cols, COLS_IO_CONFIG, sizeof(COLS_IO_CONFIG));
	parse_fill_io_config(keyboard_gpio_rows, ROWS_IO_CONFIG, sizeof(ROWS_IO_CONFIG));

	u8 i = 0;

	for (i = 0; i < COL_LEN; i++)
	{
		IOConfig(keyboard_gpio_cols[i].port, keyboard_gpio_cols[i].num > 7, keyboard_gpio_cols[i].pin, 8);
		IOout(keyboard_gpio_cols[i].port, keyboard_gpio_cols[i].num, 1);
		GPIOC->ODR = 0XFFFF;
	}

	for (i = 0; i < ROW_LEN; i++)
	{
		IOConfig(keyboard_gpio_rows[i].port, keyboard_gpio_rows[i].num > 7, keyboard_gpio_rows[i].pin, 3);
		IOout(keyboard_gpio_rows[i].port, keyboard_gpio_rows[i].num, 1);
	}
}
GPIO_TypeDef *char2port(char ch)
{
	switch (ch)
	{
	case 'A':
		return (GPIO_TypeDef *)GPIOA;
	case 'B':
		return (GPIO_TypeDef *)GPIOB;
	case 'C':
		return (GPIO_TypeDef *)GPIOC;
	case 'D':
		return (GPIO_TypeDef *)GPIOD;
	case 'E':
		return (GPIO_TypeDef *)GPIOE;
	case 'F':
		return (GPIO_TypeDef *)GPIOF;
	case 'G':
		return (GPIO_TypeDef *)GPIOG;
	default:
		printf("wrong port!!!\n");
		return (GPIO_TypeDef *)GPIOA;
	}
}
u32 str2pin(char *str)
{
	return 1 << atoi(str);
}

void keyboard_send_wrap(key_t key_buf);

u16 key_val[5];
u8 first_scan = 1;

static u16 matrix_debouncing[ROW_LEN];
#define DEBOUNCE 5
static u8 debouncing = DEBOUNCE;
static u8 hardware_scan(u16 *map)
{
	for (u8 i = 0; i < ROW_LEN; i++)
	{
		IOout(keyboard_gpio_rows[i].port, keyboard_gpio_rows[i].num, 0);
		delay_us(30);
		u16 cols = 0;
		for (u8 j = 0; j < COL_LEN; j++)
		{
			u8 val = IOin(keyboard_gpio_cols[j].port, keyboard_gpio_cols[j].num);
			cols += ((val > 0) << j);
		}
		u16 mask = 1 << COL_LEN;
		mask--;
		cols = mask - cols;

		if (matrix_debouncing[i] != cols)
		{
			matrix_debouncing[i] = cols;
			debouncing = DEBOUNCE;
		}
		IOout(keyboard_gpio_rows[i].port, keyboard_gpio_rows[i].num, 1);
	}

	if (debouncing)
	{
		if (--debouncing)
		{
			delay_ms(1);
			return 0;
		}
		else
		{
			for (u8 i = 0; i < ROW_LEN; i++)
			{
				map[i] = matrix_debouncing[i];
			}
			return 1;
		}
	}
	return 0;
}
static key_t keys_pre = {0, 0};
static void key_t_add(key_t *keys, u8 p0, u8 p1)
{
	keys->key[keys->cnt].pos[0] = p0;
	keys->key[keys->cnt].pos[1] = p1;
	keys->cnt++;
}

u8 has_key(key_t *keys, single_key_t key)
{
	for (u8 j = 0; j < keys->cnt; j++)
	{
		if (key.pos[0] == keys->key[j].pos[0] && key.pos[1] == keys->key[j].pos[1])
		{
			return 1;
		}
	}
	return 0;
}
void keyboard_scan()
{
	if (!hardware_scan(key_val))
	{
		return;
	}

	u8 changes = false;

	key_t keys_pressing = {0, 0};
	key_t keys_send = {0, 0};
	key_t keys_next = {0, 0};

	for (u8 j = 0; j < ROW_LEN; j++)
	{
		for (u8 i = 0; i < COL_LEN; i++)
		{
			if (key_val[j] & (1 << i))
			{
				u8 value = get_key(0, j, i);
				key_t_add(&keys_pressing, j, i);

				//                printf("key press %d  %d %d\n",j,i, key_index[j][i]);
				if (keys_pressing.cnt == KEY_BUF_LEN)
				{
					goto scan_end;
				}
			}
		}
	}
scan_end:;
	for (u8 i = 0; i < keys_pressing.cnt; i++)
	{ //check new pressed
		single_key_t key = keys_pressing.key[i];

		if (!has_key(&keys_pre, key))
		{

			changes = 1;
			key_t_add(&keys_next, key.pos[0], key.pos[1]);
		}
	}

	for (u8 i = 0; i < keys_pre.cnt; i++)
	{ //check new pressed
		single_key_t key = keys_pre.key[i];

		if (!has_key(&keys_pressing, key))
		{

			changes = 1;
		}
		else
		{
			key_t_add(&keys_next, key.pos[0], key.pos[1]);
		}
	}
	keys_pre = keys_next;

	u8 modify_key = 0;
	for (u8 i = 0; i < keys_next.cnt; i++)
	{ //check new pressed
		single_key_t key = keys_next.key[i];
		u8 value = get_key(0, key.pos[0], key.pos[1]);
		switch (value)
		{
		case 225:
			modify_key |= (1 << 1);
			break;
		case 224:
			modify_key |= (1 << 0);
			break;
		case 227:
			modify_key |= (1 << 3);
			break;
		case 226:
			modify_key |= (1 << 2);
			break;
		case 228:
			modify_key |= (1 << 4);
			break;
		case 229:
			modify_key |= (1 << 5);
			break;
		case 230:
			modify_key |= (1 << 6);
			break;
		case 231:
			modify_key |= (1 << 7);
			break;
		default:
			key_t_add(&keys_send, key.pos[0], key.pos[1]);
		}
	}
	keys_send.control = modify_key;

	if (changes)
	{
		keyboard_send_wrap(keys_send);
	}
}

extern void app_handle(u8 *buf, key_t *);
void keyboard_send_wrap2(u8 *buf);

key_t commu_buf_pre;
//typedef const u8 Key_map[ROW_LEN][COL_LEN];
//typedef Key_map *key_map_t;
u8 current_mode = 0;
void key_set(u8 *buf, u8 index, const key_t *key)
{
	for (u8 i = 0; i < key->cnt; i++)
	{
		buf[2 + i] = get_key(index, key->key[i].pos[0], key->key[i].pos[1]);
	}
}

static u8 check_key(key_t key_buf,u8 target){
	for (u8 i = 0; i < key_buf.cnt; i++)
	{
		u8 key_this = get_key(0, key_buf.key[i].pos[0], key_buf.key[i].pos[1]);
		if (key_this == target)
		{
			return 1;
		}
	}
	return 0;
}

void keyboard_send_wrap(key_t key_buf)
{
	u8 buf[8];
	buf[0] = key_buf.control;
	buf[1] = 0;
	for (u8 i = 2; i < 8; i++)
	{
		buf[i] = 0;
	}

	u8 changed = 0;						  //if key state change
	if (commu_buf_pre.cnt != key_buf.cnt) //if key cnt changes
		changed = 1;
	if (commu_buf_pre.control != key_buf.control) //if control key changes
		changed = 1;
	if (current_mode != 2)
	{
		current_mode = 0;
	}

	if (first_scan)
	{ //just check once when power on. If clean_mode, disable all app programs.
		first_scan = 0;
		clean_mode = check_key(key_buf,clean_key);
		udisk_mode = check_key(key_buf,udisk_key);
		printf("udisk%d",udisk_mode);
	}

	if (clean_mode)
	{
		for (u8 i = 0; i < key_buf.cnt; i++)
		{ //if enter mode 1
			u8 key_this = get_key(0, key_buf.key[i].pos[0], key_buf.key[i].pos[1]);
			if (key_this == fn1)
			{
				current_mode = 1;
				break;
			}
		}
	}

	for (u8 i = 0; i < key_buf.cnt; i++)
	{ //if key changes
		if (commu_buf_pre.key[i].pos[0] != key_buf.key[i].pos[0] || commu_buf_pre.key[i].pos[1] != key_buf.key[i].pos[1])
		{
			changed = 1;
			commu_buf_pre.key[i].pos[0] = key_buf.key[i].pos[0];
			commu_buf_pre.key[i].pos[1] = key_buf.key[i].pos[1];
		}
	}
	commu_buf_pre.cnt = key_buf.cnt;
	commu_buf_pre.control = key_buf.control;
	if (changed)
	{
		if (clean_mode)
		{
			for (u8 i = 2; i < 2 + key_buf.cnt; i++)
			{
				u8 key_this = get_key(0, key_buf.key[i - 2].pos[0], key_buf.key[i - 2].pos[1]);
				if (key_this == fn2)
				{
					if (!current_mode)
						current_mode = 2;
					else
						current_mode = 0;
					break;
				}
			}
		}

		key_set(buf, current_mode, &key_buf);
		app_handle(buf, &key_buf);
	}
}
