#include "DACservice.h"
#include "MIDIdefines.h"



uint16_t scaleForDAC(uint16_t data, MIDIconfigProfile value)
{
  if (data < value.minValue) data = value.minValue;  // clamping
  if (data > value.maxValue) data = value.maxValue;

  //Serial.print("scaled increment  = ");
  //Serial.println(filter->scaledIncrement);
 
  data = data - value.minValue;  // minus offset
  data = data * value.scaledIncrement;  // multiply to final value
  //if (data > 4095) data = 4095;  // don't need this, clamped in next function

  return data;
}