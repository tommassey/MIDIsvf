#include <Arduino.h>
#include "setup.h"
//#include "MIDIservice.h"
#include "MIDIsetup.h"
#include "TeensyTimerTool.h"
#include "LFO.h"
#include "inputs/potSmoothing.h"

using namespace TeensyTimerTool;

LED led;
Bounce button = Bounce();

bool potchangeflag = false;
uint16_t potval = 0;

pot potty(22, &potval, &potchangeflag);

PeriodicTimer LFOtimer(TCK);
PeriodicTimer potTimer(TCK);
PeriodicTimer potTimer2(TCK);

MIDIconfigProfile mainfilter1;
MIDIconfigProfile mainfilter2;

bool configMode = false;   //  when true, we go into config mode

uint16_t output1CCamt = 0;  //  +4095
uint16_t output2CCamt = 0;

float output1LFOamt = 0.5;  //  +/-512
float output2LFOamt = 2;

int16_t LFOvalue = 0;   //  +/- 2048

float DAC1finalOutput = 0; // 4095 max
float DAC2finalOutput = 0;



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

void readpots(void)
{
  potty.read();
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
  potTimer2.begin(readpots, 100_Hz);

  initSineTable();
  initNMLtable();

  initFilterPointers(&output1CCamt, &output2CCamt);
  initLFOpointer(&LFOvalue);


}


void loop()
{

  if (potchangeflag) potty.update();
  //DACrawSpeedTest();
  byte MIDIchange = checkMIDI();



  DAC1finalOutput = (float)output1CCamt + ((float)LFOvalue * output1LFOamt);
  if (DAC1finalOutput > 4095) DAC1finalOutput = 4095;
  if (DAC1finalOutput < 0) DAC1finalOutput = 0;
  
  DAC2finalOutput = (float)output2CCamt + ((float)LFOvalue * output2LFOamt);
  if (DAC2finalOutput > 4095) DAC2finalOutput = 4095;
  if (DAC2finalOutput < 0) DAC2finalOutput = 0;
  

  // if (MIDIchange == Filter1)
  // {
  //   DAC1finalOutput = (float)output1CCamt + ((float)LFOvalue * output1LFOamt);
  //   if (DAC1finalOutput > 4095) DAC1finalOutput = 4095;
  //   if (DAC1finalOutput < 0) DAC1finalOutput = 0;
  // }
  // else if (MIDIchange == Filter2)
  // {
  //   DAC2finalOutput = output2CCamt * output2LFOamt;
  //   if (DAC2finalOutput > 4095) DAC2finalOutput = 4095;
  //   if (DAC2finalOutput < 0) DAC2finalOutput = 0;
  // }
  // else
  // {
  //   DAC1finalOutput = (float)output1CCamt + ((float)LFOvalue * output1LFOamt);
  //   if (DAC1finalOutput > 4095) DAC1finalOutput = 4095;
  //   if (DAC1finalOutput < 0) DAC1finalOutput = 0;

  // }
  
  


  // if ((output1LFOamt || output2LFOamt) != 0)
  // {

  // }



    updateLFO();
  
  //getRatePotValue();
}


