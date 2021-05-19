#include "MIDIsetup.h"

LED* _led;
Bounce* _btn;

byte currentMIDIconfigState = 0;
byte midiConfigValues[totalFiltValues] = {"\0"};
static MIDIconfigValue filter1;
static MIDIconfigValue filter2;
static MIDIconfigValue* currentConfigValue;

int readyForBounding = 0;     //  counts up to boundNow, then starts bounding recieved CC values.   To stop chatter at startup
const int boundNow = 5;


void initMIDIconfig(LED* led, Bounce* btn)
{
    _led = led;
    _btn = btn;
}


void MIDIconfigMode()
{
  updateLED(_led);
  buttonService(_btn);

  switch (currentConfigMode)
  {
    case CONFIG_MODE_start:
    {
        _led->setBlinkProfile(init);
        break;
    }
    case CONFIG_MODE_filter1:
    {
        currentConfigValue = &filter1;
        //resetMIDIconfigValueToDefaults(&filter1);
        _led->setBlinkProfile(f1config);
        break;
    }
    case CONFIG_MODE_filter2:
    {
        currentConfigValue = &filter2;
        //resetMIDIconfigValueToDefaults(&filter2);
        
        _led->setBlinkProfile(f2config);
        break;
    }
    case CONFIG_MODE_save:
    {
      _led->setBlinkProfile(saved);
      break;
    }
    case CONFIG_MODE_reset:
    {
        Serial.println("init filter values");
        filter1.initialised7bit  = false;
        filter1.initialised14bit = false;
        filter2.initialised7bit  = false;
        filter2.initialised14bit = false;
        resetMIDIconfigValueToDefaults(&filter1);
        resetMIDIconfigValueToDefaults(&filter2);
        _led->setBlinkProfile(reset);
        currentConfigMode = CONFIG_MODE_filter1;
        delay(200);
        break;
    }
    
    default:
      break;
  }
  
  readMIDIforConfig(currentConfigValue);
}





void buttonService(Bounce* btn)
{
    switch (checkButton(_btn))
    {
        case noPress:
        {
            break;
        }
        case shortPress:
        {   
            if (currentConfigMode == CONFIG_MODE_save) break;
            
            
            currentConfigMode++;
            _led->currentBlink = 0;
            readyForBounding = 0;
            if (currentConfigMode >= CONFIG_MODE_total) currentConfigMode = CONFIG_MODE_filter1;
            Serial.print("configmode = ");
            Serial.println(currentConfigMode);
            break;
        }
        case longPress:
        {
            if (currentConfigMode == CONFIG_MODE_save)
            {
                _led->currentBlink = 0;
                readyForBounding = 0;
                currentConfigMode = CONFIG_MODE_reset;
                break;
            }
            else
            {
                _led->currentBlink = 0;
                readyForBounding = 0;
                currentConfigMode = CONFIG_MODE_save;
                Serial.println("CONFIG SAVED");
                break;
            }
        }
    }
}


void readMIDIforConfig(MIDIconfigValue* configToChange)
{
  if (usbMIDI.read())
  {
    switch (usbMIDI.getType())
    {
      case usbMIDI.ControlChange:
      {
        byte newCC    = usbMIDI.getData1();
        byte newValue = usbMIDI.getData2();
        byte channel  = usbMIDI.getChannel();

        // Serial.print("Got CC.  Channel = ");
        // Serial.print(channel);
        // Serial.print("  CC# = ");
        // Serial.print(newCC);
        // Serial.print("  value = ");
        // Serial.println(newValue);

        if (newCCswitch(newCC, newValue, configToChange))
        {
          inputValueBounding(configToChange);
        }
      }
    }
  }
}  // end of function 




