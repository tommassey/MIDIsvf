#ifndef MIDISETUP_H
#define MIDISETUP_H

#include <Arduino.h>
//#include "MIDIUSB.h"
#include "LED.h"
#include "button.h"
#include "EEPROMservice.h"
#include "MIDIdefines.h"


#define Filter1CCmsb 31
#define Filter1CClsb 63

#define Filter2CCmsb 32
#define Filter2CClsb 64

#define twelvebit 4095




enum configModes
{
    CONFIG_MODE_start = 0,
    CONFIG_MODE_filter1,
    CONFIG_MODE_filter2,
    CONFIG_MODE_total,
    CONFIG_MODE_save = 10,
    CONFIG_MODE_countdown,
    CONFIG_MODE_reset
};



void printCC(int cc, int val);

uint16_t bitShiftCombine16(uint8_t x_high, uint8_t x_low);  // combines two bytes to make a uint16


void readMIDIforConfig(MIDIconfigProfile* configToChange);       // gets MIDI data into right places
bool newCCswitch(byte cc, byte val, byte newChannel, MIDIconfigProfile* filter);  // chooses correct CC values, does 14bit ness
void inputValueBounding(MIDIconfigProfile * configToChange);     // stores min & max input values 
byte CCfilter(byte cc);                                          //  filters out invalid CCs


void initMIDIconfig(bool* stayInConfigMode, LED* led, Bounce* btn); //
void MIDIconfigModeLoop();  // loop that runs when in config mode

void menuUpdate();
bool buttonService(Bounce* btn);


void initMIDIprofileInMenu(MIDIconfigProfile* value); // called when returning to already set filter to reset defaults

void restoreSettings(MIDIconfigProfile* f1, MIDIconfigProfile* f2);  // called to restore both filters' current MIDI config values from EEPROM
void saveSettings(MIDIconfigProfile* f1, MIDIconfigProfile* f2);  // called to save both filters' current MIDI config values to EEPROM

void useDefaultMIDIprofiles(MIDIconfigProfile* f1, MIDIconfigProfile* f2);  //  sets default values if no EEPROM data is detected

MIDIconfigProfile getFilterConfig(uint8_t whichFilter);  //  returns filter config data to main loop

void checkForSavedMIDIdata();





#endif
