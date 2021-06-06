#ifndef INPUTS_H
#define INPUTS_H

#include <Arduino.h>
#include "pot.h"


bool LFOrateChange = false;
float LFOrate = 0;

pot LFOratePot(23, &LFOrate, &LFOrateChange);


bool LFOamtAchange = false;
float LFOamtA = 0;

pot LFOamtAPot(22, &LFOamtA, &LFOamtAchange);



#endif