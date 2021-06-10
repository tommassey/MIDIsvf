#ifndef LFO_H
#define LFO_H

#include <Arduino.h>

typedef byte waveform;

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
  
    const int16_t LFOmax = 4095;  // max LFO value / resolution

    int16_t LFOval = 0;           //  current LFO value
    float LFOamp = 0.0;           //  LFO amplitude +/- 1.0
    uint16_t LFOrate = 0;       
    uint64_t prevLFOtime = 0;

    float* externalLFOval;      //  pointer to external value we keep updated

    waveform currentWaveForm = triangle;
    bool triangleGoingUp = true;            //  used to switch triangle direction at top/bottom

    bool waveForms_Init = false;            //  set true after wavetable arrays have been filled
        
    void initSineTable(void);               //  init wavetables
    void initNMLtable(void);


  public:

    LFO(float* value);
    void initWaveForms(void);

    void update(void);          //  polled externally to update LFO

    void setRate(float rate);
    void setAmount(float amount);
    void setShape(waveform wave);
};


#endif