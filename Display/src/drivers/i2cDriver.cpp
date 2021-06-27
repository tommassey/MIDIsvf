#include <Wire.h>
#include "i2cDriver.h"

#define I2C_CMD        0X00
#define I2C_RAM        0X40


SSD1306_I2C_Driver::SSD1306_I2C_Driver(){}


SSD1306_I2C_Driver::SSD1306_I2C_Driver(uint8_t _address)
{
    address = _address;
}

SSD1306_I2C_Driver::~SSD1306_I2C_Driver(){}

// Public functions

void SSD1306_I2C_Driver::sendI2Ccommand(uint8_t command)
{
    I2CwriteByte(command, I2C_CMD);
}

void SSD1306_I2C_Driver::sendI2Cdata(uint8_t data)
{
    I2CwriteByte(data, I2C_RAM);
}

void SSD1306_I2C_Driver::setAddress(uint8_t newAddress)
{
    address = newAddress;
}


//  Private functions

void SSD1306_I2C_Driver::I2CwriteByte(uint8_t value, uint8_t Cmd)
{
  Wire.beginTransmission(address);
  Wire.write(Cmd);
  Wire.write(value);
  Wire.endTransmission();
}


