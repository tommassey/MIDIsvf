#ifndef POTSMOOTHING_H
#define POTSMOOTHING_H

#include <Arduino.h>


class pot
{
private:
    uint8_t pin;
    static const uint8_t numReadings = 10;
    uint16_t readings[numReadings] = {0};
    uint8_t readIndex;
    uint16_t total;
    uint16_t average;

    uint16_t* externalValue;
    bool* changeFlag;

    /* data */
public:
    pot(uint8_t _pin, uint16_t* _externalValue, bool* _changeFlag);
    ~pot();

    void read();    // read pin and add value to array, set changeFlag high
    void update();  //  called externally when *changeFlag is high, work out average and write the updated value to the external pointer location

};





#endif