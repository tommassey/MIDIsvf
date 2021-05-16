#include "setup.h"
#include "pinDefines.h"

Bounce button = Bounce();
LED led;
LED* leddy = &led;


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
    button.interval(25);
  }
}

byte currentConfigMode = CONFIG_MODE_init;

void MIDIconfigMode()
{
  updateLED(leddy);
  checkButtons();

  switch (currentConfigMode)
  {
    case CONFIG_MODE_init:
    {
      leddy->setBlinkProfile(init);
      break;
    }
    case CONFIG_MODE_filter1:
    {
     leddy->setBlinkProfile(f1config);

      break;
    }
    case CONFIG_MODE_filter2:
    {
      leddy->setBlinkProfile(f2config);
      break;
    }
    case CONFIG_MODE_complete:
    {
      leddy->setBlinkProfile(init);
      break;
    }
    case CONFIG_MODE_save:
    {
      leddy->setBlinkProfile(saved);
      break;
    }
    
    default:
      break;
  }


}




void checkButtons(void)
{
  static long buttonholdstarttime = 0;
  button.update();
  if (button.rose())
  {
    if ((buttonholdstarttime + buttonholdtimeforsave) <= millis())
    {
      currentConfigMode = CONFIG_MODE_save;
      Serial.println("CONFIG SAVED");
      buttonholdstarttime = 0;
    }

    else
    {
      buttonholdstarttime = 0;
      currentConfigMode++;
      if (currentConfigMode >= CONFIG_MODE_total) currentConfigMode = 0;
      Serial.print("configmode = ");
      Serial.println(currentConfigMode);
    }
    
  }

  if (button.fell())
  {
    buttonholdstarttime = millis();
  }
}