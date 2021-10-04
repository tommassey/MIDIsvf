#include <Arduino.h>
#include "display/displayService.h"
#include <Wire.h>
#include "TeensyTimerTool.h"
#include "inputs/inputManager.h"
#include "menu/menu.h"
#include "comms/receiver.h"




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
  recieveCommand(command_LFO1_setRate, 2);
}

void noteOffISR(void)
{
  recieveCommand(command_LFO1_setRate, 1);
}




void checkSerial(void)
{
  // serialMessage newMessage = getSerialMessage();
  
  // switch (newMessage.messageType)
  // {
  //   case noCommand:  break;

  //   case changeScreen:
  //   {
  //     display.showFullScreen(newMessage.data);
  //     break;
  //   }

  //   case setWaveform:
  //   {
  //     display.setLFOwave(newMessage.data);
  //     break;
  //   }

  //   case setRate:
  //   {
  //     display.setLFOrate(newMessage.data);
  //     break;
  //   }

  //   case setAmp:
  //   {
  //     display.setLFOamp(newMessage.data);
  //     break;
  //   }

  //   case setPhase:
  //   {
  //     display.setLFOphase(newMessage.data);
  //     break;
  //   }
    
  //   default: break;
  //   }
    
}


void setup()
{
  Serial.println("setup ...");
  Serial.begin(115200);
  Serial2.begin(115200);
  
  receiverInit();

  

  inputManager_init();
  
  initMenu(&display);

  noteOnTimer.begin(noteOnISR, 0.3_Hz);  // setup timers
  noteOffTimer.begin(noteOffISR, 0.4_Hz);  // setup timers
  checkEncodersTimer.begin(checkEncodersTimerISR, 50_Hz);  // setup timers

    
  screen.clear();

  //display.showScreen(1);
  //display.splitScreen();
  //delay(5000);

  Serial.println("setup complete...");

}





void loop()
{
  receiverTask();
  
  int8_t input = checkInputs();

  if (input != no_input)
  {
    //Serial.print("input == ");
    //Serial.println(input);
    
    if (input == menu_encoder)
    {
      newEncoderMovement(getEncoderChange());
    }

    actOnInputs(input);
    resetInputFlag();
  }

  display.checkForRedraw();
}