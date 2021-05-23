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

  return bitShiftCombine16(secondByte, firstByte);
}


    // ROMlocation channel1       = 1;
    // ROMlocation resolution1    = 2;
    // ROMlocation CCmsb1         = 3;
    // ROMlocation CClsb1         = 4;
    // ROMlocation valMin1        = 5;
    // //
    // ROMlocation valMax1        = 7;
    // //
    // ROMlocation channel2       = 9;
    // ROMlocation resolution2    = 10;
    // ROMlocation CCmsb2         = 11;
    // ROMlocation CClsb2         = 12;
    // ROMlocation valMin2        = 13;
    // //
    // ROMlocation valMax2        = 15;

void testEEPROM(void)
{

  digitalWrite(LED_BUILTIN, HIGH);
  delay(5000);
  digitalWrite(LED_BUILTIN, LOW);

  testEEPROMread16(valMin1);
  testEEPROMread16(valMin2);

  digitalWrite(LED_BUILTIN, HIGH);
  EEPROMwriteuint16(12000, valMin1);
  EEPROMwriteuint16(28421, valMin2);
  digitalWrite(LED_BUILTIN, LOW);

  testEEPROMread16(valMin1);
  testEEPROMread16(valMin2);


  delay(1000);


  testEEPROMread16(valMin1);
  testEEPROMread16(valMin2);

  EEPROMwriteuint16(13467, valMin1);
  EEPROMwriteuint16(75, valMin2);

  testEEPROMread16(valMin1);
  testEEPROMread16(valMin2);



  while(1)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);

  }
  

}


void testEEPROMread16(ROMlocation loc)
{
  Serial.print("EEPROM read location :");
  Serial.print(loc);
  Serial.print("  value :");
  Serial.println(EEPROMreaduint16(loc));
 
        

}





