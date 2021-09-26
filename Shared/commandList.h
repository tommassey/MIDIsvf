#ifndef COMMAND_LIST_H
#define COMMAND_LIST_H

#include <Arduino.h>

enum commandTypes
{
    noCommand = 0,   
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
    saw,
    //rampUp,
    //rampDown,
    squaree,
    nonMusicLFO,
    totalWaveforms
};

enum LFOS
{
    LFO_1,
    LFO_2,
    LFO_total
};


#endif  //  COMMAND_LIST_H