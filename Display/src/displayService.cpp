#include "displayService.h"


displayService::displayService(oled* screenPtr)
{
    screen = screenPtr;
}

displayService::~displayService()
{
}

void displayService::actOnInputs(int8_t inputNumber)
{ 
    Serial.print("display serv input number = ");
    Serial.println(inputNumber);

    switch (inputNumber)
    {
        case no_input:  break;

        case encoder_button:
        {
            currentLFOselected = 0;

            break;
        }

        case lfo1_button:
        {
            if (currentLFOselected == 1)
            {
                advanceMenu();
                
            }
            else
            {
                currentLFOselected = 1;
            }

            break;
        }

        case lfo2_button:
        {
            if (currentLFOselected == 2)
            {
                advanceMenu();
                
            }
            else
            {
                currentLFOselected = 2;
            }

            break;
        }
        
        case menu_encoder:
        {

            break;
        }
    
        default:
            break;
    }

    splitScreen();

}

void displayService::advanceMenu(void)
{
    currentMenuOption++;

    if (currentMenuOption >= total_menu_options)
    {
        currentMenuOption = menu_option_none;
    }

    Serial.print("Advance menu = ");
    Serial.println(currentMenuOption);
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

    switch (splitScreenMode)
    {
        case ss_mode_home:
            {
                drawLFO1();
                drawMenu();
                //drawLFO2();
                break;
            }
        
        case ss_mode_menu:
            {
                // draw 2 LFOs
                break;
            }
            
        
        default:
            break;
    }

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


    screen->drawRect(0, 32, 127, 32, 1);  // top frame
    screen->drawRect(0, 63, 127, 32, 1);  // bottom frame

    switch (currentLFOselected)
    {
        case 0:  break;

        case 1:
        {
            screen->drawRect(2, 29, 123, 27, 1);  // selected frame LFO1
            break;
        }

        case 2:
        {
            screen->drawRect(2, 61, 123, 27, 1);  // selected frame LFO2
            break;
        }

    
    default:
        break;
    }

    






}

void displayService::drawLFO1(void)
{
    screen->smallSine();
}



void displayService::drawMenu(void)
{
    //  print option name
    screen->string(4, 48, menu[currentMenuOption].name, 16, 1);

    //  print value
    int16_t value = menu[currentMenuOption].value;
    char str[5];

    itoa(value, str, 10);

    Serial.print(" value  =  ");
    Serial.print(value);
    Serial.print("    string = ");
    Serial.println(str);

    
    screen->string(40, 48, str, 16, 0);

    
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


