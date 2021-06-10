#include "setup.h"

void setupStuff()
{
  usbMIDI.begin();

  digitalWrite(LED_BUILTIN, HIGH);        //  flash led to let us know it's rebooted
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
}


void initPins(Bounce* btn)
{
  pinMode(CONFIG_LED_PIN, OUTPUT);
  digitalWrite(CONFIG_LED_PIN, LOW);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  pinMode(CONFIG_SWITCH_PIN, INPUT_PULLUP);
  digitalWrite(CONFIG_SWITCH_PIN, HIGH);

  if (digitalRead(INPUT_BUTTON_PIN) == LOW) buttonInit(btn, INPUT_BUTTON_PIN, INPUT_PULLUP, 25);   //  only if we want to go into config mode
}


