#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
void keyboard_init(void);

void keyboard_scan(void);
#define  COL_LEN 14
#define  ROW_LEN 5
#define  KEY_BUF_LEN 14
typedef unsigned char u8;
typedef struct{
	u8 pos[2];
} single_key_t;
typedef struct {
	u8 control;
	single_key_t key[KEY_BUF_LEN];                      /*!< Key used with keyboard. This can be whatever. Like numbers, letters, everything. */
	u8 key_cnt;
} key_t;
extern const unsigned char key_map[3][ROW_LEN][COL_LEN];
extern unsigned char current_mode;
extern unsigned char clean_mode;
#endif //_KEYBOARD_H_
