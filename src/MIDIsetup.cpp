#include "MIDIsetup.h"

LED* _led;
Bounce* _btn;

MIDIconfigProfile filter1;
MIDIconfigProfile filter2;

byte currentMIDIconfigState = 0;
byte midiConfigValues[totalFiltValues] = {"\0"};
//MIDIconfigProfile* _filter1;
//MIDIconfigProfile* _filter2;
MIDIconfigProfile* currentConfigProfile = &filter1;

int readyForBounding = 0;     //  counts up to boundNow, then starts bounding recieved CC values.   To stop chatter at startup
const int boundNow = 5;

int readyToReadMIDI = 0;
const int readyNow = 7;


void initMIDIconfig(LED* led, Bounce* btn) //, MIDIconfigProfile* f1, MIDIconfigProfile* f2)
{
    _led = led;
    _btn = btn;
    //_filter1 = f1;
    //_filter2 = f2;
}


void MIDIconfigMode()
{
  updateLED(_led);

  if (buttonService(_btn) == 1)
  {
    menuUpdate();
  }


  readMIDIforConfig(currentConfigProfile);
}



byte currentConfigMode = CONFIG_MODE_start; 


bool buttonService(Bounce* btn)  // returns 1 if we need to refresh menu
{
    
    uint8_t pressed = checkButton(_btn);

    if (pressed) Serial.println("button pressed");

    switch (pressed)
    {
        case noPress:
        {
            return 0;
        }
        case shortPress:
        {   
          if (currentConfigMode == CONFIG_MODE_start)
            {
              Serial.println("short press");

              currentConfigMode = CONFIG_MODE_filter1;
            }
            if (currentConfigMode == CONFIG_MODE_save) break;
            
            
            currentConfigMode++;
            _led->currentBlink = 0;
            readyForBounding = 0;
            if (currentConfigMode >= CONFIG_MODE_total) currentConfigMode = CONFIG_MODE_filter1;
            Serial.print("configmode = ");
            Serial.println(currentConfigMode);
            return 1;
        }
        case longPress:
        {
            if (currentConfigMode == CONFIG_MODE_start)
            {
              Serial.println("long press");
              return 0;
            }

            if (currentConfigMode == CONFIG_MODE_filter1 || CONFIG_MODE_filter2)
            {
                _led->currentBlink = 0;
                readyForBounding = 0;
                currentConfigMode = CONFIG_MODE_save;
                Serial.println("CONFIG SAVED");
                return 1;
            }

            if (currentConfigMode == CONFIG_MODE_save)
            {
                _led->currentBlink = 0;
                readyForBounding = 0;
                currentConfigMode = CONFIG_MODE_reset;
                return 1;
            }
        }
    }
  return 0;
}


void menuUpdate()
{
  Serial.println("menu update");
  
  switch (currentConfigMode)
  {
    case CONFIG_MODE_start:
    {
        _led->setBlinkProfile(init);
        break;
    }
    case CONFIG_MODE_filter1:
    {
        currentConfigProfile = &filter1;
        initMIDIprofileInMenu(&filter1);
        _led->setBlinkProfile(f1config);
        break;
    }
    case CONFIG_MODE_filter2:
    {
        currentConfigProfile = &filter2;
        initMIDIprofileInMenu(&filter2);
        _led->setBlinkProfile(f2config);
        break;
    }
    case CONFIG_MODE_save:
    {
      saveSettings(&filter1, &filter2);
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
        initMIDIprofileInMenu(&filter1);
        initMIDIprofileInMenu(&filter2);
        _led->setBlinkProfile(reset);
        currentConfigMode = CONFIG_MODE_filter1;
        delay(200);
        break;
    }
    
    default:
      break;
  }
}


void readMIDIforConfig(MIDIconfigProfile* configToChange)
{
  if (usbMIDI.read())
  {
    readyToReadMIDI++;

    if (readyToReadMIDI > readyNow)
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

          if (newCCswitch(newCC, newValue, configToChange))
          {
            inputValueBounding(configToChange);
          }
        }
      }
    }
  }
}  // end of function 




