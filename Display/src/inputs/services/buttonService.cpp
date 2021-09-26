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


int8_t ButtonService::checkAllButtons(void)   //  returns a value from InputNames enum showing which button changed
{
  for (uint8_t i = 0; i < buttonCount; i++)
  {
      ButtonEvent newPress = checkButton(i);

      

      if (newPress.pressType != 0)
      {
          status[i] = newPress.pressType;
          presses[i]++;
          changeFlag = i;

          if (newPress.pressType == longPress)
          {
            Serial.println("LONG PRESS");
            changeFlag = encoder_button_long_press;
          }
      }
  }
  
    return changeFlag;
}
