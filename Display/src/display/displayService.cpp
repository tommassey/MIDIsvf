#include "displayService.h"


displayService::displayService(oled* screenPtr)
{
    screen = screenPtr;

    
}

displayService::~displayService()
{
}

void displayService::clear(void)
{
    screen->clear();
}

void displayService::redraw(void)
{
    needsRedraw = true;
}

void displayService::checkForRedraw(void)
{
    if (needsRedraw == true)
    {
        screen->display();
        needsRedraw = false;
    }
}


void displayService::updateLFO(uint8_t whichLFO, uint8_t whichValue, int16_t value)
{
    switch (whichValue)
    {
        
        case lfo_menu_option_wave:
        {
            setLFOwave(whichLFO, ((byte)value));
            break;
        }
        
        case lfo_menu_option_amp:
        {
            setLFOamp(whichLFO, ((byte)value));
            break;
        }

        case lfo_menu_option_phase:
        {
            setLFOphase(whichLFO, ((byte)value));
            break;
        }

        case lfo_menu_option_delay:
        {
            setLFOdelay(whichLFO, ((byte)value));
            break;
        }

        case lfo_menu_option_rate:
        {
            setLFOrate(whichLFO, ((byte)value));
            break;
        }

        default: break;   
    
    }

}




void displayService::noteOnEvent(uint8_t whichLFO)
{
    if (noteOn[whichLFO] == true) return;
    
    else
    {
        noteOn[whichLFO] = true;
        needsRedraw = true;
    }
}

void displayService::noteOffEvent(uint8_t whichLFO)
{
    if (noteOn[whichLFO] == false) return;

    else
    {
        noteOn[whichLFO] = false;
        needsRedraw = true;
    }
}






//  0,0                         127,0
//  -----------------------------
//  |                           |
//  |                           |
//  |                           |
//  |                           |
//  |                           |
//  |                           |
//  |                           |
//  -----------------------------
// 0, 63                        127, 63

void displayService::drawBorders(byte currentLFOselected)
{
    //screen->drawRect(0, 0, 127, 31, 1);


    screen->drawRect(0, 32, 127, 32, 1);  // top frame
    screen->drawRect(0, 63, 127, 32, 1);  // bottom frame

    switch (currentLFOselected)
    {
        case LFO_1:
        {
            screen->drawRect(2, 29, 123, 27, 1);  // selected frame LFO1
            break;
        }

        case LFO_2:
        {
            screen->drawRect(2, 61, 123, 27, 1);  // selected frame LFO2
            break;
        }

    
    default:
        break;
    }

    






}


void displayService::drawLFOs(byte currentLFOselected)
{
    if (currentLFOselected == LFO_none)
    {
        drawCurrentWaveform(LFO_1);
        drawCurrentWaveform(LFO_2);
    }
    else
    {
        drawCurrentWaveform(currentLFOselected);
    }
}



void displayService::drawCurrentWaveform(uint8_t whichLFO)
{
    uint8_t currentWaveform = LFOshape[whichLFO];

    uint8_t rate  = LFOrate[whichLFO];
    uint8_t amp   = LFOamp[whichLFO];
    uint8_t phase = LFOphase[whichLFO];
    uint8_t delay = LFOdelay[whichLFO];

    
    switch (currentWaveform)
    {
        case sine:
        {
            screen->smallSine(whichLFO, rate, amp, phase, delay);
            break;    
        }

        case triangle:
        {
            screen->smallTriangle(whichLFO, rate, amp, phase, delay);
            break;    
        }

        case saw:
        {
            screen->smallSaw(whichLFO, rate, amp, phase, delay);
            break;    
        }

        case squaree:
        {
            screen->smallSquare(whichLFO, rate, amp, phase, delay);
            break;    
        }

    
    default:
        break;
    }
 }



uint8_t Y1 = 40;
uint8_t Y2 = 8;
uint8_t optionX = 12;
uint8_t valueX = 64;

const char* LFOmenuNames[lfo_menu_options_total] = 
{
    "NONE",
    "WAVE",
    "AMP",
    "PHASE",
    "DELAY",
    "RATE"
};

void displayService::drawLFOmenu(uint8_t whichLFO, uint8_t currentOption, int16_t value)
{
    //Serial.print(" lfo  =  ");
    //Serial.println(currentLFOselected);
    
    //Serial.print(" option  =  ");
    //Serial.println(menu[currentLFOselected][currentMenuOption].name);

    if (currentOption == lfo_menu_option_none)
    {
        return;
    }

    
    //  get value
    char str[5];
    itoa(value, str, 10);   //  int to a string

    // Serial.print(" value  =  ");
    // Serial.print(value);
    // Serial.print("    string = ");
    // Serial.println(str);


    // //  print option name
    // screen->string(12, 40, menu[currentLFOselected][currentMenuOption].name, 16, 1);

    // // print value
    // screen->string16pix(64, 40, str);

    if (whichLFO == LFO_1)
    {
        //  print option name
        screen->string(optionX, Y1, LFOmenuNames[currentOption], 16, 1);
        // print value  
        screen->string16pix(valueX, Y1, str);

    }

    if (whichLFO == LFO_2)
    {
        screen->string(optionX, Y2, LFOmenuNames[currentOption], 16, 1);
        // print value  
        screen->string16pix(valueX, Y2, str);
        
    }
    

    
}




