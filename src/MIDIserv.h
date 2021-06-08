#ifndef MIDISERV_H
#define MIDISERV_H

#include <Arduino.h>
#include "pinDefines.h"
#include "MIDIdefines.h"

#include "MIDIsetup.h"

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


//
//  looks like this isnt smoothing 7bit CC values
//
//


class MIDIservice
{
private:
    static const uint8_t totalParameters = parameterTotal;  //  for indexing
    MIDIconfigProfile config[parameterTotal];               //  config profiles
    uint16_t* externVal[parameterTotal];                    //  pointers to external values we update
    uint16_t value[parameterTotal] = {0};                   //  14bit values
    uint16_t previousValue[parameterTotal] = {0};
    
        
    byte bytesForCC[parameterTotal][2] = {{0}};  // 2 for MSB & LSB
    uint16_t bitShiftCombine(uint8_t msb, uint8_t lsb);

    bool scaling_init = false;
    void initScaling(void);  // load at init to calc scaling values from saved data
    MIDIevent getMIDImsgFromBuffer();
    
    
    //bool timeToSmooth = false;                         //  pointer to external flag set by ISR at desired smoothing interval
    //static Smoother smoother[parameterTotal];
    //void smoothValues(void);

    


    //void printConfigData();        // prints config data to serial monitor
    void printCC(int cc, int val); // print incoming CC date for debug

public:

    MIDIservice();
    void check(void);   //  poll this as often as you want to check for MIDI messages
    void initParameter(output param, MIDIconfigProfile config, uint16_t* p_value);   //  need to call this to setup each output to be used
    //void smoothNow(void);
    void printConfigData();        // prints config data to serial monitor
    //void smoothValues(void);


};


#endif
