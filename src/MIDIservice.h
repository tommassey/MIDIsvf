#ifndef MIDISERVICE_H
#define MIDISERVICE_H

#include <Arduino.h>
#include "DAC.h"
#include "pinDefines.h"
#include "MIDIsetup.h"







struct CCevent
{
    byte whichFilter = noFilter;
    uint16_t value = 0;
};






void checkMIDI(void);
CCevent readMIDI(void);
uint16_t getMIDIvalue(void);



void printCC(int cc, int val);

uint16_t bitShiftCombine(uint8_t x_high, uint8_t x_low);
uint16_t bitShiftCombine16(uint8_t x_high, uint8_t x_low);


bool newCCswitch(byte cc, byte val);

void setMIDIprofiles(MIDIconfigProfile* f1, MIDIconfigProfile* f2);

void initScaling(void);
uint16_t scaleForDAC(uint16_t data, MIDIconfigProfile* filter);




#endif
