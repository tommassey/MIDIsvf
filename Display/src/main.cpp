#include <Arduino.h>
#include "displayService.h"
#include <Wire.h>
#include "TeensyTimerTool.h"
#include "inputs/inputManager.h"


#include "comms/reciever.h"

using namespace TeensyTimerTool;


oled screen(128, 64);
displayService display(&screen);


PeriodicTimer noteOnTimer(TCK);
PeriodicTimer noteOffTimer(TCK);
PeriodicTimer checkEncodersTimer(TCK);

void checkButtonsTimerISR(void)
{
  //checkButtons();
}

void checkEncodersTimerISR(void)
{
  checkEncoders();
  //checkButtons();
}

void noteOnISR(void)
{
  display.noteOnEvent(LFO_1);
  display.noteOffEvent(LFO_2);
  Serial.println("on");
}

void noteOffISR(void)
{
  display.noteOnEvent(LFO_2);
  display.noteOffEvent(LFO_1);
  Serial.println("off ...");
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

  noteOnTimer.begin(noteOnISR, 0.3_Hz);  // setup timers
  noteOffTimer.begin(noteOffISR, 0.4_Hz);  // setup timers
  checkEncodersTimer.begin(checkEncodersTimerISR, 50_Hz);  // setup timers

    
  screen.clear();

  //display.showScreen(1);
  display.splitScreen();
  //delay(5000);

}





void loop()
{
  //Serial.println("loop");
  checkSerial();
  
  int8_t input = checkInputs();

  if (input != no_input)
  {
    //Serial.print("input == ");
    //Serial.println(input);
    
    if (input == menu_encoder)
    {
      display.newEncoderMovement(getEncoderChange());
    }

    display.actOnInputs(input);
    resetInputFlag();
  }

  display.checkForRedraw();
}