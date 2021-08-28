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


void printButtonChanges(void)
{
    Serial.print("BUTTONS  ");

        for (uint8_t i = 0; i < buttonService.buttonCount; i++)
        {
            Serial.print(i);
            Serial.print(" = ");
            Serial.print(buttonService.presses[i]);
            Serial.print("   ");
        }

        Serial.println();
}


void printEncoderChanges(void)
{
    Serial.print("ENCODER = ");
    Serial.println(menuEncoder.getValue());
}



int8_t checkInputs()
{
    int8_t newInput = buttonService.checkAllButtons();

    if ((newInput > no_input) && (newInput < max_buttons))   //  if a button changed
    {
        printButtonChanges();
        return newInput;        
    }
   

    if (menuEncoder.changeFlag == true)   //  if encoder changed
    {
        printEncoderChanges();
        menuEncoder.changeFlag = false;

        return menu_encoder;        
    }

    else
    {
        return no_input;
    }
}



void checkEncoders()
{
    menuEncoder.update();
}

void resetInputFlag(void)
{
    buttonService.changeFlag = no_input;
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