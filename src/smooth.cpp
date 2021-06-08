#include "smooth.h"

Smoother::Smoother()
{
    
}

uint16_t Smoother::smooth(uint16_t input)
{
    total = total - value[index];      // subtract the last reading ready for the next read
    value[index] = input;    // read from the sensor
    total = total + value[index];      // add the reading to the total
    index++;                // advance to the next position in the array
    if (index >= (noVals-1)) index = 0;
    uint16_t newAverage = total / noVals;
     
    return newAverage;
}


