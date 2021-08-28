#include "displayService.h"


displayService::displayService(oled* screenPtr)
{
    screen = screenPtr;

    initMenuOptions();
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
            clearMenu();

            splitScreenMode = ss_mode_home;

            break;
        }

        case lfo1_button:
        {
            splitScreenMode = ss_mode_menu;

            if (currentLFOselected == 2) resetMenu();

            currentLFOselected = 1;
            advanceMenu();
            
            break;
        }

        case lfo2_button:
        {
            splitScreenMode = ss_mode_menu;

            if (currentLFOselected == 1) resetMenu();

            currentLFOselected = 2;
            advanceMenu();
            
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

void displayService::newEncoderMovement(int32_t movement)
{
    selectedOption->value += movement;

    if (selectedOption->value > selectedOption->max) selectedOption->value = selectedOption->max;
    if (selectedOption->value < selectedOption->min) selectedOption->value = selectedOption->min;
    
}


void displayService::initMenuOptions(void)
{   

    menuOption moNone  = {0,            0,           0,          "NONE"};
    menuOption moWave  = {sine,         1,   totalWaveforms,     "WAVE"};
    menuOption moRate  = {1,            1,         255,          "RATE"};
    menuOption moAmp   = {99,         -99,          99,          "AMP"};
    menuOption moPhase = {0,            0,         360,          "PHASE"};
    menuOption moDelay = {0,            0,         255,          "DELAY"};
  

    menu[1][menu_option_none]  = moNone;
    menu[1][menu_option_wave]  = moWave;
    menu[1][menu_option_rate]  = moRate;
    menu[1][menu_option_amp]   = moAmp;
    menu[1][menu_option_phase] = moPhase;
    menu[1][menu_option_delay] = moDelay;

    menu[2][menu_option_none]  = moNone;
    menu[2][menu_option_wave]  = moWave;
    menu[2][menu_option_rate]  = moRate;
    menu[2][menu_option_amp]   = moAmp;
    menu[2][menu_option_phase] = moPhase;
    menu[2][menu_option_delay] = moDelay;

    

}



void displayService::advanceMenu(void)
{
    currentMenuOption++;

    if (currentMenuOption >= total_menu_options)
    {
        currentMenuOption = 1;
    }

    Serial.print("Advance menu = ");
    Serial.println(currentMenuOption);

    selectedOption = &menu[currentLFOselected][currentMenuOption];
}

void displayService::clearMenu(void)
{
    currentMenuOption = menu_option_none;
    selectedOption = &menu[currentLFOselected][menu_option_none];
}

void displayService::resetMenu(void)
{
    currentMenuOption = 1;
    selectedOption = &menu[currentLFOselected][1];
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
                drawLFOs();
                break;
            }
        
        case ss_mode_menu:
            {
                drawLFOs();
                drawMenu();
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

void displayService::drawLFOs(void)
{
    screen->smallSine();

    if (splitScreenMode == ss_mode_home)
    {
        
    }
}



void displayService::drawMenu(void)
{
    if (currentMenuOption == menu_option_none)
    {
        return;
    }

    Serial.print(" lfo  =  ");
    Serial.println(currentLFOselected);
    
    Serial.print(" option  =  ");
    Serial.println(menu[currentLFOselected][currentMenuOption].name);
    

    //  print option name
    screen->string(12, 40, menu[currentLFOselected][currentMenuOption].name, 16, 1);

    //  print value
    int16_t value = menu[currentLFOselected][currentMenuOption].value;
    char str[5];

    itoa(value, str, 10);

    Serial.print(" value  =  ");
    Serial.print(value);
    Serial.print("    string = ");
    Serial.println(str);

    
    screen->string16pix(64, 40, str);

    
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


