#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include <Arduino.h>



class SSD1306_I2C_Driver
{
private:
    uint8_t address = 0x3c;   // I2C address of SSD1306, default 0x3C
    void I2CwriteByte(uint8_t value, uint8_t Cmd);

public:
    SSD1306_I2C_Driver();
    SSD1306_I2C_Driver(uint8_t _address);
    ~SSD1306_I2C_Driver();

    void sendI2Ccommand(uint8_t command);
    void sendI2Cdata(uint8_t data);
    void setAddress(uint8_t newAddress);
};



#endif  // I2C_DRIVER_H