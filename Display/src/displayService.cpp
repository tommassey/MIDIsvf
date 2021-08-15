#include "displayService.h"


displayService::displayService(oled* screenPtr)
{
    screen = screenPtr;
}

displayService::~displayService()
{
}



void displayService::showScreen(byte screenNumber)
{
    Serial.print("show screen: ");
    Serial.println(screenNumber);

    switch (screenNumber)
    {
    case 1:
        screen->splashScreen();
        break;
    
    default:
        break;
    }

    screen->display();

}

void displayService::updateLFO()
{
    

    switch (LFOshape)
    {
        case sine:
        {
            screen->sine(LFOrate, LFOamp);
            break;
        }

        case triangle:
        {
            screen->triangle(LFOrate, LFOamp);
            break;
        }

        case rampUp:
        {
            screen->saw(LFOrate, LFOamp);
            break;
        }

        case rampDown:
        {
            screen->saw(LFOrate, LFOamp);
            break;
        }

        case squaree:
        {
            screen->square(LFOrate, LFOamp);
            break;
        }

        case nonMusicLFO:
        {
            screen->square(LFOrate, LFOamp);
            break;
        }
        

    
        default:
            break;
    }

    screen->stopScroll();

    screen->display();
    screen->startScroll();

}

void displayService::setLFOwave(byte waveForm)
{
    Serial.print("set wave: ");
    Serial.println(waveForm);

    LFOshape = waveForm;

    updateLFO();
}



void displayService::setLFOrate(byte rate)
{
    Serial.print("set rate: ");
    Serial.println(rate);

    LFOrate = rate;

    updateLFO();    
}


void displayService::setLFOamp(byte amp)
{
    Serial.print("set amp: ");
    Serial.println(amp);

    LFOamp = amp;

    updateLFO();    
}


void displayService::setLFOphase(byte phase)
{
    Serial.print("set phase: ");
    Serial.println(phase);

    LFOphase = phase;

    updateLFO();
}


