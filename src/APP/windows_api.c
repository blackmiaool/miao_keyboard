#include "windows_api.h"
#include "cashware_config.h"
#include "app_interface.h"
void win_api_window_min()
{
    u16 buf[6];
    buf[0]=(1<<9)+(1<<control_key_index("lalt"));//press alt
    buf[1]=ascii2usb[32];
    buf[2]=(1<<10)+(1<<2);//release alt
    buf[3]=(1<<9)+(1<<control_key_index("lctrl"));//press ctrl
    buf[4]=ascii2usb['n'];
    buf[5]=(1<<10)+(1<<control_key_index("lctrl"));
    press_string_pure(buf,6);
}
void win_api_window_toRight()
{
    u16 buf[6];
    buf[0]=(1<<9)+(1<<control_key_index("lgui"));//press alt
    buf[1]=79;
    buf[2]=79;
    buf[3]=79;//right
    buf[4]=82;
    buf[5]=(1<<10)+(1<<control_key_index("lgui"));//release alt
    press_string_pure(buf,5);
}
void win_api_window_toLeft()
{
    u16 buf[6];
    buf[0]=(1<<9)+(1<<control_key_index("lgui"));//press alt
    buf[1]=80;
    buf[2]=80;
    buf[3]=80;//left
    buf[4]=82;
    buf[5]=(1<<10)+(1<<control_key_index("lgui"));//release alt
    press_string_pure(buf,5);
}
