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
    //screenMode_splitScreen,
    screenMode_settings,
    screenMode_settingsQuick,
    screenMode_calbration_warning,
    screenMode_calibrate,
    screenMode_ss_mode_home,
    screenMode_ss_mode_menu_lfo1,
    screenMode_ss_mode_menu_lfo2
};



struct MIDIprofile
{
    byte channel = 0;
    bool is14bit = false;
    uint16_t value = 0;
    uint16_t min = 0;
    uint16_t max = 0;

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
    uint8_t enumValue;
};







class displayService
{
    private:
        oled* screen;

        
        bool needsRedraw = false;   //  set this true after doing anything that requires screen to be refreshed
        bool displayIsInverted = false;

        byte currentScreen = 0;
        
        
        byte LFOshape[LFO_total] = { sine };
        byte LFOrate[LFO_total] = { 0 };
        byte LFOamp[LFO_total] = { 0 };
        byte LFOphase[LFO_total] = { 0 };
        byte LFOdelay[LFO_total] = { 0 };

        uint8_t sinCycles = 0;
        uint8_t sinCyclesPrevious = 0;


        

        //  for notifications
        bool noteOn[LFO_total] = { true, true };
        void drawNoteOn(uint8_t whichLFO);
        
        void drawCurrentScreenMode(void);

        //void initLFOmenuOptions(void);

        // void advanceLFOmenu(uint8_t whichLFO);
        // void resetLFOmenu(uint8_t whichLFO);
        // void clearLFOmenu(uint8_t whichLFO);


        


        //============================  settingsQuickView

        
        // //============================  LFO menus
        
        //menuOption LFOmenu[LFO_total][settings_menu_options_total];
        //menuOption* selectedLFOMenuOption = &settingsMenu[settings_menu_option_noteOn];




        // //============================  settings menu
        
        //menuOption settingsMenu[settings_menu_options_total];
        //menuOption* selectedSettingsMenuOption = &settingsMenu[settings_menu_option_noteOn];

        // uint8_t currentSettingsMenuOption = settings_menu_option_noteOn;
        
        
        void initSettingsMenuOptions(void);

        void checkForInvertedDisplay(void);  // return true if inverted

        //============================ MIDI config menu

        MIDIprofile MIDIdata[MIDIchannel_total];

        







    public:
        displayService(oled* screenPtr);
        ~displayService();

        void clear(void);
        void redraw(void);   //  called externally when the screen needs to be refreshed

        

        void checkForRedraw(void);


        void updateSetting(uint8_t whichSetting, int16_t value);
        void updateLFO(uint8_t whichLFO, uint8_t whichValue, int16_t value);

    


        
        //  scrrenmodes
        void showFullScreen(byte screenNumber);
        void splitScreen(void);
        void settingsQuick(void);
        void settingsScreen(uint8_t currentOption);
        void calibrationWarningScreen(void);
        void MIDIconfigMenu(void);

        void noteOnEvent(uint8_t whichLFO);
        void noteOffEvent(uint8_t whichLFO);


        void drawBorders(uint8_t whichLFO);
        void drawLFOs(byte currentLFOselected);
        void drawCurrentWaveform(uint8_t whichLFO);
        void drawLFOmenu(uint8_t whichLFO, uint8_t currentOption, int16_t value);
        void drawNotifications(bool noteOnShows);






        void setLFOwave(uint8_t whichLFO, byte waveForm);
        void setLFOrate(uint8_t whichLFO, byte rate);
        void setLFOamp(uint8_t whichLFO, byte amp);
        void setLFOphase(uint8_t whichLFO, byte phase);
        void setLFOdelay(uint8_t whichLFO, byte phase);

        
        void receiveMIDIchannel(uint8_t whichLFO, uint16_t val);
        void receiveMIDIcc(uint8_t whichLFO, uint16_t val);
        void receiveMIDIvalue(uint8_t whichLFO, uint16_t val);
        void receiveMIDIis14bit(uint8_t whichLFO, uint16_t val);
        void receiveMIDIis7bit(uint8_t whichLFO, uint16_t val);
        


};








#endif //  DISPLAY_SERVICE_H