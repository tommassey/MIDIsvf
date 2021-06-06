#include "LFO/LFO.h"


#define sinSteps 4095
#define NMLsteps 640

uint16_t sineTable[sinSteps];
uint16_t sineCurrentStep = 0;

uint16_t NMLtable[NMLsteps];
uint16_t NMLcurrentStep = 0;



LFO::LFO(int16_t* value)
{
    externalLFOval = value;
}

void LFO::setRate(float rate)
{
    if (rate > 1.0) rate = 1.0;
    if (rate < 0.0) rate = 0.0;
    LFOrate = (rate * 4095) + 1;
}

void LFO::setAmount(float amount)
{
    if (amount > 1.0) amount = 1.0;
    if (amount < 0.0) amount = 0.0;
    LFOamp = amount;
}

void LFO::setShape(waveform wave)
{
    currentWaveForm = wave;
}


void LFO::update(void)
{
    //if (micros() > (prevLFOtime + _LFOrate))
    if ((micros() - prevLFOtime) > LFOrate)
    {   
        *externalLFOval = LFOval - 2048;
        analogWrite(5, (LFOval /16));

        switch (currentWaveForm)
        {
            case sine:
            {
                LFOval = sineTable[sineCurrentStep];
                sineCurrentStep++;
                if (sineCurrentStep > (sinSteps-1)) sineCurrentStep = 0;
                prevLFOtime = micros();
                //if(prevLFOtime > 4294967294) prevLFOtime = 0;
                break;
            }

            case triangle:
            {   
                if (triangleGoingUp)
                {
                    LFOval++;
                    prevLFOtime = micros();
                    //if(prevLFOtime > 4294967294) prevLFOtime = 0;

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
                    //if(prevLFOtime > 4294967294) prevLFOtime = 0;

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
          //if(prevLFOtime > 4294967294) prevLFOtime = 0;
          break;
        }


        case nonMusicLFO:
        {
            LFOval = NMLtable[NMLcurrentStep];
            NMLcurrentStep++;
            if (NMLcurrentStep > (NMLsteps-1)) NMLcurrentStep = 0;
            prevLFOtime = micros();
            //if(prevLFOtime > 4294967294) prevLFOtime = 0;               
            break;

        }

        
    
    default:
        break;
    }

    }
    
    
    

}


void LFO::initWaveForms(void)
{
    initSineTable();
    initNMLtable();
    waveForms_Init = true;
}


void LFO::initSineTable()
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
