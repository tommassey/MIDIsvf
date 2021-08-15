#include "LFO/LFO.h"
#include "comms/transmit.h"


#define sinSteps 4095
#define NMLsteps 640


transmit txToScreen;

uint16_t sineTable[sinSteps];
uint16_t sineCurrentStep = 0;

uint16_t NMLtable[NMLsteps];
uint16_t NMLcurrentStep = 0;



LFO::LFO(float* value)
{
    externalLFOval = value;
}

void LFO::setRate(float rate)  // recieves percentage of pot travel as a float 
{
    //float r = ((rate / (pow(rate, rate) + 1)) * 2);
    float r = (1 / 1 - (powf(rate + 1, rate))) * -1;   //  weird shit to try and make the pot taper a bit nicer
    Serial.print(" pow = ");
    Serial.println(r);
    if (r > 1.0) r = 1.0;
    if (r < 0.0) r = 0.0;

    txToScreen.setLFOrate(r);

    LFOrate = (r * LFOmax) + 1;  //  change to a value between 0 and LFO max.   +1 make sure it's never 0 
}

void LFO::setAmount(float amount)
{
    Serial.print("LFO amount = ");
    

    if (amount > 1.0) amount = 1.0;
    if (amount < 0.0) amount = 0.0;

    Serial.println(amount);
    txToScreen.setLFOamp(amount);
    LFOamp = amount;
}

void LFO::setShape(waveform wave)
{
  txToScreen.setLFOwave(wave);
  
  currentWaveForm = wave;
}

void LFO::setPhase(float phase)
{
  startPhase = (LFOmax * phase);

  if (startPhase > LFOmax) startPhase = LFOmax;
  if (startPhase < 0) startPhase = 0;

  txToScreen.setLFOphase(phase);

  Serial.print("LFO phase = ");
  Serial.println(startPhase);
}

void LFO::reset(void)
{
  LFOval = startPhase;
  sineCurrentStep = startPhase;
  
}


void LFO::update(void)  // polled externally to keep LFO updated
{
  if ((micros() - prevLFOtime) > LFOrate) //  micros() is time since prog start
  {   
    *externalLFOval = LFOval;           //  update external value
    analogWrite(5, (LFOval /16));       //  PWM to LED for visual indicator
 
    switch (currentWaveForm)
    {
      case sine:
      {
          LFOval = sineTable[sineCurrentStep];   //  read sine values from array
          sineCurrentStep++;
          if (sineCurrentStep > (sinSteps-1)) sineCurrentStep = 0;
          prevLFOtime = micros();
          break;
      }

      case triangle:
      {   
        if (triangleGoingUp)
        {
          LFOval++;
          prevLFOtime = micros();

          if (LFOval > LFOmax)
          {   
            triangleGoingUp = false;
            return;
          }
        }
 
        if (!triangleGoingUp)
        {
          LFOval--;
          prevLFOtime = micros();

          if (LFOval < 1)
          {
           triangleGoingUp = true;
           return;
          }
        }

        break;
      }

      case rampUp:
      {
        LFOval++;
        if(LFOval > LFOmax) LFOval = 0;
        prevLFOtime = micros();
        break;
      }

      case nonMusicLFO:
      {
        LFOval = NMLtable[NMLcurrentStep];
        NMLcurrentStep++;
        if (NMLcurrentStep > (NMLsteps-1)) NMLcurrentStep = 0;
        prevLFOtime = micros();
        break;
      }

      default:
            break;
    }
  }
}


void LFO::initWaveForms(void)  //  fill arrays with wavetable data
{
    initSineTable();
    initNMLtable();
    waveForms_Init = true;
}


void LFO::initSineTable()       //  fill array with sine values
{
  for (int i = 0; i < sinSteps; i++){
    sineTable[i] = 2048+2048*sin(i*(2*3.14)/sinSteps);
    Serial.println(sineTable[i]);
  }
}

void LFO::initNMLtable()
{
    // zero
    for (uint8_t i = 0; i < 80; i++)
    {
        NMLtable[i] = 0;
    }

    // top of N
    for (uint8_t i = 80; i < 120; i++)
    {
        NMLtable[i] = 4095;
    }

    // slope N
    uint16_t part = (2048/40);
    for (uint16_t i = 120, j = 4095; i < 160; i++, j-=part)
    {
        NMLtable[i] = j;
    }

    // 2ndtop of N
    for (uint16_t i = 160; i < 200; i++)
    {
        NMLtable[i] = 4095;
    }
    
    // gap
    for (uint16_t i = 200; i < 240; i++)
    {
        NMLtable[i] = 0;
    }

    // top of M
    for (uint16_t i = 240; i < 280; i++)
    {
        NMLtable[i] = 4095;
    }

    // slope M
    
    for (uint16_t i = 280, j = 4095; i < 320; i++, j-=part)
    {
        NMLtable[i] = j;
    }
    for (uint16_t i = 320, j = 2048; i < 360; i++, j+=part)
    {
        NMLtable[i] = j;
    }

    // 2nd top of M
    for (uint16_t i = 360; i < 400; i++)
    {
        NMLtable[i] = 4095;
    }

    // bottom
    for (uint16_t i = 400; i < 440; i++)
    {
        NMLtable[i] = 0;
    }

    // top of L
    for (uint16_t i = 440; i < 480; i++)
    {
        NMLtable[i] = 4095;
    }

    // bar of L
    for (uint16_t i = 480; i < 560; i++)
    {
        NMLtable[i] = 1024;
    }

    // flat
    for (uint16_t i = 560; i < 640; i++)
    {
        NMLtable[i] = 0;
    }

    for(uint16_t i = 0; i < NMLsteps-1; i++)
    {
        Serial.print("step = ");
        Serial.println(NMLtable[i]);
    }
}
