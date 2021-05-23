#ifndef EEPROMSERVICE_H
#define EEPROMSERVICE_H

#include <Arduino.h>
#include <EEPROM.h>
#include "MIDIsetup.h"



//Address can range from 0 to the EEPROM size minus 1. 128bytes max Teensy LC

enum ROMlocation
{
    channel1       = 1,
    resolution1    = 2,
    CCmsb1         = 3,
    CClsb1         = 4,
    valMin1        = 5,
    //
    valMax1        = 7,
    //
    channel2       = 9,
    resolution2    = 10,
    CCmsb2         = 11,
    CClsb2         = 12,
    valMin2        = 13,
    //
    valMax2        = 15
    //
};


    


void EEPROMwriteuint16(uint16_t input, ROMlocation location);
uint16_t EEPROMreaduint16(ROMlocation location);

void testEEPROMread16(ROMlocation loc);

void testEEPROM(void);






#endif