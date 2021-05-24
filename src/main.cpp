#include <Arduino.h>
#include "setup.h"
//#include "MIDIservice.h"
#include "MIDIsetup.h"

LED led;
LED* configLED = &led;

Bounce button = Bounce();
Bounce* configBtn = &button;

MIDIconfigProfile mainfilter1;
MIDIconfigProfile mainfilter2;


void printMIDIprofiles()
{
  Serial.println("== Filter 1 ===========");
  Serial.print("CH: ");
  Serial.print(mainfilter1.channel);
  Serial.print("   RES: ");
  Serial.print(mainfilter1.resolution);
  Serial.print("   CC: ");
  Serial.print(mainfilter1.CCforMSB);
  Serial.print("   MIN: ");
  Serial.print(mainfilter1.minValue);
  Serial.print("   MAX: ");
  Serial.println(mainfilter1.maxValue);

  Serial.println("== Filter 2 ===========");
  Serial.print("CH: ");
  Serial.print(mainfilter2.channel);
  Serial.print("   RES: ");
  Serial.print(mainfilter2.resolution);
  Serial.print("   CC: ");
  Serial.print(mainfilter2.CCforMSB);
  Serial.print("   MIN: ");
  Serial.print(mainfilter2.minValue);
  Serial.print("   MAX: ");
  Serial.println(mainfilter2.maxValue);
}


void setup()
{
  Serial.begin(9600);
  initPins(configBtn);
  setupStuff();
  

  

  


  if (digitalRead(INPUT_BUTTON_PIN) == LOW)    //  if config switch is low during startup, enter config mode
  //if (digitalRead(CONFIG_SWITCH_PIN) == LOW)    //  if config switch is low during startup, enter config mode
  {
    Serial.println("ConfigMode");
    initMIDIconfig(configLED, configBtn); //, &filter1, &filter2);
    //delay(5000);    //  wait to allow serial monitor to catch up
    while (1)
    {
      MIDIconfigModeLoop();
    }
  }

  //else
  initMCP4xxx();
  checkForSavedMIDIdata();
  mainfilter1 = getFilterConfig(Filter1);
  mainfilter2 = getFilterConfig(Filter2);
  setMIDIprofiles(&mainfilter1, &mainfilter2);
}


void loop()
{
  checkMIDI();
}


