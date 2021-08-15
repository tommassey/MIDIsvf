#include <SPI.h>
#include "spiDriver.h"


SSD1306_SPI_Driver::SSD1306_SPI_Driver()
{
    begin();
    SPI.setMOSI(11);
    SPI.setSCK(13);
    //SPI.setCS(10);
    //SPI.beginTransaction;
}


SSD1306_SPI_Driver::~SSD1306_SPI_Driver(){}

// Public functions

void SSD1306_SPI_Driver::setDChigh(void)
{
    digitalWrite(OLED_DC, HIGH);
}

void SSD1306_SPI_Driver::sendCommand(uint8_t command)
{
    digitalWrite(OLED_DC, LOW);
    sendBytes(&command, 1);
}

void SSD1306_SPI_Driver::sendBytes(uint8_t *buffer, int bufferLength)
{
    for (uint8_t i = 0; i < bufferLength; i++)
    {
        SPI.transfer(buffer[i]);
    }
}

void SSD1306_SPI_Driver::begin(void)
{
    pinMode(OLED_RST, OUTPUT);
    pinMode(OLED_DC, OUTPUT);
    pinMode(OLED_CS, OUTPUT);

    SPI.begin();
    
    SPI.setClockDivider(SPI_CLOCK_DIV128);
    
    digitalWrite(OLED_CS, LOW);
    digitalWrite(OLED_RST, HIGH);
    delay(10);
    digitalWrite(OLED_RST, LOW);
    delay(10);
    digitalWrite(OLED_RST, HIGH);
    
    sendCommand(0xae);//--turn off oled panel
	
    sendCommand(0xd5);//--set display clock divide ratio/oscillator frequency
    sendCommand(0x80);//--set divide ratio

    sendCommand(0xa8);//--set multiplex ratio(1 to 64)
    sendCommand(0x3f);//--1/64 duty

    sendCommand(0xd3);//-set display offset
    sendCommand(0x00);//-not offset

    sendCommand(0x8d);//--set Charge Pump enable/disable
    sendCommand(0x14);//--set(0x10) disable

    sendCommand(0x40);//--set start line address

    sendCommand(0xa6);//--set normal display

    sendCommand(0xa4);//Disable Entire Display On

    sendCommand(0xa1);//--set segment re-map 128 to 0

    sendCommand(0xC8);//--Set COM Output Scan Direction 64 to 0

    sendCommand(0xda);//--set com pins hardware configuration
    sendCommand(0x12);

    sendCommand(0x81);//--set contrast control register
    sendCommand(0xcf);

    sendCommand(0xd9);//--set pre-charge period
    sendCommand(0xf1);

    sendCommand(0xdb);//--set vcomh
    sendCommand(0x40);
    
    sendCommand(0xaf);//--turn on oled panel
}




//  Private functions



