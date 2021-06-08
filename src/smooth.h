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








#endif