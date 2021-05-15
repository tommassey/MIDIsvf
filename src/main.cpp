#include <Arduino.h>
#include "setup.h"
#include "MIDIservice.h"


void setup()
{
  Serial.begin(9600);
  initMCP4xxx();
  setupStuff();
}


void loop()
{
  CCevent newCC = readMIDI();


  switch (newCC.whichCC)
  {
      case nofilter:
      {
        //Serial.println("no change");
        break;
      } 

      case filter1:
      {
        Serial.println("filter1 change");
        DACwriteChannelA(newCC.value);
        break;
      }

      case filter2:
      {
        Serial.println("filter2 change");
        DACwriteChannelB(newCC.value);
        break;
      }

      default: Serial.println("default");
      
  }

  //DACtest();
  //delay(3);
  
}


