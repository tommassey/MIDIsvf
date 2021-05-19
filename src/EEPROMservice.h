#ifndef EEPROMSERVICE_H
#define EEPROMSERVICE_H

#include <Arduino.h>
#include <EEPROM.h>
#include "MIDIsetup.h"



//Address can range from 0 to the EEPROM size minus 1. 128bytes max Teensy LC

typedef ROMlocation;


    ROMlocation channel1       = 1;
    ROMlocation resolution1    = 2;
    ROMlocation CCmsb1         = 3;
    ROMlocation CClsb1         = 4;
    ROMlocation valMin1        = 5;
    //
    ROMlocation valMax1        = 7;
    //
    ROMlocation channel2       = 9;
    ROMlocation resolution2    = 10;
    ROMlocation CCmsb2         = 11;
    ROMlocation CClsb2         = 12;
    ROMlocation valMin2        = 13;
    //
    ROMlocation valMax2        = 15;
    //







#endif