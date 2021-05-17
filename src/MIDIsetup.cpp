#include "MIDIsetup.h"

LED* _led;
Bounce* _btn;


byte currentMIDIconfigState = 0;
byte midiConfigValues[totalFiltValues] = {"\0"};
static MIDIconfigValue filter1;
static MIDIconfigValue filter2;
static MIDIconfigValue* currentConfigValue;


void initMIDIconfig(LED* led, Bounce* btn)
{
    _led = led;
    _btn = btn;
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
            currentConfigMode++;
            if (currentConfigMode >= CONFIG_MODE_total) currentConfigMode = CONFIG_MODE_filter1;
            Serial.print("configmode = ");
            Serial.println(currentConfigMode);
            break;
        }
        case longPress:
        {
            if (currentConfigMode == CONFIG_MODE_save)
            {
                currentConfigMode = CONFIG_MODE_reset;
                break;
            }
            else
            {
                currentConfigMode = CONFIG_MODE_save;
                Serial.println("CONFIG SAVED");
                break;
            }
        }

    }
     

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
        _led->setBlinkProfile(f1config);
        break;
    }
    case CONFIG_MODE_filter2:
    {
        currentConfigValue = &filter2;
        
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

          Serial.print("Got CC.  Channel = ");
          Serial.print(channel);
          Serial.print("  CC# = ");
          Serial.print(newCC);
          Serial.print("  value = ");
          Serial.println(newValue);

          newCCswitch(newCC, newValue, configToChange);
        }
      }
  }
}  // end of function 




bool newCCswitch(byte cc, byte val, MIDIconfigValue* filter)
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
    return 1;
  }
  //=========================================================================================== 14bit mode  

  else if (filter->initialised7bit && filter->initialised14bit &&(cc == filter->CCforMSB))   //  if we're in 14bit CCmsb, update value
  {
    Serial.println("we're in bit mode and we got the right CC#, update value");
    filter->incomingValueMSB = val;
    return 0; // wait til get LSB before returnning 1
  }

  else if (cc == (filter->CCforMSB + 32))  // check if we're reciving 14bit MIDI
  {
    Serial.println("CC says 14bit");

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

