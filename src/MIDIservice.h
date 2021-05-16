#ifndef MIDISERVICE_H
#define MIDISERVICE_H

#include <Arduino.h>


#define Filter1CCmsb 31
#define Filter1CClsb 63

#define Filter2CCmsb 32
#define Filter2CClsb 64

#define twelvebit 4095

enum filters
{
    nofilter = 0,
    filter1,
    filter2,
    totalFilters
};

enum filterBytes
{
    filt1LSB = 0,
    filt1MSB,
    filt2LSB,
    filt2MSB,
    totalFiltValues
};

struct CCevent
{
    byte whichCC = nofilter;
    uint16_t value = 0;
};



void checkMIDI(void);
CCevent readMIDI(void);
uint16_t getMIDIvalue(void);

void printCC(int cc, int val);

uint16_t BitShiftCombine(uint8_t x_high, uint8_t x_low);






#endif
