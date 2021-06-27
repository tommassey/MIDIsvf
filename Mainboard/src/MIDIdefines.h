#ifndef MIDI_DEFINES_H
#define MIDI_DEFINES_H


enum CCresolution
{
    sevenBit = 0,
    fourteenBit
};

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

struct MIDIconfigProfile
{
    byte channel = 1;               //  MIDI channel
    byte CCforMSB = 0;                 //  CC# 7bit
    byte CCforLSB = 0;                 // 2nd CC# for 14bit MIDI
    bool resolution = sevenBit;
    byte incomingValueMSB = 0;  // incoming MIDI CC value
    byte incomingValueLSB = 0;  // incoming MIDI CC value
    uint16_t currentValue = 0;
    uint16_t minValue = 33000;       // lowest value so far
    uint16_t maxValue = 1;     // highest value so far
    bool initialised7bit = false;
    bool initialised14bit = false;
    float scaledIncrement = 0;
};




#endif