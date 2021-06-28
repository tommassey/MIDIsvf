#ifndef TRANSMIT_H
#define TRANSMIT_H

#include <Arduino.h>


class transmit
{
private:
    /* data */
public:
    transmit(/* args */);
    ~transmit();
    void showScreen(byte newScreen);
    void setLFOwave(byte newWave);
    void setLFOrate(float newRate);
    void setLFOamp(float newAmp);
    void setLFOphase(float newPhase);
};








#endif  // TRANSMIT_H