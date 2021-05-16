#include "setup.h"
#include "pinDefines.h"
#include "MIDIservice.h"

Bounce button = Bounce();
LED led;
LED* configLED = &led;


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


  if (digitalRead(CONFIG_SWITCH_PIN) == LOW)  //  only if we're in config mode
  {
    button.attach(INPUT_BUTTON_PIN, INPUT_PULLUP);
    button.interval(25);  // debounce time
  }
}


byte currentConfigMode = CONFIG_MODE_init;

void MIDIconfigMode()
{
  updateLED(configLED);
  checkButtons();


  switch (currentConfigMode)
  {
    case CONFIG_MODE_init:
    {
      configLED->setBlinkProfile(init);
      break;
    }
    case CONFIG_MODE_filter1:
    {
     configLED->setBlinkProfile(f1config);

      break;
    }
    case CONFIG_MODE_filter2:
    {
      configLED->setBlinkProfile(f2config);
      break;
    }
    case CONFIG_MODE_save:
    {
      configLED->setBlinkProfile(saved);
      break;
    }
    
    default:
      break;
  }

  readMIDIforConfig();


}




void checkButtons(void)
{
  static uint32_t buttonholdstarttime = 0;
  button.update();
  if (button.rose())
  {
    if ((buttonholdstarttime + buttonholdtimeforsave) <= millis())   //  if button's been held long enough, save changes
    {
      currentConfigMode = CONFIG_MODE_save;
      Serial.println("CONFIG SAVED");
      buttonholdstarttime = 0;
    }

    else                                                             //  if buttons not been held, cycle menu
    {
      buttonholdstarttime = 0;
      currentConfigMode++;
      if (currentConfigMode >= CONFIG_MODE_total) currentConfigMode = CONFIG_MODE_filter1;
      Serial.print("configmode = ");
      Serial.println(currentConfigMode);
    }
    
  }

  if (button.fell())    //  start counting when button is held
  {
    buttonholdstarttime = millis();
  }
}