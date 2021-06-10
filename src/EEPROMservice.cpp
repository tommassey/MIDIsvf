#include "EEPROMservice.h"

//Address can range from 0 to the EEPROM size minus 1. 128bytes max Teensy LC, 1024bytes Teensy 4.0


void EEPROMwriteuint16(ROMlocation location, uint16_t input)
{
  byte bytes[2] =   //  little endian
  {
    (input >> 0) & 0xFF,
    (input >> 8) & 0xFF,
  };

  EEPROM.write(location, bytes[0]);  // write bytes
  EEPROM.write(location + 1, bytes[1]);
}


uint16_t EEPROMreaduint16(ROMlocation location)
{
  byte firstByte = EEPROM.read(location);
  byte secondByte = EEPROM.read(location + 1);

  return bitShiftCombine16(secondByte, firstByte);
}


void testEEPROMread16(ROMlocation loc)
{
  Serial.print("EEPROM read location :");
  Serial.print(loc);
  Serial.print("  value :");
  Serial.println(EEPROMreaduint16(loc));
}


static uint16_t bitShiftCombine16( byte x_high, byte x_low)
{
  uint16_t highpart = ((uint16_t)x_high << 8);
  uint16_t combined = highpart | x_low;
  return combined;
}