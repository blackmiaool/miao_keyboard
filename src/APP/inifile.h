/**
* @file
* @brief initialization file read and write API
* -size of the ini file must less than 16K
* -after '=' in key value pair, can not support empty char. this would not like WIN32 API
* -support comment using ';' prefix
* -can not support multi line
* @author Deng Yangjun
* @date 2007-1-9
* @version 0.2
*/

#ifndef INI_FILE_H_
#define INI_FILE_H_


#include "ff.h"
#include "rtthread.h"
#include "stm32f4lib.h"
int read_profile_string( const char *section, const char *key,char *value, int size,const char *default_value,const char *file_name);
int read_profile_int( const char *section, const char *key,int default_value,const char *file_name);
int write_profile_string( const char *section, const char *key,const char *value,const char *file_name);
int  ini_init(void);
struct ini_parameters{
    int key_interval;
}; 
struct ini_oled{
    int orientation;
};  
struct ini_service{
    bool audio;
    bool key_remap;
    bool ahk;
    bool mouse_gesture;
    int first_mode;
    bool lua_script;
    bool ultrasonic;
    
};

struct ini_debug{
    bool usb_init;
    bool audio;
    bool mouse;
    bool keyboard;
    bool nrf24l01;
    bool ini;
    bool key_remap;
    bool ahk;
};
struct ini_top{
    struct ini_debug Debug;
    struct ini_service Service;
    struct ini_oled OLED;
    struct ini_parameters Parameters;
};
extern struct ini_top ini;

#endif //end of INI_FILE_H_
