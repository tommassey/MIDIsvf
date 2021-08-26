#include "inputManager.h"




//  Buttons
//===========
Bounce deBounce1; 
Bounce deBounce2; 
Bounce deBounce3; 

ButtonDriver button1 = ButtonDriver(&deBounce1, BUTTON_1_PIN, INPUT_PULLUP, DEBOUNCE_TIME_MILLIS);   //  LFO1 button
ButtonDriver button2 = ButtonDriver(&deBounce2, BUTTON_2_PIN, INPUT_PULLUP, DEBOUNCE_TIME_MILLIS);   //  LFO2 button)
ButtonDriver button3 = ButtonDriver(&deBounce3, BUTTON_3_PIN, INPUT_PULLUP, DEBOUNCE_TIME_MILLIS);   //  Encoder button)

ButtonService buttonService;


//  Encoders
//===========
Encoder encoder = Encoder(ENCODER_PIN_A, ENCODER_PIN_B);

EncoderService menuEncoder = EncoderService(&encoder);


void setupButtons(void);




void inputManager_init(void)
{
    Serial.println("input managaer init");

    pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);

    setupButtons();
}



void checkButtons()
{
    //buttonService.checkAllButtons();
}

void checkEncoders()
{
    menuEncoder.update();
}


void setupButtons(void)
{
    buttonService.addButton(&button1);
    buttonService.addButton(&button2);
    buttonService.addButton(&button3);
}