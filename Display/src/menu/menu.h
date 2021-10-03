#ifndef  MENU_H
#define MENU_H

#include <Arduino.h>
#include "../../Shared/commandList.h"
#include <display/displayService.h>
#include "menuOptions.h"





void initMenu(displayService* disp);



void newEncoderMovement(int32_t movement);
void actOnInputs(int8_t inputNumber);



void initLFOmenuOptions(void);
void advanceLFOmenu(uint8_t whichLFO);
void clearLFOmenu(uint8_t whichLFO);
void resetLFOmenu(uint8_t whichLFO);
void initSettingsMenuOptions(void);



#endif //  MENU_H