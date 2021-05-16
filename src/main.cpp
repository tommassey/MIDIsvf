#include <Arduino.h>
#include "setup.h"
#include "MIDIservice.h"



void setup()
{
  delay(2000);
  setupStuff();

  if (digitalRead(CONFIG_SWITCH_PIN) == LOW)    //  if config switch is low during startup, enter config mode
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