bool newCCswitch(byte cc, byte val, MIDIconfigValue* filter)  // return true if value needs changing
{
  cc = CCfilter(cc);   // filter out invalid CC numbers
 
  //=========================================================================================== 7bit mode  
  if (!filter->initialised7bit)  // if it's the first time, set incoming CC# to Filter1CCMSB
  {
    filter->CCforMSB = cc;
    filter->incomingValueMSB = val;
    filter->initialised7bit = true;
    filter->resolution = sevenBit;

    Serial.print("init filter1 7bit MSB CC = ");
    Serial.print(filter->CCforMSB);
    Serial.print("  value = ");
    Serial.println(filter->incomingValueMSB);

    return 1;
  }
  else if (filter->initialised7bit && !filter->initialised14bit && (cc == filter->CCforMSB))   //  if we're in 7bit mode and we got the right CC#, update value
  {
    Serial.println("we're in 7bit mode and we got the right CC#, update value");
    filter->incomingValueMSB = val;
    filter->currentValue = val;
    readyForBounding++;
    return 1;
  }
  //=========================================================================================== 14bit mode  

  else if (filter->initialised7bit && filter->initialised14bit &&(cc == filter->CCforMSB))   //  if we're in 14bit CCmsb, update value
  {
    Serial.println("we're in 14bit mode and we got the right CC#, update value");
    filter->incomingValueMSB = val;
    return 0; // wait til get LSB before returnning 1
  }

  else if (cc == (filter->CCforMSB + 32))  // check if we're reciving 14bit MIDI
  {
    Serial.println("CC says 14bit");
    readyForBounding++;

    if (!filter->initialised14bit)   //  if it's first 14bit msg
    {
      filter->CCforLSB = cc;
      filter->incomingValueLSB = val;
      filter->initialised14bit = true;
      filter->resolution = fourteenBit;
      filter->currentValue = bitShiftCombine16(filter->incomingValueMSB, filter->incomingValueLSB);

      Serial.print("init filter 1 14bit input LSB CC = ");
      Serial.print(filter->incomingValueLSB);
      Serial.print("  MSB CC = ");
      Serial.print(filter->CCforMSB);
      Serial.print("  value = ");
      Serial.println(filter->currentValue);
      
      return 1;
    }
  
    else if (filter->resolution == fourteenBit) // if we're init'd for 14bit and the CC# is the right one for the 14bit's LSB
    {
      Serial.print("14bit MIDI event  ");
      filter->incomingValueLSB = val;
      filter->currentValue = bitShiftCombine16(filter->incomingValueMSB, filter->incomingValueLSB);
      Serial.print("  CC = ");
      Serial.print(filter->CCforMSB);
      Serial.print("  value = ");
      Serial.println(filter->currentValue);

      return 1;
    }   
  
  }  // end 14b

  return 0;
}




uint16_t bitShiftCombine16( byte x_high, byte x_low)
{
  uint16_t highpart = ((uint16_t)x_high << 8);
  uint16_t combined = highpart | x_low;
  return combined;
}


void inputValueBounding(MIDIconfigValue* configToChange)
{
  if (readyForBounding > boundNow)
  {
    uint16_t newValue = configToChange->currentValue;
  
    if (newValue > configToChange->maxValue) configToChange->maxValue = newValue;
    if (newValue < configToChange->minValue) configToChange->minValue = newValue;
  
    Serial.print("value bounds ---  high = ");
    Serial.print(configToChange->maxValue);
    Serial.print("  low = ");
    Serial.println(configToChange->minValue);
  }
}


byte CCfilter(byte cc)   // filters out excluded CC numbers, returns CC if valid, 0 if not.
{
  if (cc > 119)
  {
    Serial.println("excluded CC 120+");
    return 0;
  }

  switch (cc)
  {
    case 0:
    {
      Serial.println("excluded CC - Bank select");
      return 0;
    }
    case 6:
    {
      Serial.println("excluded CC - Data Entry");
      return 0;
    }
    case 7:
    {
      Serial.println("excluded CC - Volume");
      return 0;
    }
    case 10:
    {
      Serial.println("excluded CC - Pan position");
      return 0;
    }
    case 98:
    {
      Serial.println("excluded CC - Non-Registered param LSB");
      return 0;
    }
    case 99:
    {
      Serial.println("excluded CC - Non-Registered param MSB");
      return 0;
    }
    case 100:
    {
      Serial.println("excluded CC - Registered param LSB");
      return 0;
    }
    case 101:
    {
      Serial.println("excluded CC - Registered param MSB");
      return 0;
    }
  }
  return cc;
}


void resetMIDIconfigValueToDefaults(MIDIconfigValue* value)
{
  value->channel = 1;               //  MIDI channel
  value->CCforMSB = 0;                 //  CC# 7bit
  value->CCforLSB = 0;                 // 2nd CC# for 14bit MIDI
  value->resolution = sevenBit;
  value->incomingValueMSB = 0;  // incoming MIDI CC value
  value->incomingValueLSB = 0;  // incoming MIDI CC value
  value->currentValue = 0;
  value->minValue = 33000;       // lowest value so far
  value->maxValue = 1;     // highest value so far
  value->initialised7bit = false;
  value->initialised14bit = false;

}