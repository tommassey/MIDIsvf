#include "menu.h"

displayService* displayServPtr;


byte currentScreenMode = screenMode_ss_mode_home;

//============================  LFO menu
uint8_t currentLFOselected = LFO_none;
uint8_t currentLFOmenuOption[LFO_total] = { lfo_menu_option_none };

menuOption LFOmenu[LFO_total][lfo_menu_options_total];
menuOption* selectedLFOmenuOption = &LFOmenu[currentLFOselected][lfo_menu_option_none];


//============================  settings menu
uint8_t currentSettingsMenuOption = settings_menu_option_noteOn;

menuOption settingsMenu[settings_menu_options_total];
menuOption* selectedSettingsMenuOption = &settingsMenu[settings_menu_option_noteOn];




void drawCurrentScreenMode(void);






//==================================================================================   INPUTS

void newEncoderMovement(int32_t movement)
{
    switch (currentScreenMode)
    {
        case screenMode_fullScreen:
        {
            break;
        }

        case screenMode_ss_mode_home:
        {
            break;
        }

        case screenMode_ss_mode_menu_lfo1:
        {
            selectedLFOmenuOption->value += movement;

            if (selectedLFOmenuOption->value > selectedLFOmenuOption->max) selectedLFOmenuOption->value = selectedLFOmenuOption->max;
            if (selectedLFOmenuOption->value < selectedLFOmenuOption->min) selectedLFOmenuOption->value = selectedLFOmenuOption->min;
            displayServPtr->updateLFO(LFO_1, selectedLFOmenuOption->enumValue, selectedLFOmenuOption->value);

            drawCurrentScreenMode();
            break;
        }

        case screenMode_ss_mode_menu_lfo2:
        {
            selectedLFOmenuOption->value += movement;

            if (selectedLFOmenuOption->value > selectedLFOmenuOption->max) selectedLFOmenuOption->value = selectedLFOmenuOption->max;
            if (selectedLFOmenuOption->value < selectedLFOmenuOption->min) selectedLFOmenuOption->value = selectedLFOmenuOption->min;
            displayServPtr->updateLFO(LFO_2, selectedLFOmenuOption->enumValue, selectedLFOmenuOption->value);

            drawCurrentScreenMode();
            break;
        }

        case screenMode_settingsQuick:
        {
            break;
        }

        case screenMode_settings:
        {
            selectedSettingsMenuOption->value += movement;

            if (selectedSettingsMenuOption->value > selectedSettingsMenuOption->max) selectedSettingsMenuOption->value = selectedSettingsMenuOption->max;
            if (selectedSettingsMenuOption->value < selectedSettingsMenuOption->min) selectedSettingsMenuOption->value = selectedSettingsMenuOption->min;
            displayServPtr->updateSetting(selectedSettingsMenuOption->enumValue, selectedSettingsMenuOption->value);
        
            drawCurrentScreenMode();
            break;
        }

        case screenMode_calbration_warning:
        {
            break;
        }

        case screenMode_calibrate:
        {
            break;
        }
   
        default:  break;
    }
}


