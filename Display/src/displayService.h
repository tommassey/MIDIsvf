#ifndef DISPLAY_SERVICE_H
#define DISPLAY_SERVICE_H

#include <Arduino.h>
#include "drivers/oled.h"
#include "../../Shared/commandList.h"

#include "inputs/inputNames.h"



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

        void drawBorders(void);




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