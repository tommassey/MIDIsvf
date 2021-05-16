#include "MIDIservice.h"
#include "DAC.h"

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







byte currentMIDIconfigState = 0;
byte midiConfigValues[totalFiltValues] = {"\0"};
static MIDIconfigValue filter1;
static MIDIconfigValue filter2;

void readMIDIforConfig(void)
{
  if (usbMIDI.read())
  {
    switch (usbMIDI.getType())
      {
        case usbMIDI.ControlChange:
        {
          byte newCC    = usbMIDI.getData1();
          byte newValue = usbMIDI.getData2();
          byte channel  = usbMIDI.getChannel();

          Serial.print("Got CC.  Channel = ");
          Serial.print(channel);
          Serial.print("  CC# = ");
          Serial.print(newCC);
          Serial.print("  value = ");
          Serial.println(newValue);

          newCCswitch(newCC, newValue);
        }
      }
  }
}  // end of function 




void newCCswitch(byte cc, byte val)
{

  if (cc > 119)
  {
    Serial.println("excluded CC 120+");
    return;
  }

  switch (cc)
  {
    case 0:
    {
      Serial.println("excluded CC - Bank select");
      return;
    }
    case 6:
    {
      Serial.println("excluded CC - Data Entry");
      return;
    }
    case 7:
    {
      Serial.println("excluded CC - Volume");
      return;
    }
    case 10:
    {
      Serial.println("excluded CC - Pan position");
      return;
    }
    case 98:
    {
      Serial.println("excluded CC - Non-Registered param LSB");
      return;
    }
    case 99:
    {
      Serial.println("excluded CC - Non-Registered param MSB");
      return;
    }
    case 100:
    {
      Serial.println("excluded CC - Registered param LSB");
      return;
    }
    case 101:
    {
      Serial.println("excluded CC - Registered param MSB");
      return;
    }
  }


  
  //=========================================================================================== 7bit mode  
  if (!filter1.initialised7bit)  // if it's the first time, set incoming CC# to Filter1CCMSB
  {
    filter1.CCforMSB = cc;
    filter1.incomingValueMSB = val;
    filter1.initialised7bit = true;
    filter1.resolution = sevenBit;

    Serial.print("init filter1 7bit MSB CC = ");
    Serial.print(filter1.CCforMSB);
    Serial.print("  value = ");
    Serial.println(filter1.incomingValueMSB);

    return;
  }
  else if (filter1.initialised7bit && (cc == filter1.CCforMSB))   //  if we're in 7bit mode and we got the right CC#, update value
  {
    Serial.println("we're in 7bit mode and we got the right CC#, update value");
    filter1.incomingValueMSB = val;
    return;
  }

  //=========================================================================================== 14bit mode  
  else if (cc == (filter1.CCforMSB + 32))  // check if we're reciving 14bit MIDI
  {
    Serial.println("CC says 14bit");

    if (!filter1.initialised14bit)   //  if it's first 14bit msg
    {
      filter1.CCforLSB = cc;
      filter1.incomingValueLSB = val;
      filter1.initialised14bit = true;
      filter1.resolution = fourteenBit;
      filter1.currentValue = bitShiftCombine16(filter1.incomingValueMSB, filter1.incomingValueLSB);

      Serial.print("init filter 1 14bit input LSB CC = ");
      Serial.print(filter1.incomingValueLSB);
      Serial.print("  MSB CC = ");
      Serial.print(filter1.CCforMSB);
      Serial.print("  value = ");
      Serial.println(filter1.currentValue);
      
      return;
    }
  
    else if (filter1.resolution == fourteenBit) // if we're init'd for 14bit and the CC# is the right one for the 14bit's LSB
    {
      Serial.print("14bit MIDI event  ");
      filter1.incomingValueLSB = val;
      filter1.currentValue = bitShiftCombine16(filter1.incomingValueMSB, filter1.incomingValueLSB);
      Serial.print("  CC = ");
      Serial.print(filter1.CCforMSB);
      Serial.print("  value = ");
      Serial.println(filter1.currentValue);

      return;
    }   
  
  }  // end 14b


}




uint16_t bitShiftCombine16( byte x_high, byte x_low)
{
  uint16_t highpart = ((uint16_t)x_high << 8);
  uint16_t combined = highpart | x_low;
  return combined;
}


uint16_t bitShiftCombine( uint8_t x_high, uint8_t x_low)
{
  uint16_t combined;
  combined = x_high;              //send x_high to rightmost 8 bits
  combined = combined<<8;         //shift x_high over to leftmost 8 bits
  combined |= x_low;              //logical OR keeps x_high intact in combined and fills in rightmost 8 bits
  return combined;
}