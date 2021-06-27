#include <Arduino.h>
#include "setup.h"
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
PeriodicTimer printTimer(TCK);


MIDIconfigProfile config[parameterTotal];

MIDIservice midi;


bool configMode = false;   //  when true, we go into config mode

Smoother smoother[parameterTotal];
bool smoothFlag = false;
uint16_t valueToBeSmoothed[parameterTotal] = {0};
uint16_t value[parameterTotal] = {0};                   //  14bit values


float LFOvalue = 0;   //  +/- 2048
LFO lfo(&LFOvalue);

bool LFOresetFlag = false;


uint16_t DAC1finalOutput = 0; // 4095 max
uint16_t DAC2finalOutput = 0; //  


void smoothISR(void)
{
  smoothFlag = true;
}


void printISR(void)
{
  // Serial.print("lfo = ");
  // Serial.print(LFOvalue);
  // Serial.print("    amt = ");
  // Serial.println(LFOamtA);

  Serial.println("send msg");
  //Serial4.println(1,1);
  Serial4.write('3');
  Serial4.write(',');
  Serial4.write('2');
  Serial4.write('\n');
  
}


bool smoothValues(void)  // return true if it smoothed
{  
  if (smoothFlag)
  {
    for (byte i = parameterFilter1; i < parameterTotal; i++)
    {
      uint16_t val = smoother[i].smooth(valueToBeSmoothed[i]);  //  add new val to smoothing array
      value[i] = val;
    }
    smoothFlag = false;
    return true;
  }
  return false;
}


void isrWriteToDAC(void)
{
  DACwriteBothChannels(DAC1finalOutput, DAC2finalOutput);   
}


void sumBeforeDAC(void)
{
  DAC1finalOutput = (value[parameterFilter1]>>3) + (LFOvalue * LFOamtA);
    
    if (DAC1finalOutput > 4095) DAC1finalOutput = 4095;
    if (DAC1finalOutput < 0) DAC1finalOutput = 0;
  
  DAC2finalOutput = (value[parameterFilter2]>>3) + (LFOvalue * LFOamtA);
    
    if (DAC2finalOutput > 4095) DAC2finalOutput = 4095;
    if (DAC2finalOutput < 0) DAC2finalOutput = 0;
}





void setup()
{
  Serial.begin(9600);
  initPins(&button);
  setupStuff();

  Serial4.begin(9600);
  
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
  initMCP4xxx();            // init DAC
  checkForSavedMIDIdata();  
  config[parameterFilter1] = getFilterConfig(parameterFilter1);  //  load MIDI config data into MIDIservice
  config[parameterFilter2] = getFilterConfig(parameterFilter2);


  midi.initParameter(noParameter, config[noParameter], &valueToBeSmoothed[noParameter], &LFOresetFlag);  // init pointers for values
  midi.initParameter(parameterFilter1, config[parameterFilter1], &valueToBeSmoothed[parameterFilter1], &LFOresetFlag);
  midi.initParameter(parameterFilter2, config[parameterFilter2], &valueToBeSmoothed[parameterFilter2], &LFOresetFlag);

  midi.printConfigData();

  LFOtimer.begin(isrWriteToDAC, 88.2_kHz);  // setup timers
  potTimer.begin(readpotsISR, 100_Hz);
  smoothTimer.begin(smoothISR, 88.2_kHz);
  printTimer.begin(printISR, 0.1_Hz);


  lfo.initWaveForms();  //  load arrays with wavetable data
}


void loop()
{
  checkPots(&lfo);
  lfo.update();
  midi.check();
  if (smoothValues()) sumBeforeDAC();
  
  if (LFOresetFlag)
  {
    lfo.reset();
    LFOresetFlag = false;
  }


}


