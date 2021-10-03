#include "receiverService.h"

displayService* displayPtr;


void func(uint16_t val){}
void executeCommandList(serialMessage msg);


// =============================  command functions
void cmd_None(uint16_t val);
void cmd_enter_setup_mode(uint16_t val);
void cmd_changeScreen(uint16_t val);

void cmd_LFO1_setWaveform(uint16_t val);
void cmd_LFO1_setRate(uint16_t val);
void cmd_LFO1_setAmp(uint16_t val);
void cmd_LFO1_setPhase(uint16_t val);
void cmd_LFO1_setDelay(uint16_t val);
void cmd_LFO1_noteOn(uint16_t val);
void cmd_LFO1_noteOff(uint16_t val);
void cmd_LFO1_recieve_midi_channel(uint16_t val);
void cmd_LFO1_recieve_midi_cc(uint16_t val);
void cmd_LFO1_recieve_midi_value(uint16_t val);
void cmd_LFO1_recieve_midi_is_14bit(uint16_t val);
void cmd_LFO1_recieve_midi_is_7bit(uint16_t val);

void cmd_LFO2_setWaveform(uint16_t val);
void cmd_LFO2_setRate(uint16_t val);
void cmd_LFO2_setAmp(uint16_t val);
void cmd_LFO2_setPhase(uint16_t val);
void cmd_LFO2_setDelay(uint16_t val);
void cmd_LFO2_noteOn(uint16_t val);
void cmd_LFO2_noteOff(uint16_t val);
void cmd_LFO2_recieve_midi_channel(uint16_t val);
void cmd_LFO2_recieve_midi_cc(uint16_t val);
void cmd_LFO2_recieve_midi_value(uint16_t val);
void cmd_LFO2_recieve_midi_is_14bit(uint16_t val);
void cmd_LFO2_recieve_midi_is_7bit(uint16_t val);





//   Array of function pointers.  stores functions called for all rx commmands
void (*(commandFunction[command_total]))(uint16_t) = 
{ 

  //      THIS MUST MATCH THE commandTypes ENUM IN commandList.h!!!

    cmd_None,
    cmd_enter_setup_mode,
    cmd_changeScreen,

    cmd_LFO1_setWaveform,
    cmd_LFO1_setRate,
    cmd_LFO1_setAmp,
    cmd_LFO1_setPhase,
    cmd_LFO1_setDelay,
    cmd_LFO1_noteOn,
    cmd_LFO1_noteOff,
    cmd_LFO1_recieve_midi_channel,
    cmd_LFO1_recieve_midi_cc,
    cmd_LFO1_recieve_midi_value,
    cmd_LFO1_recieve_midi_is_14bit,
    cmd_LFO1_recieve_midi_is_7bit,

    cmd_LFO2_setWaveform,
    cmd_LFO2_setRate,
    cmd_LFO2_setAmp,
    cmd_LFO2_setPhase,
    cmd_LFO2_setDelay,
    cmd_LFO2_noteOn,
    cmd_LFO2_noteOff,
    cmd_LFO2_recieve_midi_channel,
    cmd_LFO2_recieve_midi_cc,
    cmd_LFO2_recieve_midi_value,
    cmd_LFO2_recieve_midi_is_14bit,
    cmd_LFO2_recieve_midi_is_7bit,

};



void receiverServiceInit(displayService* disp)
{
    receiverInit();

    displayPtr = disp;

       
}

void receiverServiceTask(void)
{
    getSerialMessage();
    
    if (getCommandCount() > 0)
    {
        executeCommandList(passCommand());
    }
    

}


void executeCommandList(serialMessage newMessage)
{

  byte msg = newMessage.messageType;
  byte data1 = newMessage.data[0];
  byte data2 = newMessage.data[1];

   uint16_t val = ((uint16_t)data2 << 8) | data1;   // combine two bytes to make uint16

  commandFunction[msg](val);

}


