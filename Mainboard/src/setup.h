#ifndef SETUP_H
#define SETUP_H

#include <Arduino.h>
#include "DACservice.h"
#include "LED.h"
#include "pinDefines.h"
#include "button.h"
#include "EEPROMservice.h"


void setupStuff(void);
void initPins(Bounce* btn);


#endif
