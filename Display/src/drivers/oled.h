/***************************************************
//Web: http://www.buydisplay.com
EastRising Technology Co.,LTD
****************************************************/
  
#ifndef _oled_H_
#define _oled_H_

#include <Arduino.h>
#include "spiDriver.h"




class oled
{
    private:

        SSD1306_SPI_Driver spi;
        
        uint16_t HEIGHT = 64;
        uint16_t WIDTH = 128;
        uint16_t PAGES = HEIGHT / 8;
        uint8_t *buffer;

        int16_t centreX = ((WIDTH / 2) - 1);
        int16_t centreY = ((HEIGHT / 2) - 1);


        uint16_t sawY = HEIGHT;
        uint16_t triY = HEIGHT;
        bool triangleDirectionisUp = false;
        bool squareIsAtTop = true;
        

        void initScreenBuffer(void);   //  call at init to allocate for buffer

        void printChar(unsigned char x, unsigned char y, char acsii, char size, char mode);

        void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);


        
        void pixel(int x,int y,char color);
        void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
        void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

        void text(uint8_t x, uint8_t y, uint8_t chChar);
        void char1616(uint8_t x,uint8_t y,uint8_t chChar);
        void char3216(uint8_t x, uint8_t y, uint8_t chChar);
        void string(uint8_t x, uint8_t y, const char *pString, uint8_t Size, uint8_t Mode);
        
        void bitmap(uint8_t x,uint8_t y,const uint8_t *pBmp, uint8_t chWidth, uint8_t chHeight);

    public:
        oled();
        oled(uint16_t width, uint16_t height);
        oled(uint16_t width, uint16_t height, uint8_t I2Caddress);
        ~oled();

        void display();
        void clear();

        
        void sine(uint8_t rate, uint8_t amp);
        void triangle(uint8_t rate, uint8_t amp);
        void saw(uint8_t rate, float amp);
        void square(uint8_t rate, float amp);
        
        void splashScreen(void);

        
        

        void test(void);

        
};






#endif
