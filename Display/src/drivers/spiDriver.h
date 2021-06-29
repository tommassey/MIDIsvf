#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

#include <Arduino.h>



class SSD1306_SPI_Driver
{
private:

    byte OLED_DC = 15;
    byte OLED_CS = 10;
    byte OLED_RST = 14;

    void begin();

public:
    SSD1306_SPI_Driver();
    ~SSD1306_SPI_Driver();

    void sendCommand(uint8_t command);
    void sendBytes(uint8_t *buffer, int bufferLength);
    void setDChigh();
};




#endif  // SPI_DRIVER_H