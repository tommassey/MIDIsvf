#include "DACdriver.h"


void initMCP4xxx()
{
    SPI.begin();
    SPI.setMOSI(SPI_MOSI_PIN);
    SPI.setSCK(SPI_SCK_PIN);

    pinMode(MCP4922_LDAC_PIN, OUTPUT);
    pinMode(MCP4922_CS_PIN, OUTPUT);
    pinMode(MCP4922_SHDN_PIN, OUTPUT);
    DACwakeup();

    digitalWrite(MCP4922_LDAC_PIN, LOW);    //  LOW = write to register immediatley.   put HIGH , load data, then drop LOW to write to bopth channels simultaneously
    digitalWrite(MCP4922_CS_PIN, HIGH);     //  active LOW, so set HIGH to begin

    Serial.println("DAC init");
} 

void DACwriteChannelA(uint16_t data)
{
    if (data > twelvebit) data = twelvebit;
    if (data < 0) data = 0;

    SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));

    digitalWrite(MCP4922_CS_PIN, LOW);
    SPI.transfer16(mcp4922ChannelA | data);
    digitalWrite(MCP4922_CS_PIN, HIGH);

    SPI.endTransaction();
}

void DACwriteChannelB(uint16_t data)
{
    if (data > twelvebit) data = twelvebit;
    if (data < 0) data = 0;
    
    SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));

    digitalWrite(MCP4922_CS_PIN, LOW);
    SPI.transfer16(mcp4922ChannelB | data);
    digitalWrite(MCP4922_CS_PIN, HIGH);

    SPI.endTransaction();
}

void DACwriteBothChannels(uint16_t dataA, uint16_t dataB)
{
    SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));

    digitalWrite(MCP4922_LDAC_PIN, HIGH);    //  HIGH = hold incoming data

    digitalWrite(MCP4922_CS_PIN, LOW);
    SPI.transfer16(mcp4922ChannelA | dataA);
    digitalWrite(MCP4922_CS_PIN, HIGH);

    digitalWrite(MCP4922_CS_PIN, LOW);
    SPI.transfer16(mcp4922ChannelB | dataB);
    digitalWrite(MCP4922_CS_PIN, HIGH);

    digitalWrite(MCP4922_LDAC_PIN, LOW);    //  LOW = write to registers 

    SPI.endTransaction();
}

void DACrawSpeedTest(void)
{
    while (1)   // raw speed test loop, generates 'square' wave as fast as possible 
  {
    DACwriteChannelA(0);
    DACwriteChannelA(4095); 
  }
}

void DACtriangleWave(void)  // generates 4096 step triangle wave
{
  for (int i = 0; i < 4095; i++)
  {
    DACwriteChannelA(i);
  }

  for (int i = 4095; i > 0; i--)
  {
    DACwriteChannelA(i);
  }
}

void DACshutdown(void)
{
    digitalWrite(MCP4922_SHDN_PIN, LOW);
}


void DACwakeup(void)
{
    digitalWrite(MCP4922_SHDN_PIN, HIGH);
}



