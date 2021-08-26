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

void displayService::splitScreen(void)
{
    screen->clear();

    drawBorders();

    screen->display();

}

//  0,0                         127,0
//  -----------------------------
//  |                           |
//  |                           |
//  |                           |
//  |                           |
//  |                           |
//  |                           |
//  |                           |
//  -----------------------------
// 0, 63                        127, 63

void displayService::drawBorders()
{
    //screen->drawRect(0, 0, 127, 31, 1);

    screen->
    screen->drawRect(0, 31, 127, 32, 1);

    screen->pixel(0,0,1);

    screen->pixel(127,0,1);

    screen->pixel(0,63,1);

    screen->pixel(127,63,1);






}

void displayService::updateLFO()
{
    

    switch (LFOshape)
    {
        case sine:
        {
            screen->sine(sinCycles, LFOamp);
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
    Serial.print("new rate: ");
    Serial.println(rate);

    LFOrate = rate;

    switch (LFOshape)
    {
        case sine:
        {
            //  rate comes in at a max of 255, higher is a slower LFO
            //  we need it to range between 0 and 14, higher is more cycles onscreen (so faster LFO)

            uint8_t invRate = 255 - rate;   // invert rate so higher is faster

            sinCycles = invRate / 18;   // 255 / 18 = 14.16666

            if (sinCycles > 14) sinCycles = 14;
            if (sinCycles < 1) sinCycles = 1;

            if (sinCycles == sinCyclesPrevious) return;
            sinCyclesPrevious = sinCycles;
            break;
        }
    
        default:
        break;
    }

    

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


