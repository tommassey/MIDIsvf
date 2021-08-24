#ifndef INPUTS_H
#define INPUTS_H

#include <Arduino.h>
#include <Encoder.h>
#include <Bounce2.h>

#include "hardware/pinDefines.h"


Encoder encoder = Encoder(ENCODER_PIN_A, ENCODER_PIN_B);

Bounce button1 = Bounce(BUTTON_1_PIN, DEBOUNCE_TIME_MILLIS);   // LFO1 button
Bounce button2 = Bounce(BUTTON_2_PIN, DEBOUNCE_TIME_MILLIS);   //  LFO2 button
Bounce button3 = Bounce(BUTTON_3_PIN, DEBOUNCE_TIME_MILLIS);   //  Encoder button






#endif  // INPUTS_H