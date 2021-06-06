#ifndef LFO_H
#define LFO_H

#include <Arduino.h>


enum waveForms
{
    noWaveform = 0,
    sine,
    triangle,
    rampUp,
    rampDown,
    square,
    nonMusicLFO
};


class LFO
{
private:
    const int16_t LFOmax = 4095;

    int16_t LFOval = 0;
    float LFOamp = 0.0;    // +/- 1.0
    uint16_t LFOrate = 0;
    uint64_t prevLFOtime = 0;

    int16_t* externalLFOval;

    uint8_t currentWaveForm = nonMusicLFO;
    bool triangleGoingUp = true;
    
    bool waveForms_Init = false;
    void initSineTable(void);
    void initNMLtable(void);

public:
    LFO(int16_t* value);
    void initWaveForms(void);

    void setRate(float rate);
    void setAmount(float amount);
    void update(void);
};



//uint16_t getRatePotValue(void);
void updateLFO(void);


//void initLFOpointer(int16_t* value);

void setLFOrate(float rate);
void setLFOamount(float amount);





#endif