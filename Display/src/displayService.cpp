#include "displayService.h"


displayService::displayService(oled* screenPtr)
{
    screen = screenPtr;

    initLFOmenuOptions();
    initSettingsMenuOptions();
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

    switch (currentScreenMode)
    {
        case screenMode_fullScreen:  //==============================================  Fullscreen Mode
        {
            switch (inputNumber)
            {
                case no_input:  break;

                case encoder_button:
                {
                    currentScreenMode = screenMode_splitScreen;
                    currentLFOselected = LFO_none;
                    splitScreenMode = ss_mode_home;
                    clearLFOmenu();

                    break;
                }

                case encoder_button_long_press:
                {
                    Serial.println("Encoder Button Long Press");
                    break;
                }

                case lfo1_button:
                {
                    currentScreenMode = screenMode_splitScreen;
                    splitScreenMode = ss_mode_menu;
                    resetLFOmenu();            
                    
                    break;
                }

                case lfo2_button:
                {
                    currentScreenMode = screenMode_splitScreen;
                    splitScreenMode = ss_mode_menu;
                    resetLFOmenu();
                    
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


            break;
        }

        case screenMode_splitScreen:  //==============================================  Splitscreen Mode
        {
            switch (inputNumber)
            {
                case no_input:  break;

                case encoder_button:
                {
                    currentLFOselected = LFO_none;
                    clearLFOmenu();

                    splitScreenMode = ss_mode_home;

                    break;
                }

                case encoder_button_long_press:
                {
                    currentScreenMode = screenMode_settings;
                    Serial.println("Encoder Button Long Press");
                    break;
                }

                case lfo1_button:
                {
                    splitScreenMode = ss_mode_menu;

                    if (currentLFOselected == LFO_2) resetLFOmenu();

                    currentLFOselected = LFO_1;
                    advanceLFOmenu();
                    
                    break;
                }

                case lfo2_button:
                {
                    splitScreenMode = ss_mode_menu;

                    if (currentLFOselected == LFO_1) resetLFOmenu();

                    currentLFOselected = LFO_2;
                    advanceLFOmenu();
                    
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


            break;
        }

        case screenMode_settings:  //==============================================  Settings Mode
        {
            switch (inputNumber)
            {
                case no_input:  break;

                case encoder_button:
                {
                    currentScreenMode = screenMode_splitScreen;
                    splitScreenMode = ss_mode_home;

                    break;
                }

                case encoder_button_long_press:
                {
                    if(currentSettingsMenuOption == settings_menu_option_save)
                    {

                    }

                    if (currentSettingsMenuOption == settings_menu_option_midi_config)
                    {
                        currentScreenMode = screenMode_calbration_warning;
                    }
                    //currentScreenMode = screenMode_settings;
                    Serial.println("Encoder Button Long Press");
                    break;
                }

                case lfo1_button:
                {
                    if (currentSettingsMenuOption == settings_menu_option_noteOn) currentSettingsMenuOption = settings_menu_option_midi_config;
                    else
                    {
                        currentSettingsMenuOption--;
                    }
                    selectedSettingsMenuOption = &settingsMenu[currentSettingsMenuOption];
                    
                    
                    Serial.print("settings decrement   currentSettingsMenuOption value =");
                    Serial.println(currentSettingsMenuOption);
                                
                    break;
                }

                case lfo2_button:
                {
                    currentSettingsMenuOption++;
                    if (currentSettingsMenuOption >= settings_menu_options_total) currentSettingsMenuOption = settings_menu_option_noteOn;
                    selectedSettingsMenuOption = &settingsMenu[currentSettingsMenuOption];
                    Serial.print("settings increment   currentSettingsMenuOption value =");
                    Serial.println(currentSettingsMenuOption);
  
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


            break;
        }


        case screenMode_calbration_warning:  //==============================================  Calibration Mode
        {

            switch (inputNumber)
            {
                case no_input:  break;

                case encoder_button_long_press:
                {
                    currentScreenMode = screenMode_calibrate;
                    break;
                }

                default:
                {
                    currentScreenMode = screenMode_settings;
                    break;
                }

            }
        }

        case screenMode_calibrate:  //==============================================  Calibration Mode
        {

            switch (inputNumber)
            {
                case no_input:  break;

                case encoder_button:
                {
                    currentScreenMode = screenMode_splitScreen;
                    splitScreenMode = ss_mode_home;

                    break;
                }

                case encoder_button_long_press:
                {
                    // if(currentSettingsMenuOption == settings_menu_option_save)
                    // {

                    // }

                    // if (currentSettingsMenuOption == settings_menu_option_midi_config)
                    // {
                    //     currentScreenMode = screenMode_calibrate;
                    // }
                    //currentScreenMode = screenMode_settings;
                    Serial.println("Encoder Button Long Press");
                    break;
                }

                case lfo1_button:
                {
                    if (currentSettingsMenuOption == settings_menu_option_noteOn) currentSettingsMenuOption = settings_menu_option_midi_config;
                    else
                    {
                        currentSettingsMenuOption--;
                    }
                    selectedSettingsMenuOption = &settingsMenu[currentSettingsMenuOption];
                    
                    
                    Serial.print("settings decrement   currentSettingsMenuOption value =");
                    Serial.println(currentSettingsMenuOption);
                                
                    break;
                }

                case lfo2_button:
                {
                    currentSettingsMenuOption++;
                    if (currentSettingsMenuOption >= settings_menu_options_total) currentSettingsMenuOption = settings_menu_option_noteOn;
                    selectedSettingsMenuOption = &settingsMenu[currentSettingsMenuOption];
                    Serial.print("settings increment   currentSettingsMenuOption value =");
                    Serial.println(currentSettingsMenuOption);
  
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

        }
 
        
        default:
        break;
    }


    

    drawCurrentScreenMode();

}

void displayService::newEncoderMovement(int32_t movement)
{
    if (currentScreenMode == screenMode_fullScreen)
    {

    }
    else if (currentScreenMode == screenMode_splitScreen)
    {
        selectedLFOmenuOption->value += movement;

        if (selectedLFOmenuOption->value > selectedLFOmenuOption->max) selectedLFOmenuOption->value = selectedLFOmenuOption->max;
        if (selectedLFOmenuOption->value < selectedLFOmenuOption->min) selectedLFOmenuOption->value = selectedLFOmenuOption->min;
        
        needsRedraw = true;
    }
    else if (currentScreenMode == screenMode_settings)
    {
        selectedSettingsMenuOption->value += movement;

        if (selectedSettingsMenuOption->value > selectedSettingsMenuOption->max) selectedSettingsMenuOption->value = selectedSettingsMenuOption->max;
        if (selectedSettingsMenuOption->value < selectedSettingsMenuOption->min) selectedSettingsMenuOption->value = selectedSettingsMenuOption->min;
        
        needsRedraw = true;
    }
    else if (currentScreenMode == screenMode_calibrate)
    {
        //selectedSettingsMenuOption->value += movement;

        // if (selectedSettingsMenuOption->value > selectedSettingsMenuOption->max) selectedSettingsMenuOption->value = selectedSettingsMenuOption->max;
        //if (selectedSettingsMenuOption->value < selectedSettingsMenuOption->min) selectedSettingsMenuOption->value = selectedSettingsMenuOption->min;
        
        needsRedraw = true;
    }
    else
    {

    }
    

    
    
    
}

void displayService::noteOnEvent(uint8_t whichLFO)
{
    if (noteOn[whichLFO] == true) return;
    
    else
    {
        noteOn[whichLFO] = true;
        drawCurrentScreenMode();
    }
}

void displayService::noteOffEvent(uint8_t whichLFO)
{
    if (noteOn[whichLFO] == false) return;

    else
    {
        noteOn[whichLFO] = false;
        drawCurrentScreenMode();
    }
}


void displayService::initLFOmenuOptions(void)
{   

    menuOption moNone  = {0,            0,           0,          "NONE"};
    menuOption moWave  = {sine,         1,   totalWaveforms,     "WAVE"};
    menuOption moRate  = {1,            1,         255,          "RATE"};
    menuOption moAmp   = {99,         -99,          99,          "AMP"};
    menuOption moPhase = {0,            0,         254,          "PHASE"};
    menuOption moDelay = {0,            0,         254,          "DELAY"};
  

    LFOmenu[LFO_1][lfo_menu_option_none]  = moNone;
    LFOmenu[LFO_1][lfo_menu_option_wave]  = moWave;
    LFOmenu[LFO_1][lfo_menu_option_rate]  = moRate;
    LFOmenu[LFO_1][lfo_menu_option_amp]   = moAmp;
    LFOmenu[LFO_1][lfo_menu_option_phase] = moPhase;
    LFOmenu[LFO_1][lfo_menu_option_delay] = moDelay;

    LFOmenu[LFO_2][lfo_menu_option_none]  = moNone;
    LFOmenu[LFO_2][lfo_menu_option_wave]  = moWave;
    LFOmenu[LFO_2][lfo_menu_option_rate]  = moRate;
    LFOmenu[LFO_2][lfo_menu_option_amp]   = moAmp;
    LFOmenu[LFO_2][lfo_menu_option_phase] = moPhase;
    LFOmenu[LFO_2][lfo_menu_option_delay] = moDelay;
}



void displayService::advanceLFOmenu(void)
{
    currentLFOmenuOption++;

    if (currentLFOmenuOption >= lfo_menu_options_total)
    {
        currentLFOmenuOption = 1;
    }

    //Serial.print("Advance menu = ");
    //Serial.println(currentMenuOption);

    selectedLFOmenuOption = &LFOmenu[currentLFOselected][currentLFOmenuOption];
    needsRedraw = true;
}

void displayService::clearLFOmenu(void)
{
    currentLFOmenuOption = lfo_menu_option_none;
    selectedLFOmenuOption = &LFOmenu[currentLFOselected][lfo_menu_option_none];
    needsRedraw = true;
}

void displayService::resetLFOmenu(void)
{
    currentLFOmenuOption = 1;
    selectedLFOmenuOption = &LFOmenu[currentLFOselected][1];
    needsRedraw = true;
}




void displayService::initSettingsMenuOptions(void)
{   
                            //value          min          max           name
    menuOption moNoteOn     = {0,            0,           1,     "Note On Flag"};
    menuOption moInvert     = {0,            0,           1,     "Invert"};
    //menuOption mo1          = {1,            1,         255,          " "};
    //menuOption mo2          = {99,         -99,          99,          " "};
    menuOption moSave       = {0,            0,         254,          "SAVE"};
    menuOption moMIDIconfig = {0,            0,         254,          "MIDI confg"};
  

    settingsMenu[settings_menu_option_noteOn]      = moNoteOn;
    settingsMenu[settings_menu_option_invert]      = moInvert;
    settingsMenu[settings_menu_option_save]        = moSave;
    settingsMenu[settings_menu_option_midi_config] = moMIDIconfig;

}


void displayService::drawCurrentScreenMode(void)
{
    //screen->stopScroll();
    checkForInvertedDisplay();

    switch (currentScreenMode)
    {
        case screenMode_fullScreen:
        {
            showFullScreen(currentScreen);
            break;
        }

        case screenMode_splitScreen:
        {
            splitScreen();
            break;
        }

        case screenMode_settings:
        {
            settingsScreen();
            break;
        }

        case screenMode_calbration_warning:
        {
            
            calibrationWarningScreen();
            break;
        }
        
        
        case screenMode_calibrate:
        {
            
            MIDIconfigMenu();
            //calibrationScreen();
            break;
        }
    
    
    default:
        break;
    }


}

void displayService::showFullScreen(byte screenNumber)
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
                drawLFOmenu();
                
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
    uint8_t currentWaveform = LFOmenu[whichLFO][lfo_menu_option_wave].value;

    uint8_t rate  = LFOmenu[whichLFO][lfo_menu_option_rate].value;
    uint8_t amp   = LFOmenu[whichLFO][lfo_menu_option_amp].value;
    uint8_t phase = LFOmenu[whichLFO][lfo_menu_option_phase].value;
    uint8_t delay = LFOmenu[whichLFO][lfo_menu_option_delay].value;

    
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

void displayService::drawLFOmenu(void)
{
    //Serial.print(" lfo  =  ");
    //Serial.println(currentLFOselected);
    
    //Serial.print(" option  =  ");
    //Serial.println(menu[currentLFOselected][currentMenuOption].name);

    if (currentLFOmenuOption == lfo_menu_option_none)
    {
        return;
    }

    
    //  get value
    int16_t value = LFOmenu[currentLFOselected][currentLFOmenuOption].value;
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
        screen->string(optionX, Y1, LFOmenu[currentLFOselected][currentLFOmenuOption].name, 16, 1);
        // print value  
        screen->string16pix(valueX, Y1, str);

    }

    if (currentLFOselected == LFO_2)
    {
        screen->string(optionX, Y2, LFOmenu[currentLFOselected][currentLFOmenuOption].name, 16, 1);
        // print value  
        screen->string16pix(valueX, Y2, str);
        
    }
    

    
}


static const char* MIDIconfigText = "    MIDI SETUP     ";
static const char* warningLine1 =   "input MIDI data per";
static const char* warningLine2 =   "channel. full sweep.";
static const char* warningLine3 =   "long press=continue";



void displayService::calibrationWarningScreen(void)
{
    screen->clear();

    screen->string(9, 0, MIDIconfigText, 12, 1);
    screen->string(3, 15, warningLine1, 12, 1);
    screen->string(1, 28, warningLine2, 12, 1);
    screen->string(3, 50, warningLine3, 12, 1);




    needsRedraw = true;


}

static const char* oneText =   "1";
static const char* twoText =   "2";


static const char* LFOtext =  "LFO";
static const char* minmaxText =   "min         max ";
static const char* ccText =   "ch    cc";
static const char* noteText =   "note on";
static const char* resText =   "res";
static const char* fourteenText =   "14";
static const char* sevenText =   "7";
static const char* bitText =   "bit";


void displayService::MIDIconfigMenu(void)
{
    screen->clear();

    //  LFO: ?
    screen->string16pix(27, 2, twoText);   // print value
    screen->string(4, 2, LFOtext, 12, 1);  // print LFO:
    //screen->drawRect(0, 18, 51, 18, 1);    // border

    screen->string(50, 0, ccText, 12, 1); // print cc ch
    
    static const char* ch =   "16";
    screen->string(64, 0, ch, 16, 1); // print cc ch

    static const char* cc =   "127";
    screen->string(100, 0, cc, 16, 1); // print cc ch

    


    screen->string(2, 18, resText, 12, 1); // print cc ch
    screen->string(22, 19, fourteenText, 16, 1); // print cc ch
    screen->string(40, 24, bitText, 12, 1); // print cc ch



    screen->string(74, 20, noteText, 12, 1); // print cc ch
    screen->drawRect(64, 33, 62, 12, 1); // note on box



    
    screen->string(2, 50, minmaxText, 12, 1);  //  print min max

    static const char* longvalueText =   "16535";
    static const char* shortvalueText =   "127";

    screen->string(27, 54, longvalueText, 12, 1); // print min val
    //screen->string(98, 42, longvalueText, 12, 1); // print max val

    //screen->string(26, 38, shortvalueText, 16, 1); // print min val
    
    screen->string(98, 50, shortvalueText, 16, 1); // print max val
    //screen->string(98, 38, shortvalueText, 16, 1); // print max val



    screen->writeLine(0, 17, 127, 17, 1); // first horiz line
    screen->drawRect(2, 49, 124, 12, 1); // bar


    //screen->string(0, 10, oneText, 12, 1);
    //screen->string(0, 22, one1Text, 12, 1);
    //screen->string(0, 40, twoText, 12, 1);
    //screen->string(0, 52, one1Text, 12, 1);

    




    needsRedraw = true;


}



void displayService::drawNotifications(void)
{
    static const char* noteOnText = " NOTE ON ";

    if (settingsMenu[settings_menu_option_noteOn].value == false) return;
    
    if (noteOn[LFO_1] == true)
    {
        if ( (splitScreenMode == ss_mode_home) ||
             (splitScreenMode == ss_mode_menu  && currentLFOselected == LFO_1) )
        {
            screen->string(38, 10, noteOnText, 12, 0);   
        }
        else
        {
            screen->drawRect(4, 28, 120, 24, 1);
        }  
    }
    
    if (noteOn[LFO_2] == true)
    {
        if ( (splitScreenMode == ss_mode_home) ||
             (splitScreenMode == ss_mode_menu  && currentLFOselected == LFO_2) )
        {
            screen->string(38, 42, noteOnText, 12, 0);   
        }
        else
        {
            screen->drawRect(4, 60, 120, 24, 1);
        }
    }
}


void displayService::checkForInvertedDisplay(void)  // return true if inverted
{
    if (displayIsInverted == settingsMenu[settings_menu_option_invert].value) return;
    else
    {
        if (settingsMenu[settings_menu_option_invert].value == true)
        {
            screen->invert();
            displayIsInverted = true;
        }
        if (settingsMenu[settings_menu_option_invert].value == false)
        {
            screen->uninvert();
            displayIsInverted = false;
        }

    }
}

static const char* settingsText = "SETTINGS";
static const char* onText =  " ON  ";
static const char* offText = " OFF ";
static const char* longPressText = "LONG PRESS";



void displayService::settingsScreen(void)
{
    

    screen->clear();

    screen->string(40, 0, settingsText, 12, 1);

    screen->string(4, 15, settingsMenu[settings_menu_option_noteOn].name, 12, 1);
    screen->string(4, 27, settingsMenu[settings_menu_option_invert].name, 12, 1);
    screen->string(4, 39, settingsMenu[settings_menu_option_save].name, 12, 1);
    screen->string(4, 51, settingsMenu[settings_menu_option_midi_config].name, 12, 1);




    const char* msg = "0";

    if      (settingsMenu[settings_menu_option_noteOn].value == true)   msg = onText;
    else if (settingsMenu[settings_menu_option_noteOn].value == false)  msg = offText;

    screen->string(96, 15, msg, 12, 1);


    if      (settingsMenu[settings_menu_option_invert].value == true)   msg = onText;
    else if (settingsMenu[settings_menu_option_invert].value == false)  msg = offText;

    screen->string(96, 27, msg, 12, 1);



    if (settingsMenu[currentSettingsMenuOption].value == true)
    {
        msg = onText;
    }
    else if (settingsMenu[currentSettingsMenuOption].value == false)
    {
        msg = offText;
    }



    screen->string(96, ((currentSettingsMenuOption * 12) + 15), msg, 12, 0);

    if (currentSettingsMenuOption == settings_menu_option_save)  screen->string(64, 39, longPressText, 12, 0);
    if (currentSettingsMenuOption == settings_menu_option_midi_config)  screen->string(64, 51, longPressText, 12, 0);
    

    

    

    needsRedraw = true;

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


