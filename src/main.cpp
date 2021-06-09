#include <Arduino.h>
#include "setup.h"
//#include "MIDIservice.h"
#include "MIDIsetup.h"
#include "MIDIserv.h"
#include "TeensyTimerTool.h"
#include "LFO/LFO.h"
#include "inputs/inputs.h"
#include "smooth.h"
#include "DACservice.h"
#include "DACdriver.h"

using namespace TeensyTimerTool;

LED led;


PeriodicTimer LFOtimer(TCK);
PeriodicTimer potTimer(TCK);
PeriodicTimer smoothTimer(TCK);


MIDIconfigProfile config[parameterTotal];

MIDIservice midi;


bool configMode = false;   //  when true, we go into config mode

Smoother smoother[parameterTotal];
bool smoothFlag = false;
uint16_t valueToBeSmoothed[parameterTotal] = {0};
uint16_t value[parameterTotal] = {0};                   //  14bit values





float LFOvalue = 0;   //  +/- 2048
LFO lfo(&LFOvalue);



uint16_t DAC1finalOutput = 0; // 4095 max
uint16_t DAC2finalOutput = 0; //  


void smoothISR(void)
{
  smoothFlag = true;
}

bool smoothValues(void)  // return true if it smoothed
{
  
  if (smoothFlag)
  {
    // Serial.println("time to smooth");
    // Serial.print("vals to smooth  0 = ");
    // Serial.print(valueToBeSmoothed[noParameter]);
    // Serial.print("   F1 = ");
    // Serial.print(valueToBeSmoothed[parameterFilter1]);
    // Serial.print("   F2 = ");
    // Serial.println(valueToBeSmoothed[parameterFilter2]);

    for (byte i = parameterFilter1; i < parameterTotal; i++)
    {
      uint16_t val = smoother[i].smooth(valueToBeSmoothed[i]);  //  add new val to smoothing array
      //  Serial.print("val in = ");
      //  Serial.println(val);
      value[i] = val; //scaleForDAC(val, config[i]);              //  scale new average and write to external pointer location
    }
    // Serial.print("done smoothing 0 = ");
    // Serial.print(value[noParameter]);
    // Serial.print("    F1 = ");
    // Serial.print(value[parameterFilter1]);
    // Serial.print("    F2 = ");
    // Serial.println(value[parameterFilter2]);

    smoothFlag = false;
    return true;
  }
  return false;
}



void isrWriteToDAC(void)
{
  
  DACwriteBothChannels(DAC1finalOutput, DAC2finalOutput);   
}

//int count = 0; 

void sumBeforeDAC(void)
{
  //count++;

  DAC1finalOutput = (value[parameterFilter1]>>3) + (LFOvalue * LFOamtA);
    
    if (DAC1finalOutput > 4095) DAC1finalOutput = 4095;
    if (DAC1finalOutput < 0) DAC1finalOutput = 0;
  
  DAC2finalOutput = (value[parameterFilter2]>>3) + (LFOvalue * LFOamtA);
    
    if (DAC2finalOutput > 4095) DAC2finalOutput = 4095;
    if (DAC2finalOutput < 0) DAC2finalOutput = 0;

    // if (count == 1000)
    // {
    //   Serial.print("DAC1 final  = ");
    //   Serial.print(DAC1finalOutput);
    //   Serial.print("      DAC2 final  = ");
    //   Serial.println(DAC2finalOutput);
    //   count = 0;
    // }
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
  config[parameterFilter1] = getFilterConfig(parameterFilter1);  //  load MIDI config data into MIDIservice
  config[parameterFilter2] = getFilterConfig(parameterFilter2);


  midi.initParameter(noParameter, config[noParameter], &valueToBeSmoothed[noParameter]);
  midi.initParameter(parameterFilter1, config[parameterFilter1], &valueToBeSmoothed[parameterFilter1]);
  midi.initParameter(parameterFilter2, config[parameterFilter2], &valueToBeSmoothed[parameterFilter2]);

  midi.printConfigData();

  LFOtimer.begin(isrWriteToDAC, 88.2_kHz);
  potTimer.begin(readpotsISR, 100_Hz);
  smoothTimer.begin(smoothISR, 88.2_kHz);

  lfo.initWaveForms();
}


void loop()
{

  checkPots(&lfo);
  lfo.update();

  //DACrawSpeedTest();
  //byte MIDIchange = checkMIDI();
  midi.check();
  if (smoothValues()) sumBeforeDAC();









  
  



    
  
  
}


