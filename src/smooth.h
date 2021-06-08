#ifndef SMOOTH_H
#define SMOOTH_H

#include <Arduino.h>



class Smoother
{
private:
    static const byte noVals = 8;
    byte index = 0;
    uint16_t value[noVals];// = {0};
    uint16_t average = 0;
    uint32_t total = 0;


public:
    Smoother();
    uint16_t smooth(uint16_t input);

};



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







#endif