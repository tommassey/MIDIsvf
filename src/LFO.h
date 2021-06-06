#ifndef LFO_H
#define LFO_H

#include <Arduino.h>
#include "DAC.h"


uint16_t getRatePotValue(void);
void updateLFO(void);

void initSineTable(void);
void initNMLtable();
void initLFOpointer(int16_t* value);

void setLFOrate(float rate);
void setLFOamount(float amount);




enum waveForms
{
    noWaveform = 0,
    sine,
    triangle,
    rampUp,
    rampDown,
    square,
    nonMusicLFO

};

#endif