#include <Arduino.h>
#include "setup.h"
//#include "MIDIservice.h"
#include "MIDIsetup.h"
#include "TeensyTimerTool.h"
#include "LFO.h"

using namespace TeensyTimerTool;

LED led;
Bounce button = Bounce();



PeriodicTimer LFOtimer(TCK);
PeriodicTimer potTimer(TCK);

MIDIconfigProfile mainfilter1;
MIDIconfigProfile mainfilter2;

bool configMode = false;   //  when true, we go into config


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
  initPins(&button);
  setupStuff();

  

  

  


  if (digitalRead(INPUT_BUTTON_PIN) == LOW)    //  if button is low during startup, enter config mode
  {
    configMode = true;
    Serial.println("ConfigMode");

    initMIDIconfig(&configMode, &led, &button);

    while (configMode)
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

  LFOtimer.begin(isrWriteToDAC, 44.1_kHz);
  potTimer.begin(getRatePotValue, 10_Hz);

  //initSineTable();
  initNMLtable();


}


void loop()
{
  //DACrawSpeedTest();
  checkMIDI(&led);
  updateLFO();
  //getRatePotValue();
}


