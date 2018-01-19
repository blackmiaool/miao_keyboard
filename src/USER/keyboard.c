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
u8 clean_mode=0;//disable app programs
u8 udisk_mode=0;
u8 clean_key=41;//press esc to enter clean mode 
u8 udisk_key=30;//press esc to enter clean mode 
u8 fn1=135;
u8 fn2=136;

u8 key_time[ROW_LEN][COL_LEN];
const u8 key_map[3][ROW_LEN][COL_LEN]={{{41 ,30 ,31 ,32 ,33 ,34 ,35 ,36 ,37 ,38 ,39 ,45 ,46 ,42},
{43 ,20 ,26 ,8  ,21 ,23 ,28 ,24 ,12 ,18 ,19 ,47 ,48 ,49},
{57 ,4  ,22 ,7  ,9  ,10 ,11 ,13 ,14 ,15 ,51 ,52 ,40 ,40},
{225,225,29 ,27 ,6  ,25 ,5  ,17 ,16 ,54 ,55 ,56 ,0  ,229},
{224,227,82 ,226 ,44,44 ,44  ,44  ,228,80,79,80,135,136},},

{{41 ,58 ,59 ,60 ,61 ,62 ,63 ,64 ,65 ,66 ,67 ,68 ,69 ,42},//active when pressing fn1
{43 ,20 ,26 ,8  ,21 ,23 ,28 ,24 ,12 ,18 ,19 ,47 ,48 ,49},
{57 ,4  ,22 ,7  ,9  ,10 ,11 ,13 ,14 ,15 ,51 ,52 ,40 ,40},
{225,225,29 ,27 ,6  ,25 ,5  ,17 ,16 ,54 ,55 ,56 ,0  ,229},
{224,227,82 ,226 ,44,44 ,44  ,44  ,228,80,79,80,0,0},},

{{41 ,30 ,31 ,32 ,33 ,34 ,35 ,36 ,37 ,38 ,39 ,45 ,46 ,42},//active when pressed fn2 
{43 ,20 ,26 ,8  ,21 ,23 ,28 ,24 ,82 ,18 ,19 ,47 ,48 ,49},
{57 ,4  ,22/*s*/ ,7  ,9  ,10 /*g*/,11 ,80/*j*/ ,81 ,79 ,51/*;*/ ,52 ,40 ,40},
{225,225,29 ,27 ,6  ,25 ,5  ,17 ,16 ,54 ,55 ,56 ,0  ,229},
{224,227,82 ,226 ,44,44 ,44  ,44  ,228,80,79,80,0,0},}};



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

extern void keyboard_process(u8* buf);
const char cols[]="C0C1C2C3C4C5C6C7C8C9C10C11C12C13";
const char rows[]="B5B6B7B8B9";
u8 get_key(u8 index,u8 row,u8 col){
	if(key_index_lua_buf){
		return key_index_lua_buf[index*ROW_LEN*16+row*16+col];
	}else{
		return key_map[index][row][col];
	}
	
}
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


u8 key_prevent_cycle=2;
u8 key_state_map[256];
void scan_update(){
    for(int i=0;i<256;i++){
        if(key_state_map[i]){
            key_state_map[i]--;
        }
    }
}


u8 scan_push_check(u8 key){
    if(!key_state_map[key]){
       key_state_map[key]=key_prevent_cycle;
       return 0; 
    }else
        return 1;
        
}

