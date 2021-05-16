#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include "pinDefines.h"


class LED
{
    uint32_t oldTime = 0;
    bool ledState = LOW;
    uint32_t LEDtimeOn  = 50;   // millis
    uint32_t LEDtimeOff = 50;   // millis

    public:
    void updateLED(void);
    void setLEDflashtime(uint32_t timeon, uint32_t timeoff);


};





#endif