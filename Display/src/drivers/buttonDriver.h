#ifndef BUTTON_DRIVER_H
#define BUTTON_DRIVER_H

#include <Arduino.h>
#include "Bounce2.h"


enum buttonPressTypes
{
    noPress = 0,
    shortPress,
    longPress
};


class ButtonDriver
{
    private:
        Bounce* deBouncer;

        uint8_t status = noPress;


    public:
        ButtonDriver(Bounce* btn, uint8_t pin, uint8_t mode, uint16_t bounceTime);
        
        uint8_t check(void);
};




#endif // BUTTON_DRIVER_H