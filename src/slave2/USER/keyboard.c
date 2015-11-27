#include "keyboard.h"
#include <stm32f10x_lib.h>
#include "stm32lib.h"
#include "hw_config.h"
#include "sys.h"
#include "stm32f10x_gpio.h"
#include "usart.h"
#include "commu_mcu.h"
typedef u16 uint16_t;

struct GPIO_struct{
    GPIO_TypeDef* port;
    u32 pin;
	  u8 num;
};
#define  COL_LEN 14
#define  ROW_LEN 5
struct GPIO_struct keyboard_gpio_rows[ROW_LEN];
struct GPIO_struct keyboard_gpio_cols[COL_LEN];

GPIO_TypeDef* char2port(char ch);
u32 str2pin(char *str);
u8 key_index[5][14]={{41 ,30 ,31 ,32 ,33 ,34 ,35 ,36 ,37 ,38 ,39 ,45 ,46 ,42},
{43 ,20 ,26 ,8  ,21 ,23 ,28 ,24 ,12 ,18 ,19 ,47 ,48 ,49},
{57 ,4  ,22 ,7  ,9  ,10 ,11 ,13 ,14 ,15 ,51 ,52 ,40 ,40},
{225,225,29 ,27 ,6  ,25 ,5  ,17 ,16 ,54 ,55 ,56 ,0  ,229},
{224,227,82 ,226 ,44,44 ,0  ,0  ,228,80,79,80,79},};
typedef struct {
	u8 L_CTRL;  /*!< Left CTRL button. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	u8 L_ALT;   /*!< Left ALT button. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	u8 L_SHIFT; /*!< Left SHIFT button. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	u8 L_GUI;   /*!< Left GUI (Win) button. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	u8 R_CTRL;  /*!< Right CTRL button. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	u8 R_ALT;   /*!< Right ALT button. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	u8 R_SHIFT; /*!< Right SHIFT button. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	u8 R_GUI;   /*!< Right GUI (Win) button. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
  u8 key[6];                      /*!< Key used with keyboard. This can be whatever. Like numbers, letters, everything. */
} key_t;
extern void keyborad_process(u8* buf);
void keyboard_send(key_t key_buf){
	u8 buf[9];
	buf[0]=0;
	buf[1]=(key_buf.L_CTRL<<0)+(key_buf.L_SHIFT<<1)+(key_buf.L_ALT<<2)+(key_buf.L_GUI<<3)+(key_buf.R_CTRL<<4)+(key_buf.R_SHIFT<<5)+(key_buf.R_ALT<<6)+(key_buf.R_GUI<<7);
	buf[2]=0;
	for(u8 i=0;i<6;i++){
		buf[3+i]=key_buf.key[i];
	}
	
	keyborad_process(buf);
}
char cols[]="C0C1C2C3C4C5C6C7C8C9C10C11C12C13";
char rows[]="B5B6B7B8B9";

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
		long num=atoi(cols+i);
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
		long num=atoi(rows+i);
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


void keyboard_send_commu(key_t key_buf);
//static u8 key_index[5][14];
u16 key_val[5];

void keyboard_scan(){
    u8 i=0,j=0;
    for(j=0;j<ROW_LEN;j++){
        key_val[j]=0;
       
			  IOout(keyboard_gpio_rows[j].port,keyboard_gpio_rows[j].num,1);
    }
//		delay_ms(1);
    for(j=0;j<ROW_LEN;j++){
        IOout(keyboard_gpio_rows[j].port,keyboard_gpio_rows[j].num,0);
//        delay_ms(1);
        for(i=0;i<COL_LEN;i++){
					u8 val=IOin(keyboard_gpio_cols[i].port,keyboard_gpio_cols[i].num);
					
//					printf("key %d %d",i,val);
            key_val[j]+=((val>0)<<i);
//					delay_ms(1);
        }
//				printf("\r\n");
//				delay_ms(1);
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
		for(i=0;i<6;i++){
			key_buf.key[i]=0;
			
		}
		key_buf.L_ALT=0;
		key_buf.L_CTRL=0;
		key_buf.L_SHIFT=0;
		key_buf.L_GUI=0;
		key_buf.R_ALT=0;
		key_buf.R_CTRL=0;
		key_buf.R_SHIFT=0;
		key_buf.R_GUI=0;
    for(j=0;j<ROW_LEN;j++){
        for(i=0;i<COL_LEN;i++){
            if(key_val[j]&(1<<i))
            {
                switch(key_index[j][i]){
                case 225:
                    key_buf.L_SHIFT=1;
                    break;
                case 224:
                    key_buf.L_CTRL=1;
                    break;
                case 227:
                    key_buf.L_GUI=1;
                    break;
                case 226:
                    key_buf.L_ALT=1;
                    break;
                case 228:
                    key_buf.R_CTRL=1;
                    break;
                case 229:
                    key_buf.R_SHIFT=1;
                    break;
                case 230:
                    key_buf.R_ALT=1;
                    break;
                default:
                    key_buf.key[key_data_index++] = key_index[j][i];
                }
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
//		commu_send(key_buf+1,8,COMMU_TYPE(KEYBOARD_MS));
        keyboard_send_commu(key_buf);
		
//        rt_kprintf("key press %d  %d %d\n",j,i,Keyboard.key[0]);
    }
    else if(pre_press){
        pre_press=0;
		
        keyboard_send_commu(key_buf);
    }

end:
		;

//    rt_kprintf("\n");

}

u8 commu_buf_pre[8];
void keyboard_send_commu(key_t key_buf){
	u8 buf[8];
	buf[0]=(key_buf.L_CTRL<<0)+(key_buf.L_SHIFT<<1)+(key_buf.L_ALT<<2)+(key_buf.L_GUI<<3)+(key_buf.R_CTRL<<4)+(key_buf.R_SHIFT<<5)+(key_buf.R_ALT<<6)+(key_buf.R_GUI<<7);
	buf[1]=0;
	for(u8 i=0;i<6;i++){
		buf[2+i]=key_buf.key[i];		
	}
	u8 send=0;
	for(u8 i=0;i<8;i++){
		if(buf[i]!=commu_buf_pre[i]){
			send=1;
			break;
		}
	}
	if(send){
		for(u8 i=0;i<8;i++){
			commu_buf_pre[i]=buf[i];
		}
		commu_send(buf,8,COMMU_TYPE(KEYBOARD_MS));
		
	}
} 

//void keyboard_io_init(){
//    u8 i=0,j=0;
//    for(i=0;i<col_len;i++){
//        TM_GPIO_Init(keyboard_gpio_cols[i].port, keyboard_gpio_cols[i].pin, TM_GPIO_Mode_IN, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_High);
//    }
//    for(j=0;j<row_len;j++){
//        TM_GPIO_Init(keyboard_gpio_rows[j].port, keyboard_gpio_rows[j].pin, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_High);
//    }
//}

