#include "setup.h"



void setupStuff()
{
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

//analogWriteResolution(12);
//analogReference(0); //Zero for default/ ext ref gives 3.3v pp

  usbMIDI.begin();
  Serial.begin(115200);


  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
}