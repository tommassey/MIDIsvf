#ifndef INPUTS_H
#define INPUTS_H

#include <Arduino.h>
#include "pot.h"


Bounce button = Bounce();

bool LFOrateChange = false;
float LFOrate = 0;

pot LFOratePot(23, &LFOrate, &LFOrateChange);


bool LFOamtAchange = false;
float LFOamtA = 0;

pot LFOamtAPot(22, &LFOamtA, &LFOamtAchange);



void checkPots(LFO* lfo)
{
  LFOratePot.update();
  LFOamtAPot.update();

  if (LFOrateChange) 
  {
    lfo->setRate(LFOrate);
    LFOrateChange = false;
  }

  if (LFOamtAchange) 
  {
    lfo->setAmount(LFOamtA);
    LFOrateChange = false;
  }
}


void readpotsISR(void)
{
  LFOratePot.timeToRead();
  LFOamtAPot.timeToRead();
}

#endif