#include "LED.h"

bool LEDstate = LOW;

void updateLED(LED* led)
{
  uint32_t newTime = millis();
  LEDblinkProfile* profile = &led->currentBlinkProfile;

  if (led->currentBlink < profile->numberBlinks) // if we've still go blinks to go
  {
        switch (led->ledState)
        {
        case LOW:                             //  when the LED is off
          {
            if (newTime >= (led->currentBlinkProfile.TimeOff[led->currentBlink] + led->oldTime)) // if its time to turn back on
            { 
              //Serial.print(" LED on, current blink = ");
              //Serial.println(led->currentBlink);
              
              led->oldTime = newTime;
              led->ledState = HIGH;
              digitalWrite(CONFIG_LED_PIN, HIGH);
              break;
            }
          }

        case HIGH:                             //  when the LED is on
          {
            if (newTime >= (led->currentBlinkProfile.TimeOff[led->currentBlink] + led->oldTime))  // if its time to turn off
            {
              led->currentBlink++;
                if (led->currentBlink > profile->numberBlinks)
                {
                  profile->currentBlink = 0;
                  //Serial.print(" resets blinks = ");
                  //Serial.println(led->currentBlink);
                }
              led->oldTime = newTime;
              led->ledState = LOW;
              digitalWrite(CONFIG_LED_PIN, LOW);
              //Serial.print(" LED off, current blink = ");
              //Serial.println(led->currentBlink);
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