void cmd_None(uint16_t val)
{

}
//------------------------------------------
void cmd_enter_setup_mode(uint16_t val)
{

}
//------------------------------------------
void cmd_changeScreen(uint16_t val)
{
    //displayPtr->showFullScreen(val);
}
//---------------------------------------------------------  LFO 1
void cmd_LFO1_setWaveform(uint16_t val)
{
    //displayPtr->setLFOwave(LFO_1, val);    
}
//------------------------------------------
void cmd_LFO1_setRate(uint16_t val)
{
    //displayPtr->setLFOrate(LFO_1, val);
}
//------------------------------------------
void cmd_LFO1_setAmp(uint16_t val)
{
    //displayPtr->setLFOamp(LFO_1, val);
}
//------------------------------------------
void cmd_LFO1_setPhase(uint16_t val)
{
    //displayPtr->setLFOphase(LFO_1, val);
}
//------------------------------------------
void cmd_LFO1_setDelay(uint16_t val)
{
    //displayPtr->setLFOdelay(LFO_1, val);
}
//------------------------------------------
void cmd_LFO1_noteOn(uint16_t val)
{
    //displayPtr->noteOnEvent(LFO_1);
}
//------------------------------------------
void cmd_LFO1_noteOff(uint16_t val)
{
    //displayPtr->noteOffEvent(LFO_1);
}
//------------------------------------------
void cmd_LFO1_recieve_midi_channel(uint16_t val)
{
    //displayPtr->receiveMIDIchannel(LFO_1, val);
}
//------------------------------------------
void cmd_LFO1_recieve_midi_cc(uint16_t val)
{
    //displayPtr->receiveMIDIcc(LFO_1, val);
}
//------------------------------------------
void cmd_LFO1_recieve_midi_value(uint16_t val)
{
    //displayPtr->receiveMIDIvalue(LFO_1, val);
}
//------------------------------------------

void cmd_LFO1_recieve_midi_is_14bit(uint16_t val)
{
    //displayPtr->receiveMIDIis14bit(LFO_1, val);
}
//------------------------------------------
void cmd_LFO1_recieve_midi_is_7bit(uint16_t val)
{
    //displayPtr->receiveMIDIis7bit(LFO_1, val);
}
//------------------------------------------


//--------------------------------------------------------- LFO 2

void cmd_LFO2_setWaveform(uint16_t val)
{
    //displayPtr->setLFOwave(LFO_2, val);
    
}
//------------------------------------------
void cmd_LFO2_setRate(uint16_t val)
{
    //displayPtr->setLFOrate(LFO_2, val);
}
//------------------------------------------
void cmd_LFO2_setAmp(uint16_t val)
{
    //displayPtr->setLFOamp(LFO_2, val);
}
//------------------------------------------
void cmd_LFO2_setPhase(uint16_t val)
{
    //displayPtr->setLFOphase(LFO_2, val);
}
//------------------------------------------
void cmd_LFO2_setDelay(uint16_t val)
{
    //displayPtr->setLFOdelay(LFO_2, val);
}
//------------------------------------------
void cmd_LFO2_noteOn(uint16_t val)
{
    //displayPtr->noteOnEvent(LFO_2);
}
//------------------------------------------
void cmd_LFO2_noteOff(uint16_t val)
{
   // displayPtr->noteOffEvent(LFO_2);
}
//------------------------------------------
void cmd_LFO2_recieve_midi_channel(uint16_t val)
{
    //displayPtr->receiveMIDIchannel(LFO_2, val);
}
//------------------------------------------
void cmd_LFO2_recieve_midi_cc(uint16_t val)
{
    //displayPtr->receiveMIDIcc(LFO_2, val);
}
//------------------------------------------
void cmd_LFO2_recieve_midi_value(uint16_t val)
{
    //displayPtr->receiveMIDIvalue(LFO_2, val);
}
//------------------------------------------

void cmd_LFO2_recieve_midi_is_14bit(uint16_t val)
{
    //displayPtr->receiveMIDIis14bit(LFO_2, val);
}
//------------------------------------------
void cmd_LFO2_recieve_midi_is_7bit(uint16_t val)
{
    //displayPtr->receiveMIDIis7bit(LFO_2, val);

    //MIDIprofile bob = { 0, 0, 0, 0, 0 };
    //displayPtr->showFullScreen(1);
}
//------------------------------------------





