#ifndef COMMAND_LIST_H
#define COMMAND_LIST_H

#include <Arduino.h>

enum MIDIchannels
{
    MIDIchannel_1 = 0,
    MIDIchannel_2,
    MIDIchannel_total
};

enum commandTypes
{
    command_none = 0,

    command_enter_setup_mode,
    command_changeScreen,
    
    //====================================  LFO1
    
    command_LFO1_setWaveform,
    command_LFO1_setRate,
    command_LFO1_setAmp,
    command_LFO1_setPhase,
    command_LFO1_setDelay,
    command_LFO1_noteOn,
    command_LFO1_noteOff,
    
    command_LFO1_recieve_midi_channel,
    command_LFO1_recieve_midi_cc,
    command_LFO1_recieve_midi_value,
    command_LFO1_recieve_midi_is_14bit,
    command_LFO1_recieve_midi_is_7bit,

    //====================================  LFO2
    command_LFO2_setWaveform,
    command_LFO2_setRate,
    command_LFO2_setAmp,
    command_LFO2_setPhase,
    command_LFO2_setDelay,
    command_LFO2_noteOn,
    command_LFO2_noteOff,
    
    command_LFO2_recieve_midi_channel,
    command_LFO2_recieve_midi_cc,
    command_LFO2_recieve_midi_value,
    command_LFO2_recieve_midi_is_14bit,
    command_LFO2_recieve_midi_is_7bit,


    command_total
};



enum waveForms
{
    noWaveform = 0,
    sine,
    triangle,
    saw,
    //rampUp,
    //rampDown,
    squaree,
    nonMusicLFO,
    totalWaveforms
};

enum LFOS
{
    LFO_1,
    LFO_2,
    LFO_total,
    LFO_none
};


#endif  //  COMMAND_LIST_H