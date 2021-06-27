// #ifndef SSD1306_I2C_H
// #define SSD1306_I2C_H

// #include <Arduino.h>

// class SSD1306_i2c
// {
//     private:
//         uint8_t address = 0x3c;
//         uint8_t* buffer;
        
//         void I2C_Write_Byte(uint8_t value, uint8_t cmd);
//         void pixel(int x, int y, bool color);
    
//     protected:
//         uint16_t width;
//         uint16_t height;
//         uint16_t pages = height / 8;
        
//         //uint8_t buffer[];

        
//     public:
//         SSD1306_i2c(uint8_t addr, uint16_t w, uint16_t h, uint8_t* buf);
//         ~SSD1306_i2c();

//         void er_oled_clear(void);
//         void er_oled_pixel(int x, int y, char color);
//         void er_oled_bitmap(uint8_t x,uint8_t y,const uint8_t *pBmp, uint8_t chWidth, uint8_t chHeight);
//         void er_oled_display();

//         void begin(void);
//         void bitmap(uint8_t x,uint8_t y,const uint8_t *pBmp, uint8_t chWidth, uint8_t chHeight);
//         void display(void);
// };






// #endif // SSD1306_I2C_H