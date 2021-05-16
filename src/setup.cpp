#include "setup.h"
#include "pinDefines.h"

Bounce button = Bounce();
LED led;


void setupStuff()
{
  Serial.begin(115200);
  initPins();

  usbMIDI.begin();

  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
}

void initPins(void)
{
  pinMode(CONFIG_LED_PIN, OUTPUT);
  digitalWrite(CONFIG_LED_PIN, LOW);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  pinMode(CONFIG_SWITCH_PIN, INPUT_PULLUP);

  if (digitalRead(CONFIG_SWITCH_PIN) == LOW)  //  only 
  {
    button.attach(INPUT_BUTTON_PIN, INPUT);
    button.interval(25);
  }
}

void MIDIconfigMode()
{
  led.updateLED();
  checkButtons();


}




void checkButtons(void)
{
  button.update();
  if (button.fell())
  {
    led.setLEDflashtime(500, 200);
    Serial.println("button");
  }
}