#include "buttonDriver.h"


#define BUTTON_HOLD_TIME_FOR_LONG_PRESS_MILLIS 500   // millis

uint32_t buttonholdstarttime = 0;

ButtonDriver::ButtonDriver(Bounce* btn, uint8_t pin, uint8_t mode, uint16_t bounceTime)
{
  deBouncer = btn;

  deBouncer->attach(pin, mode);
  deBouncer->interval(bounceTime);  // debounce time
}



uint8_t ButtonDriver::check(void)  //  called to poll the button, returns a value from buttonPresses enum
{
  deBouncer->update();

  if (deBouncer->rose())
  {
    if ((buttonholdstarttime + BUTTON_HOLD_TIME_FOR_LONG_PRESS_MILLIS) <= millis())   //  if button's been held long enough, long press
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

  if (deBouncer->fell())    //  start counting when button is held
  {
    buttonholdstarttime = millis();
  }

  return noPress;
}