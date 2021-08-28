#ifndef DISPLAY_SERVICE_H
#define DISPLAY_SERVICE_H

#include <Arduino.h>
#include "drivers/oled.h"
#include "../../Shared/commandList.h"

#include "inputs/inputNames.h"

#define TOTAL_LFOS 2

enum splitScreenModes
{
    ss_mode_home,
    ss_mode_menu

};

enum menuOptions
{
    menu_option_none,
    menu_option_rate,
    menu_option_amp,
    menu_option_phase,
    menu_option_reset,
    total_menu_options

};

struct menuOption
{
    int16_t value;
    int16_t min;
    int16_t max;
    char name[6];

} ;

class displayService
{
    private:
        oled* screen;

        byte currentScreen = 0;
        
        byte LFOshape = sine;
        byte LFOrate = 0;
        byte LFOamp = 0;
        byte LFOphase = 0;

        uint8_t sinCycles = 0;
        uint8_t sinCyclesPrevious = 0;


        // splitScreen

        uint8_t currentLFOselected = 1;
        uint8_t splitScreenMode = ss_mode_home;
        uint8_t currentMenuOption = menu_option_none;

        menuOption menu[total_menu_options] = 
        {
            {0, 0, 0, "NONE"},
            {10, 0, 0, "RATE"},
            {120, 0, 0, "AMP"},
            {50, 0, 0, "PHASE"},
            {70, 0, 0, "35467"},  // reset

        };

        void advanceMenu();


        void drawBorders(void);
        void drawLFO1(void);
        void drawMenu(void);




        void updateLFO();


    public:
        displayService(oled* screenPtr);
        ~displayService();

        void actOnInputs(int8_t inputNumber);

        void showScreen(byte screenNumber);
        void splitScreen(void);

        


        void setLFOwave(byte waveForm);
        void setLFOrate(byte rate);
        void setLFOamp(byte amp);
        void setLFOphase(byte phase);


};








#endif //  DISPLAY_SERVICE_H