#include "setup.h"





void setupStuff()
{
  Serial.begin(115200);
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








// void buttonService(Bounce* btn)
// {




//   static uint32_t buttonholdstarttime = 0;
//   button.update();
//   if (button.rose())
//   {
//     if ((buttonholdstarttime + buttonholdtimeforsave) <= millis())   //  if button's been held long enough, save changes
//     {
//       currentConfigMode = CONFIG_MODE_save;
//       Serial.println("CONFIG SAVED");
//       buttonholdstarttime = 0;
//     }

//     else                                                             //  if buttons not been held, cycle menu
//     {
//       buttonholdstarttime = 0;
//       currentConfigMode++;
//       if (currentConfigMode >= CONFIG_MODE_total) currentConfigMode = CONFIG_MODE_filter1;
//       Serial.print("configmode = ");
//       Serial.println(currentConfigMode);
//     }
    
//   }

//   if (button.fell())    //  start counting when button is held
//   {
//     buttonholdstarttime = millis();
//   }
// }