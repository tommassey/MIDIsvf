#include <Arduino.h>
#include "setup.h"
//#include "MIDIservice.h"
#include "MIDIsetup.h"
#include "TeensyTimerTool.h"
#include "LFO/LFO.h"
#include "inputs/inputs.h"

using namespace TeensyTimerTool;

LED led;



PeriodicTimer LFOtimer(TCK);
PeriodicTimer potTimer(TCK);
PeriodicTimer smoothTimer(TCK);

MIDIconfigProfile mainfilter1;
MIDIconfigProfile mainfilter2;

bool configMode = false;   //  when true, we go into config mode

uint16_t output1CCamt = 0;  //  +4095
uint16_t output2CCamt = 0;

int16_t LFOvalue = 0;   //  +/- 2048

uint16_t DAC1finalOutput = 0; // 4095 max
uint16_t DAC2finalOutput = 0;



LFO lfo(&LFOvalue);

bool smoothFlag = false;

void smoothISR(void)
{
  smoothFlag = true;
}


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

void isrWriteToDAC(void)
{
  DACwriteBothChannels(DAC1finalOutput, DAC2finalOutput);   
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
  potTimer.begin(readpotsISR, 100_Hz);
  smoothTimer.begin(smoothISR, 1000_Hz);

  lfo.initWaveForms();

  initFilterPointers(&output1CCamt, &output2CCamt);
 

}


void loop()
{

  checkPots(&lfo);
  lfo.update();
  smoothCCs(&smoothFlag, &output2CCamt, &output2CCamt);

  //DACrawSpeedTest();
  byte MIDIchange = checkMIDI();








  DAC1finalOutput = output1CCamt + ((float)LFOvalue * LFOamtA);
  
  if (DAC1finalOutput > 4095) DAC1finalOutput = 4095;
  if (DAC1finalOutput < 0) DAC1finalOutput = 0;
  
  DAC2finalOutput = (float)output2CCamt + ((float)LFOvalue * LFOamtA);
  if (DAC2finalOutput > 4095) DAC2finalOutput = 4095;
  if (DAC2finalOutput < 0) DAC2finalOutput = 0;
  



    
  
  
}


