#include <Arduino.h>
#include "displayService.h"
#include <Wire.h>
#include "TeensyTimerTool.h"
#include "inputs/inputManager.h"


#include "comms/reciever.h"

using namespace TeensyTimerTool;


oled screen(128, 64);
displayService display(&screen);


PeriodicTimer checkButtonsTimer(TCK);
PeriodicTimer checkEncodersTimer(TCK);

void checkButtonsTimerISR(void)
{
  checkButtons();
}

void checkEncodersTimerISR(void)
{
  checkEncoders();
}




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
  Serial2.begin(115200);

  Serial.println("setup ...");

  inputManager_init();

  checkButtonsTimer.begin(checkButtonsTimerISR, 50_Hz);  // setup timers
  checkEncodersTimer.begin(checkEncodersTimerISR, 60_Hz);  // setup timers

    
  screen.clear();

  display.showScreen(1);

  //delay(5000);

}





void loop()
{
  checkSerial();
}