void actOnInputs(int8_t inputNumber)
{ 
    //Serial.print("display serv input number = ");
    //Serial.println(inputNumber);

    switch (currentScreenMode)
    {
        case screenMode_fullScreen:  //=====================================================  Fullscreen Mode
        {
            switch (inputNumber)
            {
                case no_input:  break;

                case encoder_button:
                {
                    currentScreenMode = screenMode_ss_mode_home;
                    currentLFOselected = LFO_none;
                    clearLFOmenu(LFO_1);
                    clearLFOmenu(LFO_2);

                    break;
                }

                case encoder_button_long_press:
                {
                    Serial.println("Encoder Button Long Press");
                    break;
                }

                case lfo1_button:
                {
                    currentScreenMode = screenMode_ss_mode_menu_lfo1;
                    resetLFOmenu(LFO_1);
                    resetLFOmenu(LFO_2);
                    break;
                }

                case lfo2_button:
                {
                    currentScreenMode = screenMode_ss_mode_menu_lfo2;
                    resetLFOmenu(LFO_1);
                    resetLFOmenu(LFO_2);
                    
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

        case screenMode_ss_mode_home:  //===================================================  Splitscreen Home
        {
            switch (inputNumber)
            {
                case no_input:  break;

                case encoder_button:
                {
                    currentScreenMode = screenMode_settingsQuick;
                    break;
                }

                case encoder_button_long_press:
                {
                    currentScreenMode = screenMode_settings;
                    break;
                }

                case lfo1_button:
                {
                    currentLFOselected = LFO_1;
                    advanceLFOmenu(LFO_1);
                    currentScreenMode = screenMode_ss_mode_menu_lfo1;                    

                    break;
                }

                case lfo2_button:
                {
                    if (currentLFOselected == LFO_1) resetLFOmenu(LFO_1);
                    currentLFOselected = LFO_2;
                    advanceLFOmenu(LFO_2);
                    currentScreenMode = screenMode_ss_mode_menu_lfo2;
                    
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

        case screenMode_ss_mode_menu_lfo1:  //==============================================  Splitscreen LFO1 selected
        {
            switch (inputNumber)
            {
                case no_input:  break;

                case encoder_button:
                {
                    currentScreenMode = screenMode_ss_mode_home;
                    currentLFOselected = LFO_none;
                    break;
                }

                case encoder_button_long_press:
                {
                    currentScreenMode = screenMode_settings;
                    break;
                }

                case lfo1_button:
                {
                    advanceLFOmenu(LFO_1);
                    break;
                }

                case lfo2_button:
                {
                    currentLFOselected = LFO_2;
                    currentScreenMode = screenMode_ss_mode_menu_lfo2;
                    resetLFOmenu(LFO_2);                  
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

        case screenMode_ss_mode_menu_lfo2:  //==============================================  Splitscreen LFO2 selected
        {
            switch (inputNumber)
            {
                case no_input:  break;

                case encoder_button:
                {
                    currentScreenMode = screenMode_ss_mode_home;
                    currentLFOselected = LFO_none;
                    break;
                }

                case encoder_button_long_press:
                {
                    currentScreenMode = screenMode_settings;
                    break;
                }

                case lfo1_button:
                {
                    currentLFOselected = LFO_1;
                    currentScreenMode = screenMode_ss_mode_menu_lfo1;
                    resetLFOmenu(LFO_1);
                    break;
                }

                case lfo2_button:
                {
                    advanceLFOmenu(LFO_2);
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

        case screenMode_settings:  //=======================================================  Settings Mode
        {
            switch (inputNumber)
            {
                case no_input:  break;

                case encoder_button:
                {
                    currentScreenMode = screenMode_ss_mode_home;
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

        case screenMode_settingsQuick:
        {
            switch (inputNumber)
            {
                case no_input:  break;

                case encoder_button:
                {
                    //currentLFOselected = LFO_none;
                    //clearLFOmenu();
                    currentScreenMode = screenMode_ss_mode_home;
                    
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
                    currentScreenMode = screenMode_ss_mode_home;                  
                    break;
                }

                case lfo2_button:
                {
                    currentScreenMode = screenMode_ss_mode_home;
                    break;
                }
                
                case menu_encoder:
                {
                    currentScreenMode = screenMode_ss_mode_home;
                    break;
                }
            
                default:
                    break;
            }

            break;
        }
      
        case screenMode_calbration_warning:  //==============================================  Enter Calibration Mode Warning
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

        case screenMode_calibrate:  //=======================================================  Calibration Mode
        {

            switch (inputNumber)
            {
                case no_input:  break;

                case encoder_button:
                {
                    currentScreenMode = screenMode_ss_mode_home;

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
 
        default: break;
    }

    drawCurrentScreenMode();

}





//==================================================================================   MENUS


void initMenu(displayService* disp)
{
    displayServPtr = disp;
    initLFOmenuOptions();
    initSettingsMenuOptions();
}


//=======================================================  LFO MENU

void initLFOmenuOptions(void)
{   

    menuOption moNone  = {0,            0,           0,          lfo_menu_option_none };
    menuOption moWave  = {sine,         1,     squaree,          lfo_menu_option_wave };
    menuOption moRate  = {1,            1,         255,          lfo_menu_option_rate};
    menuOption moAmp   = {99,         -99,          99,          lfo_menu_option_amp};
    menuOption moPhase = {0,            0,         254,          lfo_menu_option_phase};
    menuOption moDelay = {0,            0,         254,          lfo_menu_option_delay};
  

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


    displayServPtr->setLFOwave (LFO_1, LFOmenu[LFO_1][lfo_menu_option_wave].value);
    displayServPtr->setLFOrate (LFO_1, LFOmenu[LFO_1][lfo_menu_option_rate].value);
    displayServPtr->setLFOamp  (LFO_1, LFOmenu[LFO_1][lfo_menu_option_amp].value);
    displayServPtr->setLFOphase(LFO_1, LFOmenu[LFO_1][lfo_menu_option_phase].value);
    displayServPtr->setLFOdelay(LFO_1, LFOmenu[LFO_1][lfo_menu_option_delay].value);

    displayServPtr->setLFOwave (LFO_2, LFOmenu[LFO_2][lfo_menu_option_wave].value);
    displayServPtr->setLFOrate (LFO_2, LFOmenu[LFO_2][lfo_menu_option_rate].value);
    displayServPtr->setLFOamp  (LFO_2, LFOmenu[LFO_2][lfo_menu_option_amp].value);
    displayServPtr->setLFOphase(LFO_2, LFOmenu[LFO_2][lfo_menu_option_phase].value);
    displayServPtr->setLFOdelay(LFO_2, LFOmenu[LFO_2][lfo_menu_option_delay].value);
}



void advanceLFOmenu(uint8_t whichLFO)
{
    currentLFOmenuOption[whichLFO]++;

    if (currentLFOmenuOption[whichLFO] >= lfo_menu_options_total)
    {
        currentLFOmenuOption[whichLFO] = 1;
    }

    Serial.print("LFO ");
    Serial.print(whichLFO);
    Serial.print("   menu option = ");
    Serial.println(currentLFOmenuOption[whichLFO]);

    selectedLFOmenuOption = &LFOmenu[currentLFOselected][currentLFOmenuOption[whichLFO]];
}

void clearLFOmenu(uint8_t whichLFO)
{
    Serial.print("clear menu LFO ");
    Serial.print(whichLFO);

    currentLFOmenuOption[whichLFO] = lfo_menu_option_none;
    selectedLFOmenuOption = &LFOmenu[currentLFOselected][lfo_menu_option_none];
}

void resetLFOmenu(uint8_t whichLFO)
{
    Serial.print("reset menu LFO ");
    Serial.print(whichLFO);

    currentLFOmenuOption[whichLFO] = 1;
    selectedLFOmenuOption = &LFOmenu[currentLFOselected][1];
}





//=======================================================  SETTINGS MENU

void initSettingsMenuOptions(void)
{   
                            //value          min          max           name
    menuOption moNoteOn     = {0,            0,           1,     settings_menu_option_noteOn};
    menuOption moInvert     = {0,            0,           1,     settings_menu_option_invert};
    menuOption moSave       = {0,            0,         254,     settings_menu_option_save};
    menuOption moMIDIconfig = {0,            0,         254,     settings_menu_option_midi_config};
  

    settingsMenu[settings_menu_option_noteOn]      = moNoteOn;
    settingsMenu[settings_menu_option_invert]      = moInvert;
    settingsMenu[settings_menu_option_save]        = moSave;
    settingsMenu[settings_menu_option_midi_config] = moMIDIconfig;


    displayServPtr->updateSetting(settings_menu_option_noteOn, settingsMenu[settings_menu_option_noteOn].value);
    displayServPtr->updateSetting(settings_menu_option_invert, settingsMenu[settings_menu_option_invert].value);


}











//==================================================================================   SCREEN LOGIC




void showFullScreen(byte screenNumber)
{
    Serial.print("show screen: ");
    Serial.println(screenNumber);

    switch (screenNumber)
    {
    case 1:
        //displayServPtr->showFullScreen(1);
        //screen->splashScreen();
        break;
    
    default:
        break;
    }

    displayServPtr->redraw();

}



void splitScreen(byte currentLFOselected)
{
    displayServPtr->clear();

    switch (currentScreenMode)
    {
        case screenMode_ss_mode_home:
            {
                displayServPtr->drawLFOs(currentLFOselected);
                //screen->startScroll();
                break;
            }
        
        case screenMode_ss_mode_menu_lfo1:
        {
            displayServPtr->drawLFOs(currentLFOselected);
            displayServPtr->drawLFOmenu(currentLFOselected, currentLFOmenuOption[currentLFOselected], selectedLFOmenuOption->value);
            
            break;
        }

        case screenMode_ss_mode_menu_lfo2:
        {
            displayServPtr->drawLFOs(currentLFOselected);
            displayServPtr->drawLFOmenu(currentLFOselected, currentLFOmenuOption[currentLFOselected], selectedLFOmenuOption->value);
            
            break;
        }
            
        
        default:
            break;
    }

    displayServPtr->drawBorders(currentLFOselected);
    displayServPtr->drawNotifications((bool)settingsMenu[settings_menu_option_noteOn].value);
    
    

    displayServPtr->redraw();
}



void drawCurrentScreenMode(void)
{
    //screen->stopScroll();
    //displayServPtr->checkForInvertedDisplay();

    switch (currentScreenMode)
    {
        case screenMode_fullScreen:
        {
           // displayServPtr->showFullScreen(1);
            break;
        }

        case screenMode_ss_mode_home:
        case screenMode_ss_mode_menu_lfo1:
        case screenMode_ss_mode_menu_lfo2:
        {
            splitScreen(currentLFOselected);
            break;
        }

        case screenMode_settings:
        {
            displayServPtr->settingsScreen(currentSettingsMenuOption);
            break;
        }

        case screenMode_settingsQuick:
        {
            displayServPtr->settingsQuick();
            break;
        }

        case screenMode_calbration_warning:
        {
            
            displayServPtr->calibrationWarningScreen();
            break;
        }
        
        
        case screenMode_calibrate:
        {
            
            displayServPtr->MIDIconfigMenu();
            //calibrationScreen();
            break;
        }
    
    
    default:
        break;
    }


}

