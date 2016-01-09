#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
void keyboard_init(void);

void keyboard_scan(void);
#define  COL_LEN 14
#define  ROW_LEN 5


extern const unsigned char key_map[3][ROW_LEN][COL_LEN];
extern unsigned char current_mode;
extern unsigned char clean_mode;
#endif //_KEYBOARD_H_
