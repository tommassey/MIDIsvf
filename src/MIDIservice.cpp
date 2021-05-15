#include "MIDIservice.h"

CCevent readMIDI()
{
  CCevent ccevent = {0,0};
  static uint16_t cutoff[totalFilters] = {0};
  static uint16_t oldCutoff[totalFilters] = {0};

  static uint8_t filterValues[totalFiltValues];

  if (usbMIDI.read())
  {             
    switch (usbMIDI.getType())
    {
      case usbMIDI.ControlChange:
      {
        byte data1 = usbMIDI.getData1();
        //Serial.print("data1 = ");
        //Serial.print(data1);

        byte data2 = usbMIDI.getData2();
        //Serial.print("      data2 = ");
        //Serial.println(data2);

        switch (data1)
        {
          case Filter1CCmsb:
          {
            filterValues[filt1MSB] = data2;
            break;
          }

          case Filter1CClsb:
          {
            filterValues[filt1LSB] = data2;
            uint16_t sixteenBitNumber = BitShiftCombine(filterValues[filt1MSB], filterValues[filt1LSB]);
            uint16_t twelveBitNumber = sixteenBitNumber>>3;
            if (twelveBitNumber != oldCutoff[filter1])
            {
              oldCutoff[filter1] = cutoff[filter1];
              cutoff[filter1] = twelveBitNumber;

              ccevent = {filter1, cutoff[filter1]};
              //printCC(ccevent.whichCC, ccevent.value);
              return ccevent;
            }
          }

          case Filter2CCmsb:
          {
            filterValues[filt2MSB] = data2;
            break;
          }

          case Filter2CClsb:
          {
            filterValues[filt2LSB] = data2;
            uint16_t sixteenBitNumber = BitShiftCombine(filterValues[filt2MSB], filterValues[filt2LSB]);
            uint16_t twelveBitNumber = sixteenBitNumber>>3;
            if (twelveBitNumber != oldCutoff[filter2])
            {
              oldCutoff[filter2] = cutoff[filter2];
              cutoff[filter2] = twelveBitNumber;

              ccevent = {filter2, cutoff[filter2]};
              //printCC(ccevent.whichCC, ccevent.value);
              return ccevent;
            }
          }
        }
      }
    }
  }
  return ccevent;
}

void printCC(int cc, int val)
{
  Serial.print("cc event    channel ");
  Serial.print(cc);
  Serial.print("    value ");
  Serial.println(val);
}







uint16_t BitShiftCombine( uint8_t x_high, uint8_t x_low)
{
  uint16_t combined;
  combined = x_high;              //send x_high to rightmost 8 bits
  combined = combined<<8;         //shift x_high over to leftmost 8 bits
  combined |= x_low;              //logical OR keeps x_high intact in combined and fills in rightmost 8 bits
  return combined;
}