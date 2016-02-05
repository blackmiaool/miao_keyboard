#include "keyboard.h"
#include <stm32f10x_lib.h>
#include "stm32lib.h"
#include "hw_config.h"
#include "sys.h"
#include "stm32f10x_gpio.h"
#include "usart.h"
#include "commu_mcu.h"
#include "lua_inf.h"
typedef u16 uint16_t;



struct GPIO_struct keyboard_gpio_rows[ROW_LEN];
struct GPIO_struct keyboard_gpio_cols[COL_LEN];

GPIO_TypeDef* char2port(char ch);
u32 str2pin(char *str);
u8 clean_mode=0;
u8 clean_key=41;
u8 fn1=135;
u8 fn2=136;
const u8 key_map[3][ROW_LEN][COL_LEN]={{{41 ,30 ,31 ,32 ,33 ,34 ,35 ,36 ,37 ,38 ,39 ,45 ,46 ,42},
{43 ,20 ,26 ,8  ,21 ,23 ,28 ,24 ,12 ,18 ,19 ,47 ,48 ,49},
{57 ,4  ,22 ,7  ,9  ,10 ,11 ,13 ,14 ,15 ,51 ,52 ,40 ,40},
{225,225,29 ,27 ,6  ,25 ,5  ,17 ,16 ,54 ,55 ,56 ,0  ,229},
{224,227,82 ,226 ,44,44 ,44  ,44  ,231,80,79,80,135,136},},

{{41 ,58 ,59 ,60 ,61 ,62 ,63 ,64 ,65 ,66 ,67 ,68 ,69 ,42},//active when pressing fn1
{43 ,20 ,26 ,8  ,21 ,23 ,28 ,24 ,12 ,18 ,19 ,47 ,48 ,49},
{57 ,4  ,22 ,7  ,9  ,10 ,11 ,13 ,14 ,15 ,51 ,52 ,40 ,40},
{225,225,29 ,27 ,6  ,25 ,5  ,17 ,16 ,54 ,55 ,56 ,0  ,229},
{224,227,82 ,226 ,44,44 ,44  ,44  ,231,80,79,80,0,0},},

{{41 ,30 ,31 ,32 ,33 ,34 ,35 ,36 ,37 ,38 ,39 ,45 ,46 ,42},//active when pressed fn2 
{43 ,20 ,26 ,8  ,21 ,23 ,28 ,24 ,82 ,18 ,19 ,47 ,48 ,49},
{57 ,4  ,22/*s*/ ,7  ,9  ,10 /*g*/,11 ,80/*j*/ ,81 ,79 ,51/*;*/ ,52 ,40 ,40},
{225,225,29 ,27 ,6  ,25 ,5  ,17 ,16 ,54 ,55 ,56 ,0  ,229},
{224,227,82 ,226 ,44,44 ,44  ,44  ,231,80,79,80,0,0},}};



//const u8 key_map[3][ROW_LEN][COL_LEN]={{{41 ,30 ,31 ,32 ,33 ,34 ,35 ,36 ,37 ,38 ,39 ,45 ,46 ,42},
//{43 ,20 ,26 ,8  ,21 ,23 ,28 ,24 ,12 ,18 ,19 ,47 ,48 ,49},
//{57 ,4  ,22 ,7  ,9  ,10 ,11 ,13 ,14 ,15 ,51 ,52 ,40 ,40},
//{225,225,29 ,27 ,6  ,25 ,5  ,17 ,16 ,54 ,55 ,56 ,0  ,229},
//{224,227,226 ,44 ,44,44 ,44  ,44 ,44,44 ,228,230,135,136},},

//{{41 ,58 ,59 ,60 ,61 ,62 ,63 ,64 ,65 ,66 ,67 ,68 ,69 ,42},//active when pressing fn1
//{43 ,20 ,26 ,8  ,21 ,23 ,28 ,24 ,12 ,18 ,19 ,47 ,48 ,49},
//{57 ,4  ,22 ,7  ,9  ,10 ,11 ,13 ,14 ,15 ,51 ,52 ,40 ,40},
//{225,225,29 ,27 ,6  ,25 ,5  ,17 ,16 ,54 ,55 ,56 ,0  ,229},
//{224,227,226 ,44 ,44,44 ,44  ,44 ,44,44 ,228,230,0,0},},

//{{41 ,30 ,31 ,32 ,33 ,34 ,35 ,36 ,37 ,38 ,39 ,45 ,46 ,42},//active when pressed fn2 
//{43 ,20 ,26 ,8  ,21 ,23 ,28 ,24 ,82 ,18 ,19 ,47 ,48 ,49},
//{57 ,4  ,22/*s*/ ,7  ,9  ,10 /*g*/,11 ,80/*j*/ ,81 ,79 ,51/*;*/ ,52 ,40 ,40},
//{225,225,29 ,27 ,6  ,25 ,5  ,17 ,16 ,54 ,55 ,56 ,0  ,229},
//{224,227,226 ,44 ,44,44 ,44  ,44 ,44,44 ,228,230,0,0},}};




