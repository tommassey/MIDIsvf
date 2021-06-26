#include "MIDIserv.h"

enum Bytes
{
    LSB = 0,
    MSB,
    totalBytes
};


MIDIservice::MIDIservice()
{
  usbMIDI.begin();
  initScaling();
}


void MIDIservice::initParameter(output name, MIDIconfigProfile conf, uint16_t* p_value, bool* p_noteOn)
{
    config[name] = conf;
    externVal[name] = p_value;
    noteOn[name] = p_noteOn;
}


void MIDIservice::initScaling()  //  scale incoming MIDI CC value to useful 12bit value
{
  for (uint8_t i = 0; i < parameterTotal; i++)
  {
   float window = config[i].maxValue - config[i].minValue;
  config[i].scaledIncrement = ((float)twelvebit / window); 
  //Serial.print("init scaling ");
  //Serial.println(i);
  }
}


void MIDIservice::check(void)  //  polled externally to check for midi messages
{
  // if(!scaling_init)
  // {
  //   initScaling();
  //   scaling_init = true;
  // }

  MIDIevent newEvent = getMIDImsgFromBuffer();  // returns noParameter if no message

  if (newEvent.event == noEvent) return;  

  else
  {
    *externVal[newEvent.event] = newEvent.value; // if there's a valid message then update the relevant value
    return;
  }
}


MIDIevent MIDIservice::getMIDImsgFromBuffer()
{
  MIDIevent newEvent = {noEvent, 0};

  if (usbMIDI.read())  // check MIDI buffer
  {
    ///Serial.println("midi rcv");
    uint8_t msgType = usbMIDI.getType();
//=============================================================================================================   CC Message
    if (msgType == usbMIDI.ControlChange)  //  if it's a control change message
    {
      byte newChannel = usbMIDI.getChannel(); 

      if (newChannel == config[parameterFilter1].channel    //  if the channel is correct
                     || config[parameterFilter2].channel)
      {
        byte data1 = usbMIDI.getData1();                    //  ge the two data bytes
        byte data2 = usbMIDI.getData2();

        if ((data1 == config[parameterFilter1].CCforMSB) 
                   && (newChannel == config[parameterFilter1].channel))   //  ================================   FILTER 1 MSB
        {
          bytesForCC[parameterFilter1][MSB] = data2;

          if (config[parameterFilter1].resolution == sevenBit)
          {
            newEvent = {channel1CCevent, data2};  // because it's only 7bit, send MSB as value
            //printCC(newEvent.whichFilter, newEvent.value);
            return newEvent;
          }
        }

        if ((data1 == config[parameterFilter1].CCforLSB) 
                   && (newChannel == config[parameterFilter1].channel))   //  ================================   FILTER 1 LSB
        {
          bytesForCC[parameterFilter1][LSB] = data2;
          uint16_t sixteenBitNumber = bitShiftCombine(bytesForCC[parameterFilter1][MSB], bytesForCC[parameterFilter1][LSB]);
          
          //if (sixteenBitNumber != oldCutoff[Filter1])
          if (sixteenBitNumber != previousValue[parameterFilter1])
          {
            previousValue[parameterFilter1] = value[parameterFilter1];
            value[parameterFilter1] = sixteenBitNumber;

            newEvent = {channel1CCevent, value[parameterFilter1]};
            //printCC(newEvent.whichFilter, newEvent.value);
            return newEvent;
          }
        }

        if ((data1 == config[parameterFilter2].CCforMSB)
                   && (newChannel == config[parameterFilter2].channel))   //  ================================   FILTER 2 MSB
        {
          bytesForCC[parameterFilter2][MSB] = data2;

          if (config[parameterFilter2].resolution == sevenBit)
          {
            newEvent = {channel2CCevent, data2};
            //printCC(newEvent.whichFilter, newEvent.value);
            return newEvent;
          }
        }

        if ((data1 == config[parameterFilter2].CCforMSB) 
                   && (newChannel == config[parameterFilter2].channel))  // ==================================   FILTER 2 LSB
        {
          bytesForCC[parameterFilter2][LSB] = data2;
          uint16_t sixteenBitNumber = bitShiftCombine(bytesForCC[parameterFilter2][MSB], bytesForCC[parameterFilter2][LSB]);
        
          if (sixteenBitNumber != previousValue[parameterFilter2])
          {
            previousValue[parameterFilter2] = value[parameterFilter2];
            value[parameterFilter2] = sixteenBitNumber;

            newEvent = {channel2CCevent, value[parameterFilter2]};
            //printCC(newEvent.whichFilter, newEvent.value);
            return newEvent;
          }
        }
      }
    }
//=============================================================================================================   Note Message

    if (msgType == usbMIDI.NoteOn)
    {
      byte newChannel = usbMIDI.getChannel(); 

      Serial.println("Note on");

      if (newChannel == config[parameterFilter1].channel)
      {
        *noteOn[parameterFilter1] = true;
      }
      if (newChannel == config[parameterFilter2].channel)
      {
        *noteOn[parameterFilter2] = true;
      }
    }

    if (msgType == usbMIDI.NoteOff)
    {
      byte newChannel = usbMIDI.getChannel(); 
      
      Serial.println("Note off");

      if (newChannel == config[parameterFilter1].channel)
      {
        *noteOn[parameterFilter1] = false;
      }
      if (newChannel == config[parameterFilter2].channel)
      {
        *noteOn[parameterFilter2] = false;
      }
    }
  }
  return newEvent;
}



uint16_t MIDIservice::bitShiftCombine(uint8_t msb, uint8_t lsb)  // combine two bytes into a uint16
{
  uint16_t combined;
  combined = msb;              //send x_high to rightmost 8 bits
  combined = combined<<8;         //shift x_high over to leftmost 8 bits
  combined |= lsb;              //logical OR keeps x_high intact in combined and fills in rightmost 8 bits
  return combined;
}



void MIDIservice::printCC(int cc, int val)
{
  Serial.print("cc event    channel ");
  Serial.print(cc);
  Serial.print("    value ");
  Serial.println(val);
}


void MIDIservice::printConfigData()
{
  for(byte i = 0; i < parameterTotal; i++)
  {
    Serial.print("== Config ");
    Serial.print(i);
    Serial.print(" ===========");
    Serial.print("    CH: ");
    Serial.print(config[i].channel);
    Serial.print("   RES: ");
    Serial.print(config[i].resolution);
    Serial.print("   CC: ");
    Serial.print(config[i].CCforMSB);
    Serial.print("   MIN: ");
    Serial.print(config[i].minValue);
    Serial.print("   MAX: ");
    Serial.println(config[i].maxValue);
  }
}

