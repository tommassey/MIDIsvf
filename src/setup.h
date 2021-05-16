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


enum configModes
{
    CONFIG_MODE_init = 0,
    CONFIG_MODE_filter1,
    CONFIG_MODE_filter2,
    CONFIG_MODE_complete,
    CONFIG_MODE_total
};




#endif
