#include "buttonService.h"


ButtonService::ButtonService()
{
    Serial.println("button service init");

}

void ButtonService::addButton(ButtonDriver* newButton)
{
    if (buttonCount < MAX_BUTTONS)
    {
        button[buttonCount] = newButton;
        buttonCount++;
    }
    
    else
    {
        Serial.println("cannot add button to ButtonService - increase MAX_BUTTONS define");
    }
    
}

ButtonEvent ButtonService::checkButton(uint8_t whichButton)
{
    uint8_t newButtonPress = button[whichButton]->check();
    
    return {whichButton, newButtonPress};
}


void ButtonService::checkAllButtons(void)
{
  for (uint8_t i = 0; i < buttonCount; i++)
  {
      ButtonEvent newPress = checkButton(i);

      if (newPress.pressType != 0)
      {
          status[i] = newPress.pressType;
          presses[i]++;
          changeFlag = i;
      }
  }
  

  if (changeFlag > -1)
  {
    Serial.print("BUTTONS  ");

    for (uint8_t i = 0; i < buttonCount; i++)
    {
        Serial.print(i);
        Serial.print(" = ");
        Serial.print(presses[i]);
        Serial.print("   ");
    }

    Serial.println();
    changeFlag = -1; 
  }
}