static const char* MIDIconfigText = "    MIDI SETUP     ";
static const char* warningLine1 =   "input MIDI data per";
static const char* warningLine2 =   "channel. full sweep.";
static const char* warningLine3 =   "long press=continue";



void displayService::calibrationWarningScreen(void)
{
    screen->clear();

    screen->string(9, 0, MIDIconfigText, 12, 1);
    screen->string(3, 15, warningLine1, 12, 1);
    screen->string(1, 28, warningLine2, 12, 1);
    screen->string(3, 50, warningLine3, 12, 1);




    needsRedraw = true;


}

static const char* oneText =   "1";
static const char* twoText =   "2";


static const char* LFOtext =  "LFO";
static const char* minmaxText =   "min         max ";
static const char* chText =   "ch";
static const char* ccText =   "cc";
static const char* noteText =   "note on";
static const char* resText =   "res";
static const char* fourteenText =   "14";
static const char* sevenText =   "7";
static const char* bitText =   "bit";


void displayService::MIDIconfigMenu(void)
{
    screen->clear();

    //  LFO: ?
    screen->string16pix(27, 2, twoText);   // print value
    screen->string(4, 2, LFOtext, 12, 1);  // print LFO:
    //screen->drawRect(0, 18, 51, 18, 1);    // border

    screen->string(50, 0, ccText, 12, 1); // print cc ch
    
    static const char* ch =   "16";
    screen->string(64, 0, ch, 16, 1); // print cc ch

    static const char* cc =   "127";
    screen->string(100, 0, cc, 16, 1); // print cc ch

    


    screen->string(2, 18, resText, 12, 1); // print cc ch
    screen->string(22, 19, fourteenText, 16, 1); // print cc ch
    screen->string(40, 24, bitText, 12, 1); // print cc ch



    screen->string(74, 20, noteText, 12, 1); // print cc ch
    screen->drawRect(64, 33, 62, 12, 1); // note on box



    
    screen->string(2, 50, minmaxText, 12, 1);  //  print min max

    static const char* longvalueText =   "16535";
    static const char* shortvalueText =   "127";

    screen->string(27, 54, longvalueText, 12, 1); // print min val
    //screen->string(98, 42, longvalueText, 12, 1); // print max val

    //screen->string(26, 38, shortvalueText, 16, 1); // print min val
    
    screen->string(98, 50, shortvalueText, 16, 1); // print max val
    //screen->string(98, 38, shortvalueText, 16, 1); // print max val



    screen->writeLine(0, 17, 127, 17, 1); // first horiz line
    screen->drawRect(2, 49, 124, 12, 1); // bar


    //screen->string(0, 10, oneText, 12, 1);
    //screen->string(0, 22, one1Text, 12, 1);
    //screen->string(0, 40, twoText, 12, 1);
    //screen->string(0, 52, one1Text, 12, 1);

    




    needsRedraw = true;


}

static const char* currentMIDIsettingsText = "current midi settings";

void displayService::settingsQuick(void)
{
    screen->clear();

    screen->string(1, 0, currentMIDIsettingsText, 12, 1);

    screen->writeLine(0, 12, 127, 12, 1);

    screen->string(4, 19, LFOtext, 16, 1); // print LFO
    screen->string(30, 19, oneText, 16, 1); // print 1
    
    screen->string(48, 16, chText, 12, 1); // print ch
    
    static const char* ch =   "16";
    screen->string(62, 19, ch, 16, 1); // print ch num


    screen->string(87, 16, ccText, 12, 1); // print cc

    static const char* cc =   "127";
    screen->string(100, 19, cc, 16, 1); // print cc num


    //  LFO2
    screen->string(4, 44, LFOtext, 16, 1); // print LFO
    screen->string(30, 44, twoText, 16, 1); // print 1
    
    screen->string(48, 41, chText, 12, 1); // print ch
    
    //static const char* ch =   "16";
    screen->string(62, 44, ch, 16, 1); // print ch num


    screen->string(87, 41, ccText, 12, 1); // print cc

    //static const char* cc =   "127";
    screen->string(100, 44, cc, 16, 1); // print cc num

    
    needsRedraw = true;
}


void displayService::drawNoteOn(uint8_t whichLFO)
{
    static const char* noteOnText = " NOTE ON ";

    switch (whichLFO)
    {
        case LFO_1:
        {
            screen->string(38, 10, noteOnText, 12, 0);
            break;
        }
       
        case LFO_2:
        {
            screen->string(38, 42, noteOnText, 12, 0);
            break;
        }
    
        default:
            break;
    }
}




void displayService::drawNotifications(bool showNoteOn)
{
    if (showNoteOn == false) return;
    
    if (noteOn[LFO_1] == true) drawNoteOn(LFO_1);
    
    if (noteOn[LFO_2] == true) drawNoteOn(LFO_2);   

}




