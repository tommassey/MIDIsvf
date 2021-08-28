#ifndef BUTTON_SERVICE_H
#define BUTTON_SERVICE_H

#include <Arduino.h>
#include "drivers/buttonDriver.h"

#define MAX_BUTTONS 4



struct ButtonEvent
{
    uint8_t whichButton = 0;
    uint8_t pressType = noPress;

};

class ButtonService
{
    public:
        ButtonService(/* args */);

        int8_t changeFlag = -1;   // updates to indicate which button changed. -1 is no change because of 0 index arrays

        void addButton(ButtonDriver* newButton);
        ButtonEvent checkButton(uint8_t whichButton);
        int8_t checkAllButtons(void);

        uint8_t buttonCount = 0;
        uint8_t status[MAX_BUTTONS];   //  holds status of all buttons
        uint32_t presses[MAX_BUTTONS] = {0};  //  total presses

    private:

        

        ButtonDriver* button[MAX_BUTTONS];   // pointers to the buttonDriver objects
        




};







#endif //  BUTTON_SERVICE_H