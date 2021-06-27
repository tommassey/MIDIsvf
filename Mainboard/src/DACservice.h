#ifndef DAC_SERVICE_H
#define DAC_SERVICE_H

#include <Arduino.h>
#include "MIDIdefines.h"


uint16_t scaleForDAC(uint16_t data, MIDIconfigProfile value);  //  take min & max limits from MIDIconfig and scale input to suitable value for DAC



#endif