#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "inifile.h"
#define BUF_SIZE 256

struct ini_top ini;
char ini_tmp[BUF_SIZE];
int ini_read_string(const char *section,const char *key)
{
    if(!read_profile_string(section, key, ini_tmp, BUF_SIZE,"true","profile.ini"))
    {
        rt_kprintf("read ini file fail\r\n \
        section:%s \r\n key:%s \r\n in file %s",section,key,"profile.ini");
        return -1;
    }
    else
    {
        //rt_kprintf("%s=%s\n",key,ini_tmp);  
        return 0;
    }
}

int ini_read_int(const char *section,const char *key)
{
//    static char rec[BUF_SIZE]={0};
    int value=read_profile_int(section, key,-1,"profile.ini");
    if(value==-1)
    {
        rt_kprintf("read ini file fail\r\n \
        section:%s \r\n key:%s \r\n in file %s",section,key,"profile.ini");
        return -1;
    }
    else
    {
        //rt_kprintf("%s=%s\n",key,rec);  
        return 0;
    }
}

int judge_bool(char *bool_in)
{
    u8 i=0;
    char str_temp[6];
    for(i=0;i<5;i++)
    {
        if(bool_in[i]>='a')
            str_temp[i]=bool_in[i]-('a'-'A');
        else
            str_temp[i]=bool_in[i];
    }
    str_temp[5]=0;
    if(!strcmp("FALSE",str_temp))
    {
        return false;
    }
    else
    {
        str_temp[4]=0;
        if(!strcmp("TRUE",str_temp))
            return true;
    }        
    return 2;
}
int read_bool(bool *variable,const char *section,const char *key)
{
    if(!ini_read_string(section,key))    
    {
        if((*variable=judge_bool(ini_tmp))==2)       
        { 
            if(ini.Debug.ini)
            rt_kprintf("------------------\r\n\
section:%s \r\n\
key:%s \r\n\
expect bool value (true or false)but get %s\r\n------------------\r\n\
",section,key,ini_tmp);  
            return -1; 
        } 
        else   
        {
            if(ini.Debug.ini)
            rt_kprintf("%s=%s gotten\r\n",key,ini_tmp); 
             return 0; 
        }                    
    }
    else
        return -1;   
                                
}
int read_int(int *variable,const char *section,const char *key)
{
    if((*variable=ini_read_int(section,key))==0)    
    {
        if(ini.Debug.ini)
        rt_kprintf("%s=%s gotten\r\n",key,ini_tmp); 
        return 0;                      
    }
    else
        if(ini.Debug.ini)
        rt_kprintf("\
------------------\r\n\
section:%s \r\n\
key:%s \r\n\
expect int value %s\r\n\
-------------------\r\n\
",section,key,ini_tmp);  
            return -1;   
                                
}
int  ini_init()
{
#define READ_BOOL(top,section,key) (read_bool(&top.section.key,#section,#key))
#define READ_INT(top,section,key) (read_int(&top.section.key,#section,#key))
    ini.Debug.ini=true;
    READ_BOOL(ini,Debug,ini);  
    READ_BOOL(ini,Service,audio);
    READ_BOOL(ini,Service,key_remap);
    READ_BOOL(ini,Service,ahk);
    READ_BOOL(ini,Service,mouse_gesture);
    READ_INT(ini,Service,first_mode);
    READ_BOOL(ini,Service,lua_script);
    READ_BOOL(ini,Service,ultrasonic);
    READ_BOOL(ini,Debug,usb_init);
    READ_BOOL(ini,Debug,audio);
    READ_BOOL(ini,Debug,mouse);
    READ_BOOL(ini,Debug,keyboard);
    READ_BOOL(ini,Debug,nrf24l01);
    READ_BOOL(ini,Debug,key_remap);
    READ_BOOL(ini,Debug,ahk);
    READ_INT(ini,OLED,orientation);
    READ_INT(ini,OLED,orientation);
    
    return 0;  
}








#define MAX_FILE_SIZE 1024*2
#define LEFT_BRACE '['
#define RIGHT_BRACE ']'



static int load_ini_file(const char *file_name, char *buf,int *file_size)
{
        static FIL file;
        unsigned int cnt=0;   
        *file_size = 0;
        if(f_open(&file,file_name,FA_OPEN_EXISTING|FA_WRITE|FA_READ|FA_OPEN_ALWAYS|FA__WRITTEN))
        {
            printf("ini read fail\r\n");
            return 0;
        }
                

        if(f_read(&file,buf,file.fsize,&cnt))
                 return 0;
        buf[cnt] = '\0';
        *file_size = cnt;
        f_close(&file);
        return 1;
}

static int newline(char c)
{
        return ('\n' == c || '\r' == c )? 1 : 0;
}

static int end_of_string(char c)
{
        return '\0'==c? 1 : 0;
}

static int left_barce(char c)
{
        return LEFT_BRACE == c? 1 : 0;
}

static int isright_brace(char c )
{
        return RIGHT_BRACE == c? 1 : 0;
}

