#ifndef INPUTS_H
#define INPUTS_H

#include <Arduino.h>
#include "pot.h"


Bounce button = Bounce();

bool LFOrateChange = false;
float LFOrate = 0;

pot LFOratePot(23, &LFOrate, &LFOrateChange, potmode_unipolar);


bool LFOamtAchange = false;
float LFOamtA = 0;   

pot LFOamtAPot(22, &LFOamtA, &LFOamtAchange, potmode_bipolar);


bool LFOphaseChange = false;
float LFOphase = 0;

pot LFOphasePot(21, &LFOphase, &LFOphaseChange, potmode_unipolar);


void checkPots(LFO* lfo)  //  polled externally, checks pots if flag set via ISR
{
  LFOratePot.update();
  LFOamtAPot.update();
  LFOphasePot.update();

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

  if (LFOphaseChange) 
  {
    lfo->setPhase(LFOphase);
    LFOphaseChange = false;
  }
}


void readpotsISR(void)   //  call this with an ISR at desired rate, sets flags to read pots
{
  LFOratePot.timeToRead();
  LFOamtAPot.timeToRead();
  LFOphasePot.timeToRead();
}

#endif