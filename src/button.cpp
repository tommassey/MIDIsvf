#include "button.h"
#include "pinDefines.h"



void buttonInit(Bounce* btn, uint8_t pin, uint8_t mode, uint16_t bounceTime)
{
    btn->attach(pin, mode);
    btn->interval(bounceTime);  // debounce time
}


int8_t checkButton(Bounce* btn)
{
  static uint32_t buttonholdstarttime = 0;
  btn->update();

  if (btn->rose())
  {
    if ((buttonholdstarttime + buttonholdtimeforsave) <= millis())   //  if button's been held long enough, long press
    {
        buttonholdstarttime = 0;
        return longPress;
    }

    else                                                             //  if buttons not been held, short press
    {
        buttonholdstarttime = 0;
        return shortPress;
    }
    
  }

  if (btn->fell())    //  start counting when button is held
  {
    buttonholdstarttime = millis();
  }
  return noPress;
}