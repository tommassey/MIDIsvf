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
    noFilter = 0,
    Filter1,
    Filter2,
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
    byte whichFilter = noFilter;
    uint16_t value = 0;
};

enum CCresolution
{
    sevenBit = 0,
    fourteenBit
};

struct MIDIconfigValue
{
    byte channel = 1;               //  MIDI channel
    byte CCforMSB = 0;                 //  CC# 7bit
    byte CCforLSB = 0;                 // 2nd CC# for 14bit MIDI
    bool resolution = sevenBit;
    byte incomingValueMSB = 0;  // incoming MIDI CC value
    byte incomingValueLSB = 0;  // incoming MIDI CC value
    uint16_t currentValue = 0;
    uint16_t minValue = 0;       // lowest value so far
    uint16_t maxValue = 127;     // highest value so far
    bool initialised7bit = false;
    bool initialised14bit = false;


};

enum ConfigStateMachine
{

};


void checkMIDI(void);
CCevent readMIDI(void);
uint16_t getMIDIvalue(void);



void printCC(int cc, int val);

uint16_t bitShiftCombine(uint8_t x_high, uint8_t x_low);
uint16_t bitShiftCombine16(uint8_t x_high, uint8_t x_low);


void readMIDIforConfig(void);
void newCCswitch(byte cc, byte val);




#endif
