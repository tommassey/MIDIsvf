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
        //Serial.println("no change");
        break;
      } 

      case Filter1:
      {
        Serial.print("filter1 input = ");
        Serial.println(newCC.value);
        uint16_t val = scaleForDAC(newCC.value, &_filter1);
        Serial.print("  scaled value = ");
        Serial.println(val);
        DACwriteChannelA(newCC.value);
        break;
      }

      case Filter2:
      {
        Serial.print("filter2 input = ");
        Serial.println(newCC.value);
        uint16_t val = scaleForDAC(newCC.value, &_filter2);
        Serial.print("  scaled value = ");
        Serial.println(val);
        DACwriteChannelB(newCC.value);
        break;
      }

      default: Serial.println("default");
      
  }

  //DACtest();
  //delay(3);

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

        if (data1 == _filter1.CCforMSB)   //  ================================   FILTER 1 MSB
        {
          filterValues[filt1MSB] = data2; 
        }

        if (data1 == _filter1.CCforLSB)   //  ================================   FILTER 1 LSB
        {
          filterValues[filt1LSB] = data2;
          uint16_t sixteenBitNumber = bitShiftCombine(filterValues[filt1MSB], filterValues[filt1LSB]);
          uint16_t twelveBitNumber = sixteenBitNumber>>3;
          
          //if (twelveBitNumber > _filter1.maxValue) twelveBitNumber = _filter1.maxValue;  // clamping
          //if (twelveBitNumber < _filter1.minValue) twelveBitNumber = _filter1.minValue;

          if (sixteenBitNumber != oldCutoff[Filter1])
          {
            oldCutoff[Filter1] = cutoff[Filter1];
            cutoff[Filter1] = sixteenBitNumber;
            ccevent = {Filter1, cutoff[Filter1]};
            printCC(ccevent.whichFilter, ccevent.value);
            return ccevent;
          }
        }

        if (data1 == _filter2.CCforMSB)   //  ================================   FILTER 2 MSB
          {
            filterValues[filt2MSB] = data2; 
          }

          if (data1 == _filter2.CCforMSB)  // ==================================   FILTER 2 LSB
          {
            filterValues[filt2LSB] = data2;
            uint16_t sixteenBitNumber = bitShiftCombine(filterValues[filt2MSB], filterValues[filt2LSB]);
            uint16_t twelveBitNumber = sixteenBitNumber>>3;
            
            //if (twelveBitNumber > _filter2.maxValue) twelveBitNumber = _filter2.maxValue;
            //if (twelveBitNumber < _filter2.minValue) twelveBitNumber = _filter2.minValue;

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
  if(_filter1.resolution == fourteenBit)
  { 
    float window = _filter1.maxValue - _filter1.minValue;
    _filter1.scaledIncrement = ((float)twelvebit / window);
  }
  else if (_filter1.resolution == sevenBit)
  {
    float window = _filter1.maxValue - _filter1.minValue;
    _filter1.scaledIncrement = ((float)127 / window);
  }

  // Serial.print("min filter1 = ");
  // Serial.println(_filter1.minValue);
  // Serial.print("max filter1 = ");
  // Serial.println(_filter1.maxValue);
  // Serial.print("scaled increment filter1 = ");
  // Serial.println(_filter1.scaledIncrement);


  if(_filter2.resolution == fourteenBit)
  {
    float window = _filter2.maxValue - _filter2.minValue;
    _filter2.scaledIncrement = ((float)twelvebit / window);
  }
  else if (_filter2.resolution == sevenBit)
  {
    float window = _filter2.maxValue - _filter2.minValue;
    _filter2.scaledIncrement = ((float)127 / window);  }

  // Serial.print("scaled increment filter2 = ");
  // Serial.println(_filter2.scaledIncrement);
}

uint16_t scaleForDAC(uint16_t data, MIDIconfigProfile* filter)
{
  if (data < filter->minValue) data = filter->minValue;  // clamping
  if (data > filter->maxValue) data = filter->maxValue;

  Serial.print("scaled increment  = ");
  Serial.println(filter->scaledIncrement);
 
  data = data - filter->minValue;  // minus offset
  data = data * filter->scaledIncrement;  // multiply to final value

  


  //Serial.print("data scaled = ");
  //Serial.println(data);

  return data;

}