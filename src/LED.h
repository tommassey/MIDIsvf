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

const LEDblinkProfile init     = {          4,  0,    {500,   0,   0,   0,},    {500, 500, 500, 500} };
const LEDblinkProfile f1config = {          1,  0,    {200,   0,   0,   0,},    {200, 500, 500, 500} };
const LEDblinkProfile f2config = {          2,  0,    {100, 100,   0,   0,},    {100, 100, 500, 500} };
const LEDblinkProfile saved    = {          4,  0,    { 50,  50,  50,   0,},    { 50,  50,  50, 900} };
const LEDblinkProfile reset    = {          4,  0,    { 20,  20,  20,   0,},    { 20,  20,  20, 900} };


class LED
{
    public:
    uint32_t oldTime = 0;
    bool ledState = LOW;
    uint32_t LEDtimeOn  = 50;   // millis
    uint32_t LEDtimeOff = 50;   // millis
    LEDblinkProfile currentBlinkProfile;
    uint16_t currentBlink = 0;

    void setBlinkProfile(LEDblinkProfile);
};

void updateLED(LED* led);



#endif