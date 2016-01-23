#include "led.h"
#include "stdio.h"
#include <stm32f10x_lib.h>
#include "hw_config.h"
#include "sys.h"
#include "stm32f10x_gpio.h"
#include "usart.h"
#include "commu_mcu.h"
#include "keyboard.h"
static u8 buf[14][5];
static char SER595[]="A0";
static char RCK595[]="A1";
static char SCK595[]="A2";
static char rows[]="B0B1B2B3B4";
static struct GPIO_struct rows_io[ROW_LEN];
static struct GPIO_struct SER595_io;
static struct GPIO_struct RCK595_io;
static struct GPIO_struct SCK595_io;
GPIO_TypeDef* char2port(char ch);
void io_bulk_parse(char *data,u16 size,struct GPIO_struct* buf){
	u16 i=0;
	u16 cnt=0;
	while(i<size-1){
		struct GPIO_struct* io=&buf[cnt];		
		io->port=char2port(data[i]);
		i++;
		long num=atoi(data+i);
		io->pin=1<<(num%8*4);
		io->num=num;
		if(io->num>9)
		{
			i++;
		}
		i++;
		delay_ms(10);
//		printf(" n:%d %d\r\n",io->pin,io->num);
		delay_ms(10);
		cnt++;
	}
}
void io_bulk_config(struct GPIO_struct* io,u8 size,u8 config){
	while(size--){
		IOConfig(io[size].port, io[size].num>7, io[size].pin, config);
		IOout(io[size].port,io[size].num,1);
	}
	
}


void led_init(){
//	buf[1][0]=1;
	io_bulk_parse(rows,sizeof(rows),rows_io);
	io_bulk_parse(SER595,sizeof(SER595),&SER595_io);
	io_bulk_parse(RCK595,sizeof(RCK595),&RCK595_io);
	io_bulk_parse(SCK595,sizeof(SCK595),&SCK595_io);
	
	for(u8 i=0;i<5;i++){
		IOConfig(rows_io[i].port, rows_io[i].num>7, rows_io[i].pin, 3);
		IOout(rows_io[i].port,rows_io[i].num,1);
	}
	IOConfig(SER595_io.port, SER595_io.num>7, SER595_io.pin, 3);
	IOout(SER595_io.port,SER595_io.num,0);
	
	IOConfig(RCK595_io.port, RCK595_io.num>7, RCK595_io.pin, 3);
	IOout(RCK595_io.port,RCK595_io.num,1);
	
	IOConfig(SCK595_io.port, SCK595_io.num>7, SCK595_io.pin, 3);
	IOout(SCK595_io.port,SCK595_io.num,1);
	u8 b=buf[0][0];
	b=b;
	for(u8 i=0;i<ROW_LEN;i++){
		for(u8 j=0;j<COL_LEN;j++){
			buf[i][j]=1;
		}
	}
}
//10000times  20layer 500time per bright  25time*20

//static void fresh(){
//	for(u8 j=0;j<16;j++){
//		for(u8 i=0;i<5;i++){
//			IOout(rows_io[i].port,rows_io[i].num,buf[j][i]);
//		}

//		IOout(SCK595_io.port,SCK595_io.num,0);
////		delay_us(1);
//		IOout(SCK595_io.port,SCK595_io.num,1);
//	}
//	IOout(RCK595_io.port,RCK595_io.num,0);
//	delay_us(1);
//	IOout(RCK595_io.port,RCK595_io.num,1);
//	
//}
const float cycle=0.5;//1s
const int call_times_per_s=10000;
const int layer=40;
static u32 tick=call_times_per_s*cycle;
void led_reset(){
	tick=(u32)(call_times_per_s*cycle);
}
void set_all(u8 value){
	IOout(SER595_io.port,SER595_io.num,!value);
	for(u8 i=0;i<16;i++){
		IOout(SCK595_io.port,SCK595_io.num,0);
		delay_us(1);
		IOout(SCK595_io.port,SCK595_io.num,1);
	}
	IOout(RCK595_io.port,RCK595_io.num,0);
	delay_us(1);
	IOout(RCK595_io.port,RCK595_io.num,1);
	
}



void led_handle(){
//	static u8 i=0;
//	if(tick>0)
//		tick--;
	
//	tick=tick%(call_times_per_s*2);
//	u16 bright=tick/((u16)(call_times_per_s*cycle)/layer)%layer;
////	fresh();
//	u8 value=tick%layer<bright;
//	if(tick>call_times_per_s){
//		value=!value;
//	}
	tick++;

	u32 chu=1;
	u8 row=tick/chu%5;
	for(u8 i=0;i<5;i++){
		IOout(rows_io[i].port,rows_io[i].num,1);
	}
	for(u8 i=0;i<16;i++){
		IOout(RCK595_io.port,RCK595_io.num,0);
		IOout(SCK595_io.port,SCK595_io.num,0);		
		IOout(RCK595_io.port,RCK595_io.num,1);
		IOout(SCK595_io.port,SCK595_io.num,1);
		IOout(SER595_io.port,SER595_io.num,key_map[current_mode][row][COL_LEN-1-i]==key_map[0][row][COL_LEN-1-i]&&current_mode!=0);
	}
	
	IOout(rows_io[row].port,rows_io[row].num,1);
//	set_all(1);
	
	
	
//	IOout(SER595_io.port,SER595_io.num,!value);

}
