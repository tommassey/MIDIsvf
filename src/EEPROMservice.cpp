#include "EEPROMservice.h"


//Address can range from 0 to the EEPROM size minus 1. 128bytes max Teensy LC


void EEPROMwriteuint16(uint16_t input, ROMlocation location)
{
  byte bytes[2] =   //  little endian
  {
    (input >>  0) & 0xFF,
    (input >>  8) & 0xFF,
  };

  EEPROM.write(location, bytes[0]);  // write bytes
  EEPROM.write(location + 1, bytes[1]);

}


uint16_t EEPROMreaduint16(ROMlocation location)
{
  byte firstByte = EEPROM.read(location);
  byte secondByte = EEPROM.read(location + 1);

  return bitShiftCombine16(firstByte, secondByte);
}


//  MIDIconfigValue

//     byte channel = 1;               //  MIDI channel
//     byte CCforMSB = 0;                 //  CC# 7bit
//     byte CCforLSB = 0;                 // 2nd CC# for 14bit MIDI
//     bool resolution = sevenBit;
//     byte incomingValueMSB = 0;  // incoming MIDI CC value
//     byte incomingValueLSB = 0;  // incoming MIDI CC value
//     uint16_t currentValue = 0;
//     uint16_t minValue = 33000;       // lowest value so far
//     uint16_t maxValue = 1;     // highest value so far
//     bool initialised7bit = false;
//     bool initialised14bit = false;






