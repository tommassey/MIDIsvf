#ifndef COMMAND_LIST_H
#define COMMAND_LIST_H

#include <Arduino.h>

enum commandTypes
{
    noCommand = 48,   //  offset so we can send ascii in serial monitor to test
    changeScreen,
    setWaveform,
    setRate,
    setAmp,
    setPhase
};


enum waveForms
{
    noWaveform = 0,
    sine,
    triangle,
    rampUp,
    rampDown,
    squaree,
    nonMusicLFO
};


#endif  //  COMMAND_LIST_H