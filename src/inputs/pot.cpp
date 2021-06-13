#include "pot.h"


pot::pot(uint8_t _pin, float* _externalValue, bool* _changeFlag, potMode _mode)
{
    pin = _pin;
    externalValue = _externalValue;
    changeFlag = _changeFlag;
    mode = _mode;
}


void pot::timeToRead()
{
  timeToReadAvalue = true;
}


void pot::update()  //  checks if timeToReadAvalue is high, work out average and write the updated value to the external pointer location
{
    if (timeToReadAvalue)  // if the read ADC isr has been called
    {
        readings[readIndex] = analogRead(pin);    // read from the sensor
        total = total + readings[readIndex];      // add the reading to the total
        readIndex++;                              // advance to the next position in the array
        if (readIndex >= numReadings) readIndex = 0;
        newAverage = total / numReadings;
        
        if ((newAverage > average + chatterWindow) || (newAverage < average - chatterWindow))  //  to remove input noise
        {       
            *externalValue = finalOffsetScale(deadZoneScale(newAverage));  // update final float value
            average = newAverage;
            Serial.print("pot value = ");
            Serial.println(*externalValue);
            *changeFlag = true;                          // set external flag high 
        }
   
        total = total - readings[readIndex];      // subtract the last reading ready for the next read
        timeToReadAvalue = false;
    }
}
    
float pot::deadZoneScale(uint16_t value)     //  create a small dead area at top and bottom of pot travel
{
    if (value > topLimit) value = topLimit;
    if (value < bottomLimit) value = bottomLimit;

    value = (value - bottomLimit);
    float newFloat = ( 1.0 / 
                     (((float)ADCresolution - (2 * deadZone)) / (float)value));

    return newFloat;
}

float pot::finalOffsetScale(float input)      //  scales pot value for bipolar / unipolar etc.
{
    if (mode == potmode_bipolar)
    {
        float scaled = (input * 2) - 1.0;
        return scaled;
    }
    else
    {
        return input;
    }    
}