//const u8 key_index[ROW_LEN][14]={{41 ,30 ,31 ,32 ,33 ,34 ,35 ,36 ,37 ,38 ,39 ,45 ,46 ,42},//long space
//{43 ,20 ,26 ,8  ,21 ,23 ,28 ,24 ,12 ,18 ,19 ,47 ,48 ,49},
//{57 ,4  ,22 ,7  ,9  ,10 ,11 ,13 ,14 ,15 ,51 ,52 ,40 ,40},
//{225,225,29 ,27 ,6  ,25 ,5  ,17 ,16 ,54 ,55 ,56 ,0  ,229},
//{224,227,226 ,44 ,44,44 ,44  ,44  ,44,44,228,230,80,79},};
typedef struct{
	u8 pos[2];
} single_key_t;
typedef struct {
	u8 control;
	single_key_t key[6];                      /*!< Key used with keyboard. This can be whatever. Like numbers, letters, everything. */
	u8 key_cnt;
} key_t;
extern void keyborad_process(u8* buf);
const char cols[]="C0C1C2C3C4C5C6C7C8C9C10C11C12C13";
const char rows[]="B5B6B7B8B9";

void keyboard_init(void){
	SCPE(PERIOA);
	SCPE(PERIOB);
	SCPE(PERIOC);
	SCPE(PERIOD);
	u8 i=0;
	u8 cnt=0;
	while(i<sizeof(cols)-1){
		struct GPIO_struct* io=&keyboard_gpio_cols[cnt];		
		io->port=char2port(cols[i]);
//		printf(" p:%c",cols[i]);
		i++;
		long num=atoi((char *)cols+i);
		io->pin=1<<(num%8*4);
		io->num=num;
		if(io->num>9)
		{
			i++;
		}
		i++;
		delay_ms(10);
//		printf(" n:%ld %ld\r\n",io->pin,io->num);
		delay_ms(10);
		cnt++;
	}
	cnt=0;
	i=0;
	while(i<sizeof(rows)-1){
		struct GPIO_struct* io=&keyboard_gpio_rows[cnt];		
		io->port=char2port(rows[i]);
//		printf(" p:%c",rows[i]);
		i++;
		long num=atoi((char *)(rows+i));
		io->pin=1<<(num%8*4);
		io->num=num;
		if(io->num>9)
		{
			i++;
		}
		i++;
//		delay_ms(10);
//		printf(" n:%d %d\r\n",io->pin,io->num);
//		delay_ms(10);
		cnt++;
	}
	for(i=0;i<COL_LEN;i++){
		IOConfig(keyboard_gpio_cols[i].port, keyboard_gpio_cols[i].num>7, keyboard_gpio_cols[i].pin, 8);
		IOout(keyboard_gpio_cols[i].port,keyboard_gpio_cols[i].num,1);
		GPIOC->ODR=0XFFFF;
	}
//	printf("gpioc l %d h %d o %d i %d",GPIOC->CRL,GPIOC->CRH,GPIOC->ODR,GPIOC->IDR);
	for(i=0;i<ROW_LEN;i++){
		IOConfig(keyboard_gpio_rows[i].port, keyboard_gpio_rows[i].num>7, keyboard_gpio_rows[i].pin, 3);
		IOout(keyboard_gpio_rows[i].port,keyboard_gpio_rows[i].num,1);
	}
//	printf("gpioc l %d h %d o %d i %d",GPIOB->CRL,GPIOB->CRH,GPIOB->ODR,GPIOB->IDR);
//	while(1){
//		keyboard_scan();
//		delay_ms(10);
//	}
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
    default:
        printf("wrong port!!!\n");
        return (GPIO_TypeDef*)GPIOA;

    }
}
u32 str2pin(char *str){
    return 1<<atoi(str);
}


