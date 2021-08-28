#ifndef INPUT_NAMES_H
#define INPUT_NAMES_H


#include <Arduino.h>






enum InputNames
{
    no_input = -1,
    encoder_button = 0,
    lfo1_button = 1,
    lfo2_button = 2,
    max_buttons = 3,

    menu_encoder = 5
};


#endif // INPUT_NAMES_H