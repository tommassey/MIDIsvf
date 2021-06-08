#ifndef MIDISERV_H
#define MIDISERV_H

#include <Arduino.h>
#include "DAC.h"
#include "pinDefines.h"
#include "MIDIsetup.h"
#include "smooth.h"

typedef enum
{
    noParameter = 0,
    parameterFilter1,
    parameterFilter2,
    parameterTotal

}   output;


typedef struct
{
    output name = noParameter;
    uint16_t value = 0;

}   MIDIevent;


class MIDIservice
{
private:
    static const uint8_t totalParameters = parameterTotal;
    MIDIconfigProfile config[parameterTotal];
    uint16_t* externVal[parameterTotal];
    
    bool scaling_init = false;
    void initScaling(void);  // load at init to calc scaling values from saved data
    MIDIevent getMIDImsgFromBuffer();

    //static const byte totalBytes = 2; // MSB and LSB
    uint16_t bitShiftCombine(uint8_t msb, uint8_t lsb);
    byte bytesForCC[parameterTotal][totalBytes] = {{0}};

    uint16_t value[parameterTotal] = {0};
    uint16_t previousValue[parameterTotal] = {0};
    uint16_t valueToBeSmoothed[parameterTotal] = {0};
    bool* timeToSmooth;                         //  pointer to external flag set by ISR at desired smoothing interval
    Smoother smoother[parameterTotal];

    void smoothValues(void);



    uint16_t scaleForDAC(uint16_t data, MIDIconfigProfile filter);  // used before final write


    void printConfigData();        // prints config data to serial monitor
    void printCC(int cc, int val); // print incoming CC date for debug

    /* data */
public:
    MIDIservice(/* args */);
    void check(void);   //  poll this as often as you want to check for MIDI messages
    void initParameter(output param, MIDIconfigProfile config, uint16_t* p_value);   //  need to call this for each output to be used
};





#endif
