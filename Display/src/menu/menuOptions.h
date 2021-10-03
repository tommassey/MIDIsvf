#ifndef MENU_OPTIONS_H
#define MENU_OPTIONS_H

#include <Arduino.h>


enum screenModes
{
    screenMode_fullScreen,
    screenMode_settings,
    screenMode_settingsQuick,
    screenMode_calbration_warning,
    screenMode_calibrate,
    screenMode_ss_mode_home,
    screenMode_ss_mode_menu_lfo1,
    screenMode_ss_mode_menu_lfo2
};


struct menuOption
{
    int16_t value;
    int16_t min;
    int16_t max;
    uint8_t enumValue;
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








#endif // MENU_OPTIONS_H