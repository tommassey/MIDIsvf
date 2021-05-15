#include "MIDIservice.h"

CCevent readMIDI()
{
  CCevent ccevent = {0,0};
  static uint16_t cutoff[totalFilters] = {0};
  static uint16_t oldCutoff[totalFilters] = {0};

  static uint8_t filterValues[totalFiltValues];
  static uint16_t sixteenBitNumber;
  static uint16_t twelveBitNumber;
  static bool gotBothBytes[totalFilters] = {false};

  if (usbMIDI.read())
  {             
    switch (usbMIDI.getType())
    {
      case usbMIDI.ControlChange:
      {
         //digitalWrite(LED_BUILTIN, HIGH);

        byte gotData1 = usbMIDI.getData1();
        switch (gotData1)
        {
          case Filter1CCmsb: filterValues[filt1MSB] = usbMIDI.getData2();

          case Filter1CClsb:
          {
            filterValues[filt1LSB] = usbMIDI.getData2();
            gotBothBytes[filter1] = true;
          }

          case Filter2CCmsb: filterValues[filt2MSB] = usbMIDI.getData2();

          case Filter2CClsb:
          {
            filterValues[filt2LSB] = usbMIDI.getData2();
            gotBothBytes[filter2] = true;
          }
        }
      }
    }
  }

  if (gotBothBytes[filter1])
         {
          sixteenBitNumber = BitShiftCombine(filterValues[filt1MSB], filterValues[filt1LSB]);
          twelveBitNumber = sixteenBitNumber>>3;
          if (twelveBitNumber != oldCutoff[filter1])
          {
            oldCutoff[filter1] = cutoff[filter1];
            cutoff[filter1] = twelveBitNumber;
            gotBothBytes[filter1] = false;
            ccevent = {filter1, cutoff[filter1]};

            Serial.print("cc event    channel ");
            Serial.print(ccevent.whichCC);
            Serial.print("    value ");
            Serial.println(ccevent.value);

            return ccevent;
          }

          
        }

    if (gotBothBytes[filter2])
         {    
          sixteenBitNumber = BitShiftCombine(filterValues[filt2MSB], filterValues[filt2LSB]);
          twelveBitNumber = sixteenBitNumber>>3;
          if (twelveBitNumber != oldCutoff[filter2])
          {
            oldCutoff[filter2] = cutoff[filter2];
            cutoff[filter2] = twelveBitNumber;
            gotBothBytes[filter2] = false;
            ccevent = {filter2, cutoff[filter2]};

            Serial.print("cc event    channel ");
            Serial.print(ccevent.whichCC);
            Serial.print("    value ");
            Serial.println(ccevent.value);

            return ccevent;
          }
        }

  return ccevent;
}








uint16_t BitShiftCombine( uint8_t x_high, uint8_t x_low)
{
  uint16_t combined;
  combined = x_high;              //send x_high to rightmost 8 bits
  combined = combined<<8;         //shift x_high over to leftmost 8 bits
  combined |= x_low;              //logical OR keeps x_high intact in combined and fills in rightmost 8 bits
  return combined;
}