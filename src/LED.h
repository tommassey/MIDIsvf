#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include "pinDefines.h"

#define MAX_LED_BLINKS 4

struct LEDblinkProfile
{
    int numberBlinks = 1;
    int currentBlink = 0;

    uint16_t TimeOn[MAX_LED_BLINKS]  = {50};    // millis
    uint16_t TimeOff[MAX_LED_BLINKS] = {50};    // millis

};
//               name                num  | time | time | time | time | time | time
//                                  blinks| on 1 | off1 | on 2 | off2 | on 3 | off3 
//-------------|------------------|-------|-------------------
// const LEDblinkProfile filter1Config = {     1,    200,   200,   50,    50,    50,    50      };
// const LEDblinkProfile filter2Config = {     2,    100,   100,  100,   100,    50,    50      };
// const LEDblinkProfile complete      = {     3,    50,    200,   50,   200,    50,   200      };

const LEDblinkProfile init     = {          2,  0,    {200,   0,   0,   0,},    {  0,   0,   0,   0} };
const LEDblinkProfile f1config = {          2,  0,    {200,   0,   0,   0,},    {200, 500,   0,   0} };
const LEDblinkProfile f2config = {          3,  0,    {100, 100,   0,   0,},    {100, 100, 500,   0} };
const LEDblinkProfile saved    = {          4,  0,    { 50,  50,  50,   0,},    { 50,  50,  50, 900} };




class LED
{
    public:
    uint32_t oldTime = 0;
    bool ledState = LOW;
    uint32_t LEDtimeOn  = 50;   // millis
    uint32_t LEDtimeOff = 50;   // millis
    LEDblinkProfile currentBlinkProfile;

    public:
    //void updateLED(LED* led);
    void setBlinkProfile(LEDblinkProfile);


};

void updateLED(LED* led);









#endif