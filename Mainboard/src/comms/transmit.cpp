#include "transmit.h"
#include "../../Shared/commandList.h"


transmit::transmit(/* args */)
{
}

transmit::~transmit()
{
}


void transmit::showScreen(byte newScreen)
{
    Serial4.write(changeScreen);
    Serial4.write(',');
    Serial4.write(newScreen);
    Serial4.write('\n');


}


void transmit::setLFOwave(byte newWave)
{
    Serial4.write(setWaveform);
    Serial4.write(',');
    Serial4.write(newWave);
    Serial4.write('\n');
}

void transmit::setLFOrate(float newRate)
{
    byte rate = (byte)(newRate * 255);

    Serial.print("send rate = ");
    Serial.println(rate);

    Serial4.write(setRate);
    Serial4.write(',');
    Serial4.write(rate);
    Serial4.write('\n');
}

void transmit::setLFOamp(float newAmp)
{
    byte amp = (byte)(newAmp * 255);

    Serial4.write(setAmp);
    Serial4.write(',');
    Serial4.write(amp);
    Serial4.write('\n');
}

void transmit::setLFOphase(float newPhase)
{

    byte phase = (byte)(newPhase * 255);

    Serial4.write(setPhase);
    Serial4.write(',');
    Serial4.write(phase);
    Serial4.write('\n');
}