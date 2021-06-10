#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include "Bounce2.h"


enum buttonPresses
{
    noPress = 0,
    shortPress,
    longPress
};

void buttonInit(Bounce* btn, uint8_t pin, uint8_t mode, uint16_t bounceTime);
int8_t checkButton(Bounce* btn);  //  called to poll the button, returns a value from buttonPresses enum 


#endif