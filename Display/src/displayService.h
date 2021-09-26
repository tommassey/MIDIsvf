#ifndef DISPLAY_SERVICE_H
#define DISPLAY_SERVICE_H

#include <Arduino.h>
#include "drivers/oled.h"
#include "../../Shared/commandList.h"

#include "inputs/inputNames.h"

#define TOTAL_LFOS 2


enum screenModes
{
    screenMode_fullScreen,
    screenMode_splitScreen,
    screenMode_settings,
    screenMode_calibrate
};

enum splitScreenModes
{
    ss_mode_home,
    ss_mode_menu

};

enum LFOmenuOptions
{
    lfo_menu_option_none,
    lfo_menu_option_wave,
    lfo_menu_option_amp,
    lfo_menu_option_phase,
    lfo_menu_option_delay,
    lfo_menu_option_rate,
    lfo_menu_options_total

};

enum settingsMenuOptions
{
    settings_menu_option_noteOn,
    settings_menu_option_invert,
    settings_menu_option_save,
    settings_menu_option_midi_config,
    settings_menu_options_total
};

struct menuOption
{
    int16_t value;
    int16_t min;
    int16_t max;
    char name[13];

} ;

class displayService
{
    private:
        oled* screen;


        bool needsRedraw = false;   //  set this high after doing anything that requires screen to be refreshed
        bool displayIsInverted = false;

        byte currentScreen = 0;
        byte currentScreenMode = screenMode_fullScreen;
        
        byte LFOshape = sine;
        byte LFOrate = 0;
        byte LFOamp = 0;
        byte LFOphase = 0;

        uint8_t sinCycles = 0;
        uint8_t sinCyclesPrevious = 0;


        // splitScreen

        uint8_t currentLFOselected = LFO_1;
        uint8_t splitScreenMode = ss_mode_home;
        uint8_t currentLFOmenuOption = lfo_menu_option_none;

        menuOption* selectedLFOmenuOption = &LFOmenu[currentLFOselected][lfo_menu_option_none];

        menuOption LFOmenu[LFO_total][lfo_menu_options_total];

        //  for notifications
        bool noteOn[LFO_total] = { true, true };
        
        void drawCurrentScreenMode(void);

        void initLFOmenuOptions(void);

        void advanceLFOmenu();
        void resetLFOmenu();
        void clearLFOmenu(void);


        void drawBorders(void);
        void drawLFOs(void);
        void drawCurrentWaveform(uint8_t whichLFO);
        void drawLFOmenu(void);
        void drawNotifications(void);


        //============================  settings menu
        
        menuOption settingsMenu[settings_menu_options_total];
        menuOption* selectedSettingsMenuOption = &settingsMenu[settings_menu_option_noteOn];

        uint8_t currentSettingsMenuOption = settings_menu_option_noteOn;
        
        void settingsScreen(void);
        void initSettingsMenuOptions(void);

        void checkForInvertedDisplay(void);  // return true if inverted








        void updateLFO();


    public:
        displayService(oled* screenPtr);
        ~displayService();

        void checkForRedraw(void);

        void actOnInputs(int8_t inputNumber);
        void newEncoderMovement(int32_t movement);
        void noteOnEvent(uint8_t whichLFO);
        void noteOffEvent(uint8_t whichLFO);

        void showFullScreen(byte screenNumber);
        void splitScreen(void);

        


        void setLFOwave(byte waveForm);
        void setLFOrate(byte rate);
        void setLFOamp(byte amp);
        void setLFOphase(byte phase);


};








#endif //  DISPLAY_SERVICE_H