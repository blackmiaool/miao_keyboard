# blackmiaool's keyboard
<p align="center">      
    <img width="600" src="https://raw.githubusercontent.com/blackmiaool/miao_keyboard/master/photo.jpg">  
    <img width="600" src="https://raw.githubusercontent.com/blackmiaool/miao_keyboard/master/photo2.jpg">  
</p>

### Feature

* Split spaces (for close alt+f and ctrl+j (as emacs short keys))
* Emulates as udisk + mouse + keyboard
* Config key map with configuration files (in udisk)
* Runs lua (in udisk)
* Keyboard macro (in udisk)
* Fixs ubuntu capslock delay
* Different modes for different key maps
* Media functions (HID consumer page)

### Enable web feature
```bash
sudo sh -c "touch /etc/udev/rules.d/50-miao_keyboard.rules && echo 'SUBSYSTEM==\"usb\", ATTR{idVendor}==\"0483\", MODE=\"0664\", GROUP=\"plugdev\"' >> /etc/udev/rules.d/50-miao_keyboard.rules && service udev restart"
```
Unplug and plug usb.