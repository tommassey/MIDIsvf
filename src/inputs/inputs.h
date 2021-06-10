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



void checkPots(LFO* lfo)  //  polled externally, checks pots if flag set via ISR
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
    LFOamtAchange = false;
  }
}


void readpotsISR(void)   //  call this with an ISR at desired rate, sets flags to read pots
{
  LFOratePot.timeToRead();
  LFOamtAPot.timeToRead();
}

#endif