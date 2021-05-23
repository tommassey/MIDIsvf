#include <Arduino.h>
#include "setup.h"
//#include "MIDIservice.h"
#include "MIDIsetup.h"

LED led;
LED* configLED = &led;

Bounce button = Bounce();
Bounce* configBtn = &button;


void test()
{
  
}



void setup()
{
  Serial.begin(9600);
  initPins();
  setupStuff();
  

  if (digitalRead(CONFIG_SWITCH_PIN) == LOW)  //  only if we're in config mode
  {
    buttonInit(configBtn, INPUT_BUTTON_PIN, INPUT_PULLUP, 25);
  }

  delay(1000);    //  wait to allow serial monitor to catch up


  if (digitalRead(CONFIG_SWITCH_PIN) == LOW)    //  if config switch is low during startup, enter config mode
  {
    Serial.println("ConfigMode");
    initMIDIconfig(configLED, configBtn);
    while (1)
    {
      //MIDIconfigMode();
      testEEPROM();
    }
  }

  //else
  initMCP4xxx();
}


void loop()
{
  checkMIDI();
}


