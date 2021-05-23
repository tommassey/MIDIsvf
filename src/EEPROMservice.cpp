#include "EEPROMservice.h"


//Address can range from 0 to the EEPROM size minus 1. 128bytes max Teensy LC






void EEPROMwriteuint16(ROMlocation location, uint16_t input)
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


    

// void testEEPROM(void)
// {

//   digitalWrite(LED_BUILTIN, HIGH);
//   delay(5000);
//   digitalWrite(LED_BUILTIN, LOW);

//   testEEPROMread16(valMin1);
//   testEEPROMread16(valMin2);

//   digitalWrite(LED_BUILTIN, HIGH);
//   EEPROMwriteuint16(12000, valMin1);
//   EEPROMwriteuint16(28421, valMin2);
//   digitalWrite(LED_BUILTIN, LOW);

//   testEEPROMread16(valMin1);
//   testEEPROMread16(valMin2);


//   delay(1000);


//   testEEPROMread16(valMin1);
//   testEEPROMread16(valMin2);

//   EEPROMwriteuint16(13467, valMin1);
//   EEPROMwriteuint16(75, valMin2);

//   testEEPROMread16(valMin1);
//   testEEPROMread16(valMin2);



//   while(1)
//   {
//     digitalWrite(LED_BUILTIN, HIGH);
//     delay(200);
//     digitalWrite(LED_BUILTIN, LOW);
//     delay(200);

//   }
  

// }


void testEEPROMread16(ROMlocation loc)
{
  Serial.print("EEPROM read location :");
  Serial.print(loc);
  Serial.print("  value :");
  Serial.println(EEPROMreaduint16(loc));
 
        

}





