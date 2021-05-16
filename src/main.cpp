#include <Arduino.h>
#include "setup.h"
#include "MIDIservice.h"



void setup()
{
  setupStuff();

  delay(5000);    //  wait to allow serial monitor to catch up


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


