#ifndef DAC_DRIVER_H
#define DAC_DRIVER_H


#include "Arduino.h"
#include "SPI.h"
#include "pinDefines.h"

#define SPI_MOSI_PIN        11
#define SPI_SCK_PIN         14

#define MCP4822_LDAC_PIN    1
#define MCP4822_CS_PIN      10
#define MCP4922_LDAC_PIN    9   //  LOW = write to registers immediatley.   put HIGH , load data, then drop LOW to write to both channels simultaneously
#define MCP4922_CS_PIN      10
#define MCP4922_SHDN_PIN    8   //  DAC shutdown, active low, tie high for normal operation


//  12bits  = 2^12-1 = 4095 steps


typedef uint16_t mcp4xxx_register;  // 16bit register for a single channel

static const mcp4xxx_register mcp4xxxChannelA  = 0B0000000000000000;   //  for all MCP4XXX
static const mcp4xxx_register mcp4xxxChannelB  = 0B1000000000000000;   //  for MCP4XX2 only
static const mcp4xxx_register mcp4xxxBufferOff = 0B0000000000000000;   //  for all MCP4XXX
static const mcp4xxx_register mcp4xxxBufferOn  = 0B0100000000000000;   //  for MCP4XX2 only
static const mcp4xxx_register mcp4xxxGainOne   = 0B0010000000000000;   //
static const mcp4xxx_register mcp4xxxGainTwo   = 0B0000000000000000;   //
static const mcp4xxx_register mcp4xxxOutputOn  = 0B0001000000000000;   //
static const mcp4xxx_register mcp4xxxOutputOff = 0B0000000000000000;   //



//  consts for the control bit to the left, adjust to taste

static const mcp4xxx_register mcp4822ChannelA = mcp4xxxChannelA | mcp4xxxBufferOff | mcp4xxxGainOne | mcp4xxxOutputOn;
static const mcp4xxx_register mcp4822ChannelB = mcp4xxxChannelB | mcp4xxxBufferOff | mcp4xxxGainOne | mcp4xxxOutputOn;
static const mcp4xxx_register mcp4922ChannelA = mcp4xxxChannelA | mcp4xxxBufferOff | mcp4xxxGainOne | mcp4xxxOutputOn;
static const mcp4xxx_register mcp4922ChannelB = mcp4xxxChannelB | mcp4xxxBufferOff | mcp4xxxGainOne | mcp4xxxOutputOn;


void initMCP4xxx();

void DACwriteChannelA(uint16_t data);
void DACwriteChannelB(uint16_t data);
void DACwriteBothChannels(uint16_t dataA, uint16_t dataB);

void DACrawSpeedTest(void);       // raw speed test loop, generates 'square' wave as fast as possible 
void DACtriangleWave(void);       // generates 4096 step triangle wave

void DACshutdown(void);
void DACwakeup(void);             // if using DACs SHDN pin, then this needs to be called during init










#endif