void keyboard_send_wrap(key_t key_buf);
//static u8 key_index[5][14];
u16 key_val[5];
u8 start_check=1;
void keyboard_scan(){
    u8 i=0,j=0;
    for(j=0;j<ROW_LEN;j++){
        key_val[j]=0;
        IOout(keyboard_gpio_rows[j].port,keyboard_gpio_rows[j].num,1);
    }

    for(j=0;j<ROW_LEN;j++){
        IOout(keyboard_gpio_rows[j].port,keyboard_gpio_rows[j].num,0);
        for(i=0;i<COL_LEN;i++){
			u8 val=IOin(keyboard_gpio_cols[i].port,keyboard_gpio_cols[i].num);
            key_val[j]+=((val>0)<<i);
        }
        u16 mask=1<<COL_LEN;
        mask--;
        key_val[j]=mask-key_val[j];
        IOout(keyboard_gpio_rows[j].port,keyboard_gpio_rows[j].num,1);
    }

    for(j=0;j<ROW_LEN;j++){
//        printf("key value %d %3d",j,key_val[j]);
    }
//		delay_ms(200);


    u8 key_data_index=0;
    static u8 pre_press=0;
    u8 key_press=0;
	key_t key_buf;
	key_buf.control=0;
	key_buf.key_cnt=0;
    for(j=0;j<ROW_LEN;j++){
        for(i=0;i<COL_LEN;i++){
            if(key_val[j]&(1<<i))
            {
                switch(key_map[0][j][i]){
                case 225:
                    key_buf.control|=(1<<1);
                    break;
                case 224:
                    key_buf.control|=(1<<0);
                    break;
                case 227:
                    key_buf.control|=(1<<3);
                    break;
                case 226:
                    key_buf.control|=(1<<2);
                    break;
                case 228:
                    key_buf.control|=(1<<4);
                    break;
                case 229:
                    key_buf.control|=(1<<5);
                    break;
                case 230:
                    key_buf.control|=(1<<6);
                    break;
                case 231:
                    key_buf.control|=(1<<7);
                    break;
                default:
                    key_buf.key[key_data_index].pos[0]=j;
					key_buf.key[key_data_index++].pos[1]=i;
                }
				key_buf.key_cnt=key_data_index;
//                printf("key press %d  %d %d\n",j,i, key_index[j][i]);
                pre_press=1;
                key_press=1;
                if(key_data_index==6){
                   goto end;
                }
            }
        }
    }

    if(key_press){
        keyboard_send_wrap(key_buf);
    }
    else if(pre_press){
        pre_press=0;
        keyboard_send_wrap(key_buf);
    }
end:;
}


extern u8 delegate;
void app_press(u8 *buf){
	
//	u8 b[9];
//	b[0]=1;
//	
//	for(u8 i=0;i<8;i++){
//		b[i+1]=buf[i];
//	}
	keyborad_process(buf);
}
void app_handle(u8 *buf);
void keyboard_send_wrap2(u8 *buf);
void keyboard_send(u8 *buf){
	if(delegate){
		commu_send(buf,8,COMMU_TYPE(KEYBOARD_MS));
	}else{
		
		app_handle(buf);
	}
}

void find_pos(u8 **value,u8 index,u8* pos){
	
}

key_t commu_buf_pre;
typedef const u8 Key_map[ROW_LEN][COL_LEN];
typedef Key_map *key_map_t;
u8 current_mode=0;
void key_set(u8 *buf,key_map_t map,const key_t *key){
	for(u8 i=0;i<key->key_cnt;i++){
		buf[2+i]=(*map)[key->key[i].pos[0]][key->key[i].pos[1]];
	}
}
void keyboard_send_wrap(key_t key_buf){
	u8 buf[8];
	buf[0]=key_buf.control;
	buf[1]=0;
	for(u8 i=2;i<8;i++)
		buf[i]=0;
//	for(u8 i=0;i<6;i++){
//		buf[2+i]=key_buf.key[i];		
//	}
	u8 send=0;
	if(commu_buf_pre.key_cnt!=key_buf.key_cnt)
		send=1;
	if(commu_buf_pre.control!=key_buf.control)
		send=1;
	if(current_mode!=2){
			current_mode=0; 
	}
	if(start_check){
		start_check=0;
		for(u8 i=0;i<key_buf.key_cnt;i++){
			u8 key_this=key_map[0][key_buf.key[i].pos[0]][key_buf.key[i].pos[1]];
			if(key_this==clean_key){
				clean_mode=1;
				break;
			}
		}
	}
	for(u8 i=0;i<key_buf.key_cnt;i++){
		u8 key_this=key_map[0][key_buf.key[i].pos[0]][key_buf.key[i].pos[1]];
		if(key_this==fn1){
			current_mode=1;
			break;
		}
	}
	for(u8 i=0;i<key_buf.key_cnt;i++){


		if(commu_buf_pre.key[i].pos[0]!=key_buf.key[i].pos[0]||commu_buf_pre.key[i].pos[1]!=key_buf.key[i].pos[1]){
			send=1;
			commu_buf_pre.key[i].pos[0]=key_buf.key[i].pos[0];
			commu_buf_pre.key[i].pos[1]=key_buf.key[i].pos[1];
		}
		
	}
	commu_buf_pre.key_cnt=key_buf.key_cnt;
	commu_buf_pre.control=key_buf.control;
	if(send){

		for(u8 i=2;i<2+key_buf.key_cnt;i++){
			u8 key_this=key_map[0][key_buf.key[i-2].pos[0]][key_buf.key[i-2].pos[1]];
			if(key_this==fn2){
				if(!current_mode)
					current_mode=2;
				else
					current_mode=0;
				break;
			}
		}
		key_map_t map;
		map=&key_map[current_mode];
//		switch(current_mode){
//			case 0:
//				map=&key_index;
//				break;
//			case 1:
//				    map=&key_index1;
//				break;
//			case 2:
//				map=&key_index2;
//				break;
//		}
		key_set(buf,map,&key_buf);
//		led_reset();
		keyboard_send(buf);		
	}
}


