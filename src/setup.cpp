#include "setup.h"





void setupStuff()
{


  
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
}


//  checks for saved MIDI config. returns true and updates values if data exists, returns false if no data present
bool checkForSavedMIDIdata(MIDIconfigProfile* filter1, MIDIconfigProfile* filter2)
{
  if (EEPROM.read(savedDataExists))
  {
    restoreSettings(filter1, filter2);
    Serial.println("Saved MIDI config restored from EEPROM");
  }
}