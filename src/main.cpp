#include <Arduino.h>
#include "setup.h"
#include "MIDIservice.h"



void setup()
{
  delay(2000);
  setupStuff();

  if (digitalRead(CONFIG_SWITCH_PIN) == LOW)
  {
    Serial.println("ConfigMode");
    while (1)
    {
      MIDIconfigMode();
    }
  }

  //else
  initMCP4xxx();
}


void loop()
{
  checkMIDI();
}


