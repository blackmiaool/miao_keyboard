
#include "stm32lib.h"
void USB_Cable_Init()
{
    SCPE(PERIOA);
    IOConfig(IOAB,low,PIN3,3);
    
}
void USB_Cable_Config (FunctionalState NewState)
{
    if(NewState==ENABLE)
    {
        PAout(3)=1;
    }
    else
    {
PAout(3)=0;
    }
}

