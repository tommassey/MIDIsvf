#ifndef POT_H
#define POT_H

#include <Arduino.h>

typedef byte potMode;

enum potModes
{
    potmode_unipolar = 0,
    potmode_bipolar = 1
};


class pot
{
private:
    //============================================= Setup
    uint8_t pin;                              //  analog pin the pot is connected to
    uint16_t ADCresolution = 1024;            //  resolution of the ADC pin
    bool potIsBipolar = false;                //  false = 0 to 1.0, true = -1.0 to 1.0
    potMode mode = potmode_unipolar;
    
    static const uint8_t chatterWindow = 2;   //  amount pot has to move before it counts (1024 = full rotation)
    static const uint8_t deadZone = 8;       //  area at ends of pot travel that does nothing (1024 = full rotation)
    static const uint16_t topLimit = (1024 - deadZone);
    static const uint16_t bottomLimit = deadZone;


    //=============================================  Smoothing
    static const uint8_t numReadings = 16;    //  number of averages to take, higher is smoother at expense of memory
    uint16_t readings[numReadings] = {0};
    uint8_t readIndex;
    uint16_t total;
    uint16_t newAverage;
    uint16_t average;

    //=============================================  Flags and pointers
    bool timeToReadAvalue = false;            //  set high via timeToRead() when it's time to update

    float* externalValue;                     //  pointer to external float that gets updated with final pot value
    bool* changeFlag;                         //  pointer to external flag, set high to alert main prog of pot value change

    //=============================================  Scaling

    float deadZoneScale(uint16_t value);      //  scales pot travel outside of deadzones
    float finalOffsetScale(float input);      //  scales pot value for bipolar / unipolar etc.


    
public:
    pot(uint8_t _pin, float* _externalValue, bool* _changeFlag, potMode _mode);
  
    void timeToRead();  //  called by external timer/ISR, sets timeToReadAvalue high so update() knows to run on next loop
    void update();      //  polled externally,  when timeToReadAvalue is high, read new value and ...
                        //  ... work out average, then write the updated float value to *externalValue and set external *changeFlag high
};

#endif