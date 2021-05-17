#ifndef MIDISETUP_H
#define MIDISETUP_H

#include <Arduino.h>
//#include "MIDIUSB.h"
#include "pinDefines.h"
#include "LED.h"
#include "button.h"

#define Filter1CCmsb 31
#define Filter1CClsb 63

#define Filter2CCmsb 32
#define Filter2CClsb 64

#define twelvebit 4095





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


enum configModes
{
    CONFIG_MODE_start = 0,
    CONFIG_MODE_filter1,
    CONFIG_MODE_filter2,
    CONFIG_MODE_total,
    CONFIG_MODE_save = 10,
    CONFIG_MODE_reset
};



void printCC(int cc, int val);

uint16_t bitShiftCombine16(uint8_t x_high, uint8_t x_low);


void readMIDIforConfig(MIDIconfigValue* configToChange);
bool newCCswitch(byte cc, byte val, MIDIconfigValue* filter);


static byte currentConfigMode = CONFIG_MODE_start;

void initMIDIconfig(LED* led, Bounce* btn);
void MIDIconfigMode();

void menuService(Bounce* btn);




#endif