bool newCCswitch(byte cc, byte val, MIDIconfigProfile* filter)  // return true if value needs changing
{

  Serial.println("prefilter");
  cc = CCfilter(cc);   // filter out invalid CC numbers
  Serial.println("postfilter");
 
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
  // //=========================================================================================== 14bit mode  

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


void inputValueBounding(MIDIconfigProfile* configToChange)
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


void initMIDIprofileInMenu(MIDIconfigProfile* value)
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

void saveSettings(MIDIconfigProfile* f1, MIDIconfigProfile* f2)  // called to save both filters' current MIDI config values to EEPROM
{
  //===========================================  filter 1 save
  EEPROM.write(channel1, filter1.channel);
  EEPROM.write(resolution1, filter1.resolution);

  if (filter1.resolution == sevenBit)
  {
    EEPROM.write(CCmsb1, filter1.CCforMSB);

    EEPROM.write(valMin1, filter1.minValue);
    EEPROM.write(valMax1, filter1.maxValue);
  }

  else if (filter1.resolution == fourteenBit)
  {
    EEPROM.write(CCmsb1, filter1.CCforMSB);
    EEPROM.write(CClsb1, filter1.CCforLSB);

    EEPROMwriteuint16(valMin1, filter1.minValue);
    EEPROMwriteuint16(valMax1, filter1.maxValue);
  }

  Serial.println("Filter1 MIDI config saved");
  


  //===========================================  filter 2 save
  EEPROM.write(channel2, filter2.channel);
  EEPROM.write(resolution2, filter2.resolution);

  if (filter2.resolution == sevenBit)
  {
    EEPROM.write(CCmsb2, filter2.CCforMSB);

    EEPROM.write(valMin2, filter2.minValue);
    EEPROM.write(valMax2, filter2.maxValue);
  }

  else if (filter2.resolution == fourteenBit)
  {
    EEPROM.write(CCmsb2, filter2.CCforMSB);
    EEPROM.write(CClsb2, filter2.CCforLSB);

    EEPROMwriteuint16(valMin2, filter2.minValue);
    EEPROMwriteuint16(valMax2, filter2.maxValue);
  }

  Serial.println("Filter2 MIDI config saved");

  EEPROM.write(savedDataExists, 1);

  Serial.println("MIDI config data save complete");
  delay(1000);
  Serial.println("Restart in ...");
  delay(1000);
  Serial.println("3 ...");
  delay(1000);
  Serial.println("2 ...");
  delay(1000);
  Serial.println("1 ...");
  delay(500);

  SCB_AIRCR = 0x05FA0004;  // hardware reset

}

void restoreSettings(MIDIconfigProfile* f1, MIDIconfigProfile* f2)  // called to restore both filters' current MIDI config values from EEPROM
{
  //===========================================  filter 1 restore
  f1->channel = EEPROM.read(channel1);
  f1->resolution = EEPROM.read(resolution1);

  if (f1->resolution == sevenBit)
  {
    f1->CCforMSB= EEPROM.read(CCmsb1);

    f1->minValue = EEPROM.read(valMin1);
    f1->maxValue = EEPROM.read(valMax1);
  }

  else if (f1->resolution == fourteenBit)
  {
    f1->CCforMSB = EEPROM.read(CCmsb1);
    f1->CCforLSB = EEPROM.read(CClsb1);

    f1->minValue = EEPROMreaduint16(valMin1);
    f1->maxValue = EEPROMreaduint16(valMax1);
  }

  Serial.println("Filter1 MIDI config restored");
  


  //===========================================  filter 2 save
  f2->channel = EEPROM.read(channel2);
  f2->resolution = EEPROM.read(resolution2);

  if (f2->resolution == sevenBit)
  {
     f2->CCforMSB = EEPROM.read(CCmsb2);
     f2->minValue = EEPROM.read(valMin2);
     f2->maxValue = EEPROM.read(valMax2);
  }

  else if (f2->resolution == fourteenBit)
  {
    f2->CCforMSB = EEPROM.read(CCmsb2);
    f2->CCforLSB = EEPROM.read(CClsb2);

    f2->minValue = EEPROMreaduint16(valMin2);
    f2->maxValue = EEPROMreaduint16(valMax2);
  }

  Serial.println("Filter2 MIDI config restored");
  Serial.println("MIDI config data restore complete");
}

void useDefaultMIDIprofiles(MIDIconfigProfile* f1, MIDIconfigProfile* f2)
{
    f1->channel = 1;                   //  MIDI channel
    f1->CCforMSB = 10;                 //  CC# 7bit
    f1->CCforLSB = 0;                  // 2nd CC# for 14bit MIDI
    f1->resolution = sevenBit;
    f1->incomingValueMSB = 0;  // incoming MIDI CC value
    f1->incomingValueLSB = 0;  // incoming MIDI CC value
    f1->currentValue = 0;
    f1->minValue = 0;
    f1->maxValue = 127;
    f1->initialised7bit = false;
    f1->initialised14bit = false;

    f2->channel = 1;                   //  MIDI channel
    f2->CCforMSB = 11;                 //  CC# 7bit
    f2->CCforLSB = 0;                  // 2nd CC# for 14bit MIDI
    f2->resolution = sevenBit;
    f2->incomingValueMSB = 0;  // incoming MIDI CC value
    f2->incomingValueLSB = 0;  // incoming MIDI CC value
    f2->currentValue = 0;
    f2->minValue = 0;
    f2->maxValue = 127;
    f2->initialised7bit = false;
    f2->initialised14bit = false;
}



bool checkForSavedMIDIdata()
{
  if (EEPROM.read(savedDataExists))
  {
    restoreSettings(&filter1, &filter2);
    Serial.println("Saved MIDI config restored from EEPROM");
  }
  else
  {
    useDefaultMIDIprofiles(&filter1, &filter2);
    Serial.println("No saved MIDI config found, using default values");
  }
}

// void printMIDIprofiles()
// {
//   Serial.println("== Filter 1 ===========");
//   Serial.print("CH: ");
//   Serial.print(filter1.channel);
//   Serial.print("   RES: ");
//   Serial.print(filter1.resolution);
//   Serial.print("   CC: ");
//   Serial.print(filter1.CCforMSB);
//   Serial.print("   MIN: ");
//   Serial.print(filter1.minValue);
//   Serial.print("   MAX: ");
//   Serial.println(filter1.maxValue);

//   Serial.println("== Filter 2 ===========");
//   Serial.print("CH: ");
//   Serial.print(filter2.channel);
//   Serial.print("   RES: ");
//   Serial.print(filter2.resolution);
//   Serial.print("   CC: ");
//   Serial.print(filter2.CCforMSB);
//   Serial.print("   MIN: ");
//   Serial.print(filter2.minValue);
//   Serial.print("   MAX: ");
//   Serial.println(filter2.maxValue);
// }

MIDIconfigProfile getFilterConfig(uint8_t whichFilter)
{
  switch (whichFilter)
  {
  case Filter1:
    {
      return filter1;
    }
  
  case Filter2:
    {
      return filter2;
    }
  
  
  default:
    break;
  }
}