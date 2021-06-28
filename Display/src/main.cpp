#include <Arduino.h>
#include "displayService.h"
#include <Wire.h>
#include "comms/reciever.h"



oled screen(128, 64);
displayService display(&screen);

void checkSerial(void)
{
  serialMessage newMessage = getSerialMessage();
  
  switch (newMessage.messageType)
  {
    case noCommand:  break;

    case changeScreen:
    {
      display.showScreen(newMessage.data);
      break;
    }

    case setWaveform:
    {
      display.setLFOwave(newMessage.data);
      break;
    }

    case setRate:
    {
      display.setLFOrate(newMessage.data);
      break;
    }

    case setAmp:
    {
      display.setLFOamp(newMessage.data);
      break;
    }

    case setPhase:
    {
      display.setLFOphase(newMessage.data);
      break;
    }
    
    default: break;
    }
    
}


void setup()
{
  Serial.begin(115200);

  Serial.println("init");
  Serial.println("OLED Example\n");
  Wire.begin();
  Wire.setClock(1000000);

  screen.begin();
  screen.clear();

}

void loop()
{
  checkSerial();
}