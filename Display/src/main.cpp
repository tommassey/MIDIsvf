#include <Arduino.h>
#include "displayService.h"
#include <Wire.h>
#include <Encoder.h>

#include "comms/reciever.h"
#include "drivers/button.h"

Bounce button1 = Bounce();   // LFO1 button
Bounce button2 = Bounce();   //  LFO2 button
Bounce button3 = Bounce();   //  Encoder button

Encoder encoder = Encoder(3,2);

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

  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);

  Serial.begin(115200);
  Serial2.begin(115200);

  Serial.println("init");
  Serial.println("OLED Example\n");

  buttonInit(&button1, 6, INPUT_PULLUP, 50);
  buttonInit(&button2, 5, INPUT_PULLUP, 50);
  buttonInit(&button3, 4, INPUT_PULLUP, 50);
  
  
  
  screen.clear();

  display.showScreen(1);

  //delay(5000);

}



uint8_t button1Presses = 0;
uint8_t button2Presses = 0;
uint8_t button3Presses = 0;

bool buttonChange = false;

void checkAllButtons(void)
{
  if (checkButton(&button1) == shortPress)
  {
    button1Presses++;
    buttonChange = true;
  }

  if (checkButton(&button2) == shortPress)
  {
    button2Presses++;
    buttonChange = true;
  }

  if (checkButton(&button3) == shortPress)
  {
    button3Presses++;
    buttonChange = true;
  }

  if (buttonChange == true)
  {
    Serial.print("BUTTONS  1 = ");
    Serial.print(button1Presses);
    Serial.print("     2 = ");
    Serial.print(button2Presses);
    Serial.print("     3 = ");
    Serial.println(button3Presses);

    buttonChange = false; 
  }
}

uint32_t encoderCount = 0;

void checkEncoder(void)
{
  uint32_t newEncoderCount = encoder.read();

  if (newEncoderCount != encoderCount)
  {
    Serial.print("Encoder = ");
    Serial.println(newEncoderCount);
    encoderCount = newEncoderCount;
  }
}

void loop()
{
  checkSerial();
  checkAllButtons();
  checkEncoder();

}