static const char* settingsText = "SETTINGS";
static const char* onText =  " ON  ";
static const char* offText = " OFF ";
static const char* longPressText = "LONG PRESS";

const char* settingsMenuNames[settings_menu_options_total] = 
{
    "Note On Flag",
    "Invert",
    "SAVE",
    "MIDI confg"
};

int16_t settingsMenuValues[settings_menu_options_total] = { 0 };

void displayService::updateSetting(uint8_t whichSetting, int16_t value)
{
    settingsMenuValues[whichSetting] = value;
}



void displayService::settingsScreen(uint8_t currentOption)
{
    

    screen->clear();

    screen->string(40, 0, settingsText, 12, 1);

    screen->string(4, 15, settingsMenuNames[settings_menu_option_noteOn], 12, 1);
    screen->string(4, 27, settingsMenuNames[settings_menu_option_invert], 12, 1);
    screen->string(4, 39, settingsMenuNames[settings_menu_option_save], 12, 1);
    screen->string(4, 51, settingsMenuNames[settings_menu_option_midi_config], 12, 1);




    const char* msg = "0";

    //  note on notification
    if      (settingsMenuValues[settings_menu_option_noteOn] == true)   msg = onText;
    else if (settingsMenuValues[settings_menu_option_noteOn] == false)  msg = offText;

    screen->string(96, 15, msg, 12, 1);

    //  display invert
    if      (settingsMenuValues[settings_menu_option_invert] == true)   msg = onText;
    else if (settingsMenuValues[settings_menu_option_invert] == false)  msg = offText;

    screen->string(96, 27, msg, 12, 1);


    // current option
    if (settingsMenuValues[currentOption] == true)
    {
        msg = onText;
    }
    else if (settingsMenuValues[currentOption] == false)
    {
        msg = offText;
    }

    screen->string(96, ((currentOption * 12) + 15), msg, 12, 0);



    if (currentOption == settings_menu_option_save)  screen->string(64, 39, longPressText, 12, 0);
    if (currentOption == settings_menu_option_midi_config)  screen->string(64, 51, longPressText, 12, 0);
    

    

    

    needsRedraw = true;

}

void displayService::checkForInvertedDisplay(void)  // return true if inverted
{
    if (displayIsInverted == settingsMenuValues[settings_menu_option_invert]) return;
    else
    {
        if (settingsMenuValues[settings_menu_option_invert] == true)
        {
            screen->invert();
            displayIsInverted = true;
        }
        if (settingsMenuValues[settings_menu_option_invert] == false)
        {
            screen->uninvert();
            displayIsInverted = false;
        }

    }
}




void displayService::setLFOwave(uint8_t whichLFO, byte waveForm)
{
    //Serial.print("set wave: ");
    //Serial.println(waveForm);

    LFOshape[whichLFO] = waveForm;

    needsRedraw = true;
}



void displayService::setLFOrate(uint8_t whichLFO, byte rate)
{
    //Serial.print("new rate: ");
    //Serial.println(rate);

    LFOrate[whichLFO] = rate;

    switch (LFOshape[whichLFO])
    {
        case sine:
        {
            //  rate comes in at a max of 255, higher is a slower LFO
            //  we need it to range between 0 and 14, higher is more cycles onscreen (so faster LFO)

            uint8_t invRate = 255 - rate;   // invert rate so higher is faster

            sinCycles = invRate / 18;   // 255 / 18 = 14.16666

            if (sinCycles > 14) sinCycles = 14;
            if (sinCycles < 1) sinCycles = 1;

            if (sinCycles == sinCyclesPrevious) return;
            sinCyclesPrevious = sinCycles;
            break;
        }
    
        default:
        break;
    }

    

    needsRedraw = true; 
}


void displayService::setLFOamp(uint8_t whichLFO, byte amp)
{
    //Serial.print("set amp: ");
    //Serial.println(amp);

    LFOamp[whichLFO] = amp;

    needsRedraw = true;   
}


void displayService::setLFOphase(uint8_t whichLFO, byte phase)
{
    //Serial.print("set phase: ");
    //Serial.println(phase);

    LFOphase[whichLFO] = phase;

    needsRedraw = true;
}

void displayService::setLFOdelay(uint8_t whichLFO, byte delay)
{
    //Serial.print("set phase: ");
    //Serial.println(phase);

    LFOdelay[whichLFO] = delay;

    needsRedraw = true;
}



void displayService::receiveMIDIchannel(uint8_t whichLFO, uint16_t val)
{
    MIDIdata[whichLFO].channel = val;
}

void displayService::receiveMIDIcc(uint8_t whichLFO, uint16_t val)
{
    MIDIdata[whichLFO].cc = val;
}

void displayService::receiveMIDIvalue(uint8_t whichLFO, uint16_t val)
{
    MIDIdata[whichLFO].value = val;
}

void displayService::receiveMIDIis14bit(uint8_t whichLFO, uint16_t val)
{
    MIDIdata[whichLFO].is14bit = true;
}

void displayService::receiveMIDIis7bit(uint8_t whichLFO, uint16_t val)
{
    MIDIdata[whichLFO].is14bit = false;
}

