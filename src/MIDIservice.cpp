#include "MIDIservice.h"

MIDIconfigProfile _filter1;
MIDIconfigProfile _filter2;


void checkMIDI()
{
  CCevent newCC = readMIDI();


  switch (newCC.whichFilter)
  {
      case noFilter:
      {
        //Serial.println("no change");
        break;
      } 

      case Filter1:
      {
        //Serial.println("filter1 change");
        DACwriteChannelA(newCC.value);
        break;
      }

      case Filter2:
      {
        //Serial.println("filter2 change");
        DACwriteChannelB(newCC.value);
        break;
      }

      default: Serial.println("default");
      
  }

  //DACtest();
  //delay(3);
  
}



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
            uint16_t sixteenBitNumber = bitShiftCombine(filterValues[filt1MSB], filterValues[filt1LSB]);
            uint16_t twelveBitNumber = sixteenBitNumber>>3;
            if (twelveBitNumber != oldCutoff[Filter1])
            {
              oldCutoff[Filter1] = cutoff[Filter1];
              cutoff[Filter1] = twelveBitNumber;

              ccevent = {Filter1, cutoff[Filter1]};
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
            uint16_t sixteenBitNumber = bitShiftCombine(filterValues[filt2MSB], filterValues[filt2LSB]);
            uint16_t twelveBitNumber = sixteenBitNumber>>3;
            if (twelveBitNumber != oldCutoff[Filter2])
            {
              oldCutoff[Filter2] = cutoff[Filter2];
              cutoff[Filter2] = twelveBitNumber;

              ccevent = {Filter2, cutoff[Filter2]};
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





uint16_t bitShiftCombine( uint8_t x_high, uint8_t x_low)
{
  uint16_t combined;
  combined = x_high;              //send x_high to rightmost 8 bits
  combined = combined<<8;         //shift x_high over to leftmost 8 bits
  combined |= x_low;              //logical OR keeps x_high intact in combined and fills in rightmost 8 bits
  return combined;
}


void setMIDIprofiles(MIDIconfigProfile* f1, MIDIconfigProfile* f2)
{
  _filter1 = *f1;
  _filter2 = *f2;

  Serial.println("== Filter 1 ===========");
  Serial.print("CH: ");
  Serial.print(_filter1.channel);
  Serial.print("   RES: ");
  Serial.print(_filter1.resolution);
  Serial.print("   CC: ");
  Serial.print(_filter1.CCforMSB);
  Serial.print("   MIN: ");
  Serial.print(_filter1.minValue);
  Serial.print("   MAX: ");
  Serial.println(_filter1.maxValue);

  Serial.println("== Filter 2 ===========");
  Serial.print("CH: ");
  Serial.print(_filter2.channel);
  Serial.print("   RES: ");
  Serial.print(_filter2.resolution);
  Serial.print("   CC: ");
  Serial.print(_filter2.CCforMSB);
  Serial.print("   MIN: ");
  Serial.print(_filter2.minValue);
  Serial.print("   MAX: ");
  Serial.println(_filter2.maxValue);


}