static u16 matrix_debouncing[ROW_LEN];
#define DEBOUNCE 5
u8 debouncing=DEBOUNCE;
u8 hardware_scan(u16 *map){
//    for(u8 j=0;j<ROW_LEN;j++){
//        map[j]=0;
//        IOout(keyboard_gpio_rows[j].port,keyboard_gpio_rows[j].num,1);
//    }

//    for(u8 j=0;j<ROW_LEN;j++){
//        IOout(keyboard_gpio_rows[j].port,keyboard_gpio_rows[j].num,0);
//        for(u8 i=0;i<COL_LEN;i++){
//			u8 val=IOin(keyboard_gpio_cols[i].port,keyboard_gpio_cols[i].num);
//            map[j]+=((val>0)<<i);
//        }
//        u16 mask=1<<COL_LEN;
//        mask--;
//        map[j]=mask-map[j];
//        IOout(keyboard_gpio_rows[j].port,keyboard_gpio_rows[j].num,1);
//    }
	
	for (u8 i = 0; i < ROW_LEN; i++) {
        IOout(keyboard_gpio_rows[i].port,keyboard_gpio_rows[i].num,0);
        delay_us(30);
        u16 cols = 0;
		for(u8 i=0;i<COL_LEN;i++){
			u8 val=IOin(keyboard_gpio_cols[i].port,keyboard_gpio_cols[i].num);
            cols+=((val>0)<<i);
        }
		u16 mask=1<<COL_LEN;
        mask--;
		cols=mask-cols;
		
        if (matrix_debouncing[i] != cols) {
            matrix_debouncing[i] = cols;
            debouncing = DEBOUNCE;
        }
        IOout(keyboard_gpio_rows[i].port,keyboard_gpio_rows[i].num,1);
    }

    if (debouncing) {
        if (--debouncing) {
            delay_ms(1);
			return 0;
        } else {
            for (u8 i = 0; i < ROW_LEN; i++) {
                map[i] = matrix_debouncing[i];
            }
			return 1;
        }
		
    }
	return 0;
}
static key_t keys_pre={0,0};
void key_t_add(key_t *keys,u8 p0,u8 p1){
	keys->key[keys->cnt].pos[0]=p0;
	keys->key[keys->cnt].pos[1]=p1;
	keys->cnt++;
}
key_filter_list_t keys_list_press={0};
key_filter_list_t keys_list_release={0};
#define TIMER_DIFF(a, b, max)   ((a) >= (b) ?  (a) - (b) : (max) - (b) + (a))

void update_list(key_filter_list_t *list,u16 time){
	for(u8 i=0;i<list->cnt;i++){
		key_filter_t *filter=&list->filter[i];
		if(TIMER_DIFF(TIM1->CNT,filter->time,9999)/10>=time){//timeout, remove
			for(u8 j=i;j<list->cnt-1;j++){
				list->filter[j]=list->filter[j+1];
			}
			list->cnt--;
		}
	}
}
u8 try_modify_key(key_filter_list_t* list,single_key_t* key){
	for(u8 i=0;i<list->cnt;i++){
		key_filter_t *filter=&list->filter[i];
		if(filter->key.pos[0]==key->pos[0]&&filter->key.pos[1]==key->pos[1]){
			return false;
		}
	}
	if(list->cnt<KEY_BUF_LEN){
		list->filter[list->cnt].key=*key;
		list->filter[list->cnt].time=TIM1->CNT;
		list->cnt++;
		return true;
	}else{
		return false;
	}	
}
u8 has_key(key_t *keys,single_key_t key){
	for(u8 j=0;j<keys->cnt;j++){
		if(key.pos[0]==keys->key[j].pos[0]&&key.pos[1]==keys->key[j].pos[1]){
			return 1;
		}
	}
	return 0;
}
#define DEBOUNCE 5
void keyboard_scan(){
	if(!hardware_scan(key_val)){
		return;
	}
	
//    for(j=0;j<ROW_LEN;j++){
//		printf("key value %d %3d",j,key_val[j]);
//    }

    u8 changes=false;
	
	key_t keys_pressing={0,0};
	key_t keys_send={0,0};
	key_t keys_next={0,0};
	
    for(u8 j=0;j<ROW_LEN;j++){
        for(u8 i=0;i<COL_LEN;i++){
            if(key_val[j]&(1<<i))
            {
                u8 value=get_key(0,j,i);
				key_t_add(&keys_pressing,j,i);

//                printf("key press %d  %d %d\n",j,i, key_index[j][i]);
                if(keys_pressing.cnt==KEY_BUF_LEN){
                   goto scan_end;
                }
            }
        }
    }
	scan_end:;	
//	update_list(&keys_list_press,FILTER_TIME_AFTER_PRESS);
//	update_list(&keys_list_release,FILTER_TIME_AFTER_RELEASE);
	for(u8 i=0;i<keys_pressing.cnt;i++){//check new pressed
		single_key_t key=keys_pressing.key[i];
				
		if(!has_key(&keys_pre,key)){			
			if(1||try_modify_key(&keys_list_press,&key)){
				changes=1;
				key_t_add(&keys_next,key.pos[0],key.pos[1]);
			}else{
				//pass
			}
		}
	}
	
	
	for(u8 i=0;i<keys_pre.cnt;i++){//check new pressed
		single_key_t key=keys_pre.key[i];
		
		if(!has_key(&keys_pressing,key)){			
			if(1||try_modify_key(&keys_list_release,&key)){
				changes=1;
			}else{				
				key_t_add(&keys_next,key.pos[0],key.pos[1]);
			}
		}else{
			key_t_add(&keys_next,key.pos[0],key.pos[1]);
		}
	}
	keys_pre=keys_next;
	
	
	u8 modify_key=0;
	for(u8 i=0;i<keys_next.cnt;i++){//check new pressed
		single_key_t key=keys_next.key[i];
		u8 value=get_key(0,key.pos[0],key.pos[1]);
		switch(value){
			case 225:
				modify_key|=(1<<1);
				break;
			case 224:
				modify_key|=(1<<0);
				break;
			case 227:
				modify_key|=(1<<3);
				break;
			case 226:
				modify_key|=(1<<2);
				break;
			case 228:
				modify_key|=(1<<4);
				break;
			case 229:
				modify_key|=(1<<5);
				break;
			case 230:
				modify_key|=(1<<6);
				break;
			case 231:
				modify_key|=(1<<7);
				break;
			default:
				key_t_add(&keys_send,key.pos[0],key.pos[1]);
		}
	}
	keys_send.control=modify_key;
	

	if(changes){
		keyboard_send_wrap(keys_send);
	}
//    if(key_press){
//        keyboard_send_wrap(keys_pressing);
//    }
//    else if(pre_press){
//        pre_press=0;
//        keyboard_send_wrap(keys_pressing);
//    }

}


