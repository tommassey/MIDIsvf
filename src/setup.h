#ifndef SETUP_H
#define SETUP_H

#include <Arduino.h>
#include "Bounce2.h"
#include "DAC.h"
#include "LED.h"




void setupStuff(void);
void initPins();
void MIDIconfigMode(void);

void checkButtons(void);




#endif
