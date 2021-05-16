#include "LED.h"



void LED::updateLED(void)
{
  uint32_t newTime = millis();

  switch (ledState)
  {
  case LOW:                             //  when the LED is off
    {
      if (newTime >= (oldTime + LEDtimeOff))  // if its time to turn back on
      {
        oldTime = newTime;
        ledState = HIGH;
        break;
      }
    }
  
  case HIGH:                             //  when the LED is on
    {
      if (newTime >= (oldTime + LEDtimeOn))  // if its time to turn off
      {
        oldTime = newTime;
        ledState = LOW;
        break;
      }
    }

  default:
    break;
  }
  
  digitalWrite(CONFIG_LED_PIN, ledState);
}

void LED::setLEDflashtime(uint32_t timeon, uint32_t timeoff)
{
  LEDtimeOn = timeon;
  LEDtimeOff = timeoff;
}