extern u8 delegate;

void app_handle(u8 *buf,key_t* );
void keyboard_send_wrap2(u8 *buf);
void keyboard_send(u8 *buf,key_t* bufp){
	if(delegate){
		commu_send(buf,8,COMMU_TYPE(KEYBOARD_MS));
	}else{	
		app_handle(buf,bufp);
	}
}

void find_pos(u8 **value,u8 index,u8* pos){
	
}

key_t commu_buf_pre;
//typedef const u8 Key_map[ROW_LEN][COL_LEN];
//typedef Key_map *key_map_t;
u8 current_mode=0;
void key_set(u8 *buf,u8 index,const key_t *key){
	for(u8 i=0;i<key->cnt;i++){
		buf[2+i]=get_key(index,key->key[i].pos[0],key->key[i].pos[1]);
	}
}
u8 check_udisk_mode(key_t key_buf){
	for(u8 i=0;i<key_buf.cnt;i++){
		u8 key_this=get_key(0,key_buf.key[i].pos[0],key_buf.key[i].pos[1]);
		if(key_this==udisk_key){
			return 1;
		}
	}
	return 0;
}
u8 check_clean_mode(key_t key_buf){
	for(u8 i=0;i<key_buf.cnt;i++){
		u8 key_this=get_key(0,key_buf.key[i].pos[0],key_buf.key[i].pos[1]);
		if(key_this==clean_key){
			return 1;
		}
	}
	return 0;
}
void keyboard_send_wrap(key_t key_buf){
	u8 buf[8];
	buf[0]=key_buf.control;
	buf[1]=0;
	for(u8 i=2;i<8;i++){
		buf[i]=0;
	}
	
	u8 changed=0;//if key state change
	if(commu_buf_pre.cnt!=key_buf.cnt)//if key cnt changes
		changed=1;
	if(commu_buf_pre.control!=key_buf.control)//if control key changes
		changed=1;
	if(current_mode!=2){
		current_mode=0; 
	}
	
	if(start_check){//just check once when power on. If clean_mode, disable all app programs.
		start_check=0;
		clean_mode=check_clean_mode(key_buf);
		udisk_mode=check_udisk_mode(key_buf);
	}
	
	if(clean_mode){
		for(u8 i=0;i<key_buf.cnt;i++){//if enter mode 1
			u8 key_this=get_key(0,key_buf.key[i].pos[0],key_buf.key[i].pos[1]);
			if(key_this==fn1){
				current_mode=1;
				break;
			}
		}
	}
	
	for(u8 i=0;i<key_buf.cnt;i++){//if key changes
		if(commu_buf_pre.key[i].pos[0]!=key_buf.key[i].pos[0]||commu_buf_pre.key[i].pos[1]!=key_buf.key[i].pos[1]){
			changed=1;
			commu_buf_pre.key[i].pos[0]=key_buf.key[i].pos[0];
			commu_buf_pre.key[i].pos[1]=key_buf.key[i].pos[1];
		}
	}
	commu_buf_pre.cnt=key_buf.cnt;
	commu_buf_pre.control=key_buf.control;
	if(changed){
		if(clean_mode){
			for(u8 i=2;i<2+key_buf.cnt;i++){
				u8 key_this=get_key(0,key_buf.key[i-2].pos[0],key_buf.key[i-2].pos[1]);
				if(key_this==fn2){
					if(!current_mode)
						current_mode=2;
					else
						current_mode=0;
					break;
				}
			}
		}

		key_set(buf,current_mode,&key_buf);
//		led_reset();
//        scan_push_check(buf);
		keyboard_send(buf,&key_buf);		
	}
}


