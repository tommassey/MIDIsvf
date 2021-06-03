#include "MIDIservice.h"

static MIDIconfigProfile _filter1;
static MIDIconfigProfile _filter2;

bool scaling_init = false;

float bitf1 = 0;


void checkMIDI()
{
  CCevent newCC = readMIDI();


  switch (newCC.whichFilter)
  {
      case noFilter:
      {
        break;
      } 

      case Filter1:
      {
        Serial.print("filter1 input = ");
        Serial.println(newCC.value);
        uint16_t val = scaleForDAC(newCC.value, &_filter1);
        Serial.print("  scaled value = ");
        Serial.println(val);
        DACwriteChannelA(val);
        break;
      }

      case Filter2:
      {
        Serial.print("filter2 input = ");
        Serial.println(newCC.value);
        uint16_t val = scaleForDAC(newCC.value, &_filter2);
        Serial.print("  scaled value = ");
        Serial.println(val);
        DACwriteChannelB(val);
        break;
      }

      //default: //Serial.println("default");
      
  }

  if(!scaling_init)
  {
    initScaling();
    scaling_init = true;
  }
  
}



CCevent readMIDI()
{
  CCevent ccevent = {0,0};
  static uint16_t cutoff[totalFilters] = {0};
  static uint16_t oldCutoff[totalFilters] = {0};
  static uint8_t filterValues[totalFiltValues];


  if (usbMIDI.read())
  {             
    if (usbMIDI.getType() == usbMIDI.ControlChange)
    {
      byte newChannel = usbMIDI.getChannel();

      if (newChannel == _filter1.channel || _filter2.channel)
      {
        byte data1 = usbMIDI.getData1();
        byte data2 = usbMIDI.getData2();


        if ((data1 == _filter1.CCforMSB) && (newChannel == _filter1.channel))   //  ================================   FILTER 1 MSB
        {
          filterValues[filt1MSB] = data2;

          if (_filter1.resolution == sevenBit)
          {
            ccevent = {Filter1, data2};
            printCC(ccevent.whichFilter, ccevent.value);
            return ccevent;
          }
        }

        if ((data1 == _filter1.CCforLSB) && (newChannel == _filter1.channel))   //  ================================   FILTER 1 LSB
        {
          filterValues[filt1LSB] = data2;
          uint16_t sixteenBitNumber = bitShiftCombine(filterValues[filt1MSB], filterValues[filt1LSB]);
          
          if (sixteenBitNumber != oldCutoff[Filter1])
          {
            oldCutoff[Filter1] = cutoff[Filter1];
            cutoff[Filter1] = sixteenBitNumber;

            ccevent = {Filter1, cutoff[Filter1]};
            printCC(ccevent.whichFilter, ccevent.value);
            return ccevent;
          }
        }

        if ((data1 == _filter2.CCforMSB) && (newChannel == _filter2.channel))   //  ================================   FILTER 2 MSB
        {
          filterValues[filt2MSB] = data2;

          if (_filter2.resolution == sevenBit)
          {
            ccevent = {Filter2, data2};
            printCC(ccevent.whichFilter, ccevent.value);
            return ccevent;
          }
        }

        if ((data1 == _filter2.CCforMSB) && (newChannel == _filter2.channel))  // ==================================   FILTER 2 LSB
        {
          filterValues[filt2LSB] = data2;
          uint16_t sixteenBitNumber = bitShiftCombine(filterValues[filt2MSB], filterValues[filt2LSB]);
        
          if (sixteenBitNumber != oldCutoff[Filter2])
          {
            oldCutoff[Filter2] = cutoff[Filter2];
            cutoff[Filter2] = sixteenBitNumber;

            ccevent = {Filter2, cutoff[Filter2]};
            printCC(ccevent.whichFilter, ccevent.value);
            return ccevent;
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


// min 20  max 90  so  70 total  58.5mV per point


void initScaling()
{
    float window = _filter1.maxValue - _filter1.minValue;
    _filter1.scaledIncrement = ((float)twelvebit / window);

    window = _filter2.maxValue - _filter2.minValue;
    _filter2.scaledIncrement = ((float)twelvebit / window);
}

uint16_t scaleForDAC(uint16_t data, MIDIconfigProfile* filter)
{
  if (data < filter->minValue) data = filter->minValue;  // clamping
  if (data > filter->maxValue) data = filter->maxValue;

  Serial.print("scaled increment  = ");
  Serial.println(filter->scaledIncrement);
 
  data = data - filter->minValue;  // minus offset
  data = data * filter->scaledIncrement;  // multiply to final value
  //if (data > 4095) data = 4095;  // don't need this, clamped in next function

  return data;
}