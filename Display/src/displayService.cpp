#include "displayService.h"


displayService::displayService(oled* screenPtr)
{
    screen = screenPtr;

    initMenuOptions();
}

displayService::~displayService()
{
}

void displayService::checkForRedraw(void)
{
    if (needsRedraw == true)
    {
        screen->display();
        needsRedraw = false;
    }
}

void displayService::actOnInputs(int8_t inputNumber)
{ 
    //Serial.print("display serv input number = ");
    //Serial.println(inputNumber);

    switch (inputNumber)
    {
        case no_input:  break;

        case encoder_button:
        {
            currentLFOselected = LFO_none;
            clearMenu();

            splitScreenMode = ss_mode_home;

            break;
        }

        case lfo1_button:
        {
            splitScreenMode = ss_mode_menu;

            if (currentLFOselected == LFO_2) resetMenu();

            currentLFOselected = LFO_1;
            advanceMenu();
            
            break;
        }

        case lfo2_button:
        {
            splitScreenMode = ss_mode_menu;

            if (currentLFOselected == LFO_1) resetMenu();

            currentLFOselected = LFO_2;
            advanceMenu();
            
            break;
        }
        
        case menu_encoder:
        {
            // nothing here, handled by newEncoderMovement() called in main()
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
    
    needsRedraw = true;
}

void displayService::noteOnEvent(uint8_t whichLFO)
{
    if (noteOn[whichLFO] == true) return;
    
    else
    {
        noteOn[whichLFO] = true;
        needsRedraw = true;
        splitScreen();
    }
}

void displayService::noteOffEvent(uint8_t whichLFO)
{
    if (noteOn[whichLFO] == false) return;

    else
    {
        noteOn[whichLFO] = false;
        needsRedraw = true;
        splitScreen();
    }
}


void displayService::initMenuOptions(void)
{   

    menuOption moNone  = {0,            0,           0,          "NONE"};
    menuOption moWave  = {sine,         1,   totalWaveforms,     "WAVE"};
    menuOption moRate  = {1,            1,         255,          "RATE"};
    menuOption moAmp   = {99,         -99,          99,          "AMP"};
    menuOption moPhase = {0,            0,         254,          "PHASE"};
    menuOption moDelay = {0,            0,         254,          "DELAY"};
  

    menu[LFO_1][menu_option_none]  = moNone;
    menu[LFO_1][menu_option_wave]  = moWave;
    menu[LFO_1][menu_option_rate]  = moRate;
    menu[LFO_1][menu_option_amp]   = moAmp;
    menu[LFO_1][menu_option_phase] = moPhase;
    menu[LFO_1][menu_option_delay] = moDelay;

    menu[LFO_2][menu_option_none]  = moNone;
    menu[LFO_2][menu_option_wave]  = moWave;
    menu[LFO_2][menu_option_rate]  = moRate;
    menu[LFO_2][menu_option_amp]   = moAmp;
    menu[LFO_2][menu_option_phase] = moPhase;
    menu[LFO_2][menu_option_delay] = moDelay;

    

}



void displayService::advanceMenu(void)
{
    currentMenuOption++;

    if (currentMenuOption >= total_menu_options)
    {
        currentMenuOption = 1;
    }

    //Serial.print("Advance menu = ");
    //Serial.println(currentMenuOption);

    selectedOption = &menu[currentLFOselected][currentMenuOption];
    needsRedraw = true;
}

void displayService::clearMenu(void)
{
    currentMenuOption = menu_option_none;
    selectedOption = &menu[currentLFOselected][menu_option_none];
    needsRedraw = true;
}

void displayService::resetMenu(void)
{
    currentMenuOption = 1;
    selectedOption = &menu[currentLFOselected][1];
    needsRedraw = true;
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

    needsRedraw = true;

}



void displayService::splitScreen(void)
{
    screen->clear();

    switch (splitScreenMode)
    {
        case ss_mode_home:
            {
                drawLFOs();
                //screen->startScroll();
                break;
            }
        
        case ss_mode_menu:
            {
                drawLFOs();
                drawMenu();
                //screen->stopScroll();
                break;
            }
            
        
        default:
            break;
    }

    drawBorders();
    drawNotifications();

    needsRedraw = true;
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
        case LFO_1:
        {
            screen->drawRect(2, 29, 123, 27, 1);  // selected frame LFO1
            break;
        }

        case LFO_2:
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
    

    if (splitScreenMode == ss_mode_home)
    {
        drawCurrentWaveform(LFO_1);
        drawCurrentWaveform(LFO_2);
    }

    if (splitScreenMode == ss_mode_menu)
    {
        drawCurrentWaveform(currentLFOselected);
    }
}



void displayService::drawCurrentWaveform(uint8_t whichLFO)
{
    uint8_t currentWaveform = menu[whichLFO][menu_option_wave].value;

    uint8_t rate  = menu[whichLFO][menu_option_rate].value;
    uint8_t amp   = menu[whichLFO][menu_option_amp].value;
    uint8_t phase = menu[whichLFO][menu_option_phase].value;
    uint8_t delay = menu[whichLFO][menu_option_delay].value;

    
    switch (currentWaveform)
    {
        case sine:
        {
            screen->smallSine(whichLFO, rate, amp, phase, delay);
            break;    
        }

        case triangle:
        {
            screen->smallTriangle(whichLFO, rate, amp, phase, delay);
            break;    
        }

        case saw:
        {
            screen->smallSaw(whichLFO, rate, amp, phase, delay);
            break;    
        }

        case squaree:
        {
            screen->smallSquare(whichLFO, rate, amp, phase, delay);
            break;    
        }

    
    default:
        break;
    }
 }



uint8_t Y1 = 40;
uint8_t Y2 = 8;
uint8_t optionX = 12;
uint8_t valueX = 64;

void displayService::drawMenu(void)
{
    //Serial.print(" lfo  =  ");
    //Serial.println(currentLFOselected);
    
    //Serial.print(" option  =  ");
    //Serial.println(menu[currentLFOselected][currentMenuOption].name);

    if (currentMenuOption == menu_option_none)
    {
        return;
    }

    
    //  get value
    int16_t value = menu[currentLFOselected][currentMenuOption].value;
    char str[5];
    itoa(value, str, 10);   //  int to a string

    // Serial.print(" value  =  ");
    // Serial.print(value);
    // Serial.print("    string = ");
    // Serial.println(str);


    // //  print option name
    // screen->string(12, 40, menu[currentLFOselected][currentMenuOption].name, 16, 1);

    // // print value
    // screen->string16pix(64, 40, str);

    if (currentLFOselected == LFO_1)
    {
        //  print option name
        screen->string(optionX, Y1, menu[currentLFOselected][currentMenuOption].name, 16, 1);
        // print value  
        screen->string16pix(valueX, Y1, str);

    }

    if (currentLFOselected == LFO_2)
    {
        screen->string(optionX, Y2, menu[currentLFOselected][currentMenuOption].name, 16, 1);
        // print value  
        screen->string16pix(valueX, Y2, str);
        
    }
    

    
}



void displayService::drawNotifications(void)
{
    static const char* noteOnText = " NOTE ON ";
    
    if (noteOn[LFO_1] == true)
    {
        if ( (splitScreenMode == ss_mode_home) ||
             (splitScreenMode == ss_mode_menu  && currentLFOselected == LFO_1) )
        {
            screen->string(38, 10, noteOnText, 12, 0);   
        }     
    }
    
    if (noteOn[LFO_2] == true)
    {
        if ( (splitScreenMode == ss_mode_home) ||
             (splitScreenMode == ss_mode_menu  && currentLFOselected == LFO_2) )
        {
            screen->string(38, 42, noteOnText, 12, 0);   
        }     
    }
}



void displayService::updateLFO()
{
    

    switch (LFOshape)
    {
        case sine:
        {
            //screen->sine(sinCycles, LFOamp);
            break;
        }

        case triangle:
        {
            //screen->triangle(LFOrate, LFOamp);
            break;
        }

        case saw:
        {
            //screen->saw(LFOrate, LFOamp);
            break;
        }

        case squaree:
        {
            //screen->square(LFOrate, LFOamp);
            break;
        }

        case nonMusicLFO:
        {
            //screen->square(LFOrate, LFOamp);
            break;
        }
        

    
        default:
            break;
    }

    //screen->stopScroll();

    screen->display();
    //screen->startScroll();

}

void displayService::setLFOwave(byte waveForm)
{
    //Serial.print("set wave: ");
    //Serial.println(waveForm);

    LFOshape = waveForm;

    updateLFO();
}



void displayService::setLFOrate(byte rate)
{
    //Serial.print("new rate: ");
    //Serial.println(rate);

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
    //Serial.print("set amp: ");
    //Serial.println(amp);

    LFOamp = amp;

    updateLFO();    
}


void displayService::setLFOphase(byte phase)
{
    //Serial.print("set phase: ");
    //Serial.println(phase);

    LFOphase = phase;

    updateLFO();
}


