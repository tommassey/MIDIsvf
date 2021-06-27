#ifndef RECIEVER_H
#define RECIEVER_H

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

struct serialMessage
{
    byte messageType = 0;
    byte data = 0;
};

serialMessage getSerialMessage(void);

//void checkSerial(oled);




#endif