#include "LED.h"



void updateLED(LED* led)
{

  uint32_t newTime = millis();
  LEDblinkProfile* profile = &led->currentBlinkProfile;

  if (profile->currentBlink < profile->numberBlinks) // if we've still go blinks to go
  {
        switch (led->ledState)
        {
        case LOW:                             //  when the LED is off
          {

            if (newTime >= (led->currentBlinkProfile.TimeOff[profile->currentBlink] + led->oldTime))
            //if (newTime >= (oldTime + LEDtimeOff))  // if its time to turn back on
            {
                profile->currentBlink++;
              led->oldTime = newTime;
              led->ledState = HIGH;
              digitalWrite(CONFIG_LED_PIN, HIGH);
              break;
            }
          }

        case HIGH:                             //  when the LED is on
          {
            if (newTime >= (led->currentBlinkProfile.TimeOff[profile->currentBlink] + led->oldTime))  // if its time to turn off
            {
              led->oldTime = newTime;
              led->ledState = LOW;
              digitalWrite(CONFIG_LED_PIN, LOW);
              break;
            }
          }

        default:
          break;
        }

    }


  
}

void LED::setBlinkProfile(LEDblinkProfile newProfile)
{
    currentBlinkProfile = newProfile;
}