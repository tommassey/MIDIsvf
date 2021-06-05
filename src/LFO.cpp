#include "LFO.h"
#include "pinDefines.h"

#define NUM_AVRGES 4
#define chatterWindow 3

#define sinSteps 4095
#define NMLsteps 640

uint16_t sineTable[sinSteps];
uint16_t sineCurrentStep = 0;

uint16_t NMLtable[NMLsteps];
uint16_t NMLcurrentStep = 0;

const uint16_t LFOmax = 4095;

uint16_t LFOval = 0;
uint16_t rates[NUM_AVRGES] = {0};
uint16_t LFOrate = 0;
uint64_t prevLFOtime = 0;
uint8_t currentWaveForm = nonMusicLFO;
static bool triangleGoingUp = true;


void isrWriteToDAC(void)
{
    //Serial.print("dac write = ");
    //Serial.println(LFOval);
    DACwriteChannelB(LFOval);
}



uint16_t getRatePotValue(void)  // polls pot, returns true if changed LFOrate variable
{
    uint16_t newVal = analogRead(LFO_RATE_POT_PIN);

    if ((newVal > (LFOrate + chatterWindow)) || ((newVal < (LFOrate - chatterWindow))))
    {   
        Serial.print("newVal = ");
        Serial.print(newVal);

        for (uint8_t i = 0; i < (NUM_AVRGES-1); i++)
        {
           rates[i] = rates[i+1];  
           Serial.print("  rate ");
           Serial.print(i);
           Serial.print("  val = ");
           Serial.print(rates[i]);
        }
        
        rates[NUM_AVRGES-1] = newVal;

        uint16_t sum = 0;

        for (int i = 0; i < NUM_AVRGES; i++)
        {
          sum += rates[i];
        }

        Serial.print("  sum = ");
        Serial.print(sum);
        
        uint16_t average = sum / NUM_AVRGES;

        Serial.print("avrg = ");
        Serial.println(average);

        LFOrate = average;
        
        return true;
    }
    
    return false;
}




void updateLFO(void)
{
    if (micros() > (prevLFOtime + LFOrate))
    {
        switch (currentWaveForm)
        {
            case sine:
            {
                LFOval = sineTable[sineCurrentStep];
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

// void initSineTable(){
//   for (int i = 0; i < sinSteps; i++){
//     sineTable[i] = 2048+2048*sin(i*(2*3.14)/sinSteps);
//     Serial.println(sineTable[i]);
//   }
// }

void initNMLtable()
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





//        _____     ____      ____        ____      ____
//        |    \   |    |    |    \      /    |    |    |
//        |     \  |    |    |     \    /     |    |    |
//        |      \ |    |    |      \  /      |    |    |
//        |       \|    |    |       \/       |    |    |
//        |             |    |                |    |    |
//        |             |    |                |    |    |________
//        |             |    |                |    |            |
//________|             |____|                |____|            |________