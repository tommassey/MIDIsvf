#include "LED.h"

uint32_t timeOld_mS = 0;
uint32_t timeTarget_mS = 500;

bool LEDstate = LOW;

// void updateLED(LED* led)
// {
//   LEDblinkProfile* profile = &led->currentBlinkProfile;
//   uint32_t timeNow_mS = millis();
  
//   if (timeNow_mS >= timeTarget_mS)  // if it's time to do something
//   {
//     if (LEDstate == LOW) // when LED is off
//     {
//       LEDstate = !LEDstate;        //  turn LED on
//       digitalWrite(CONFIG_LED_PIN, LEDstate);
//       timeTarget_mS += 5000;  // set a new time to switch off
//       return;
//     }
    
//     if (LEDstate == HIGH) // when LED is on
//     {
//       LEDstate = !LEDstate;        //  turn LED off
//       digitalWrite(CONFIG_LED_PIN, LEDstate);
//       timeTarget_mS += 100;  // set a new time to switch back on
//       return;
//     }
//   }
// }


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

            if (newTime >= (led->currentBlinkProfile.TimeOff[led->currentBlink] + led->oldTime))
            //if (newTime >= (oldTime + LEDtimeOff))  // if its time to turn back on
            { 
              Serial.print(" LED on, current blink = ");
              Serial.println(led->currentBlink);
              
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
                  Serial.print(" resets blinks = ");
                  Serial.println(led->currentBlink);
                }
              led->oldTime = newTime;
              led->ledState = LOW;
              digitalWrite(CONFIG_LED_PIN, LOW);
              Serial.print(" LED off, current blink = ");
              Serial.println(led->currentBlink);
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