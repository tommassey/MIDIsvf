#ifndef SETUP_H
#define SETUP_H

#include <Arduino.h>
#include <avr/wdt.h>
#include "DAC.h"
#include "LED.h"
#include "pinDefines.h"
#include "MIDIservice.h"
#include "button.h"
#include "EEPROMservice.h"


void setupStuff(void);
void initPins(Bounce* btn);




#endif
