#include "inputManager.h"




//  Buttons
//===========
Bounce deBounce0; 
Bounce deBounce1; 
Bounce deBounce2; 

ButtonDriver button0 = ButtonDriver(&deBounce0, BUTTON_3_PIN, INPUT_PULLUP, DEBOUNCE_TIME_MILLIS);   //  Encoder button)
ButtonDriver button1 = ButtonDriver(&deBounce1, BUTTON_1_PIN, INPUT_PULLUP, DEBOUNCE_TIME_MILLIS);   //  LFO1 button
ButtonDriver button2 = ButtonDriver(&deBounce2, BUTTON_2_PIN, INPUT_PULLUP, DEBOUNCE_TIME_MILLIS);   //  LFO2 button)


ButtonService buttonService;


//  Encoders
//===========
Encoder encoder = Encoder(ENCODER_PIN_A, ENCODER_PIN_B);

EncoderService menuEncoder = EncoderService(&encoder);


void setupButtons(void);
void setupEncoders(void);




void inputManager_init(void)
{
    Serial.println("input managaer init");

    pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);

    setupButtons();
    setupEncoders();
}



void checkButtons()
{
    buttonService.checkAllButtons();
}

void checkEncoders()
{
    menuEncoder.update();
}


void setupButtons(void)
{
    buttonService.addButton(&button0);
    buttonService.addButton(&button1);
    buttonService.addButton(&button2);
}

void setupEncoders(void)
{
    encoder.read();  //  just a read to make sure object has been init'd
}