static int parse_file(const char *section, const char *key, const char *buf,int *sec_s,int *sec_e,
                int *key_s,int *key_e, int *value_s, int *value_e)
{
        const char *p = buf;
        int i=0;

        *sec_e = *sec_s = *key_e = *key_s = *value_s = *value_e = -1;

        while( !end_of_string(p[i]) ) {
                //find the section
                if( ( 0==i || newline(p[i-1]) ) && left_barce(p[i]) )
                {
                        int section_start=i+1;
                        //find the ']'
                        do {
                                i++;
                        } while( !isright_brace(p[i]) && !end_of_string(p[i]));
                        if( 0 == strncmp(p+section_start,section, i-section_start)) {
                                int newline_start=0;
                                i++;
                                //Skip over space char after ']'
                                while(isspace(p[i])) {
                                        i++;
                                }
                                //find the section
                                *sec_s = section_start;
                                *sec_e = i;
                                while( ! (newline(p[i-1]) && left_barce(p[i])) 
                                                && !end_of_string(p[i]) ) {
                                        int j=0;
                                        //get a new line
                                        newline_start = i;
                                        while( !newline(p[i]) && !end_of_string(p[i]) ) {
                                                i++;
                                        }
                                        //now i is equal to end of the line
                                        j = newline_start;
                                        if(';' != p[j]) //skip over comment
                                        {
                                                while(j < i && p[j]!='=') {
                                                        j++;
                                                        if('=' == p[j]) {
                                                                if(strncmp(key,p+newline_start,j-newline_start)==0)
                                                                {
                                                                        //find the key ok
                                                                        *key_s = newline_start;
                                                                        *key_e = j-1;

                                                                        *value_s = j+1;
                                                                        *value_e = i;

                                                                        return 1;
                                                                }
                                                        }
                                                }
                                        }
                                        i++;
                                }
                        }
                }
                else
                {
                        i++;
                }
        }
        return 0;
}

/**
  read string in initialization file\n
 * retrieves a string from the specified section in an initialization file
 section [in] name of the section containing the key name
 key [in] name of the key pairs to value 
 value [in] pointer to the buffer that receives the retrieved string
 size [in] size of result's buffer 
 default_value [in] default value of result
 file [in] path of the initialization file
1 : read success; \n 0 : read fail
*/
int read_profile_string( const char *section, const char *key,char *value, 
                int size, const char *default_value, const char *file_name)
{
        char buf[MAX_FILE_SIZE];
        int file_size;
        int sec_s,sec_e,key_s,key_e, value_s, value_e;

        if(!load_ini_file(file_name,buf,&file_size))
        {
                if(default_value!=NULL)
                {
                        strncpy(value,default_value, size);
                }
                return 0;
        }

        if(!parse_file(section,key,buf,&sec_s,&sec_e,&key_s,&key_e,&value_s,&value_e))
        {
                if(default_value!=NULL)
                {
                        strncpy(value,default_value, size);
                }
                return 0; //not find the key
        }
        else
        {
                int cpcount = value_e -value_s;

                if( size-1 < cpcount)
                {
                        cpcount = size-1;
                }

                memset(value, 0, size);
                memcpy(value,buf+value_s, cpcount );
                value[cpcount] = '\0';
                return 1;
        }
}

/**
  read int value in initialization file\n
 * retrieves int value from the specified section in an initialization file
 section [in] name of the section containing the key name
 key [in] name of the key pairs to value 
 default_value [in] default value of result
 file [in] path of the initialization file
 profile int value,if read fail, return default value
*/
int read_profile_int( const char *section, const char *key,int default_value, 
                const char *file_name)
{
        char value[32] = {0};
        if(!read_profile_string(section,key,value, sizeof(value),NULL,file_name))
        {
                return default_value;
        }
        else
        {
                return atoi(value);
        }
}

/**
 * @brief write a profile string to a ini file
 * @param section [in] name of the section,can't be NULL and empty string
 * @param key [in] name of the key pairs to value, can't be NULL and empty string
 * @param value [in] profile string value
 * @param file [in] path of ini file
 * @return 1 : success\n 0 : failure
*/
int write_profile_string(const char *section, const char *key, const char *value, const char *file_name)
{ 
        char buf[MAX_FILE_SIZE]={0};
        char w_buf[MAX_FILE_SIZE]={0};
        int sec_s,sec_e,key_s,key_e, value_s, value_e;
        int value_len = (int)strlen(value);
        int file_size;
        static FIL file;
        unsigned int cnt;
        
        if(!load_ini_file(file_name,buf,&file_size))
        {
                sec_s = -1;
        }
        else
        {
                parse_file(section,key,buf,&sec_s,&sec_e,&key_s,&key_e,&value_s,&value_e);
        }

        if( -1 == sec_s)
        {
                if(0==file_size)
                {
                        sprintf(w_buf+file_size,"[%s]\n%s=%s\n",section,key,value);
                }
                else
                {
                        //not find the section, then add the new section at end of the file
                        memcpy(w_buf,buf,file_size);
                        sprintf(w_buf+file_size,"\n[%s]\n%s=%s\n",section,key,value);
                }
        }
        else if(-1 == key_s)
        {
                //not find the key, then add the new key=value at end of the section
                memcpy(w_buf,buf,sec_e);
                sprintf(w_buf+sec_e,"%s=%s\n",key,value);
                sprintf(w_buf+sec_e+strlen(key)+strlen(value)+2,buf+sec_e, file_size - sec_e);
        }
        else
        {
                //update value with new value
                memcpy(w_buf,buf,value_s);
                memcpy(w_buf+value_s,value, value_len);
                memcpy(w_buf+value_s+value_len, buf+value_e, file_size - value_e);
        }

        if(f_open(&file,file_name,FA_OPEN_EXISTING|FA_WRITE|FA_READ|FA_OPEN_ALWAYS|FA__WRITTEN))
                return 0;
        if(f_write(&file, w_buf, file_size, &cnt))
        {
                f_close(&file);
                return 0;
        }
        f_close(&file);
        return 1;
}



