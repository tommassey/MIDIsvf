// #include "SSD1306_i2c.h"
// #include <Wire.h>

// //I2C
// #define IIC_CMD        0X00
// #define IIC_RAM        0X40
// #define command(Reg)  I2C_Write_Byte(Reg, IIC_CMD)
// #define data(Data)    I2C_Write_Byte(Data, IIC_RAM)

// #define MEMORYMODE 0x20          ///< See datasheet
// #define COLUMNADDR 0x21          ///< See datasheet
// #define PAGEADDR 0x22            ///< See datasheet
// #define SETCONTRAST 0x81         ///< See datasheet
// #define CHARGEPUMP 0x8D          ///< See datasheet
// #define SEGREMAP 0xA0            ///< See datasheet
// #define DISPLAYALLON_RESUME 0xA4 ///< See datasheet
// #define DISPLAYALLON 0xA5        ///< Not currently used
// #define NORMALDISPLAY 0xA6       ///< See datasheet
// #define INVERTDISPLAY 0xA7       ///< See datasheet
// #define SETMULTIPLEX 0xA8        ///< See datasheet
// #define DISPLAYOFF 0xAE          ///< See datasheet
// #define DISPLAYON 0xAF           ///< See datasheet
// #define COMSCANINC 0xC0          ///< Not currently used
// #define COMSCANDEC 0xC8          ///< See datasheet
// #define SETDISPLAYOFFSET 0xD3    ///< See datasheet
// #define SETDISPLAYCLOCKDIV 0xD5  ///< See datasheet
// #define SETPRECHARGE 0xD9        ///< See datasheet
// #define SETCOMPINS 0xDA          ///< See datasheet
// #define SETVCOMDETECT 0xDB       ///< See datasheet

// #define SETPAGE 0xB0
// #define SETLOWCOLUMN 0x00  
// #define SETHIGHCOLUMN 0x10 
// #define SETSTARTLINE 0x40  ///< See datasheet

// #define EXTERNALVCC 0x01  ///< External display voltage source
// #define SWITCHCAPVCC 0x02 ///< Gen. display voltage from 3.3V

// #define RIGHT_HORIZONTAL_SCROLL 0x26              ///< Init rt scroll
// #define LEFT_HORIZONTAL_SCROLL 0x27               ///< Init left scroll
// #define VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29 ///< Init diag scroll
// #define VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A  ///< Init diag scroll
// #define DEACTIVATE_SCROLL 0x2E                    ///< Stop scroll
// #define ACTIVATE_SCROLL 0x2F                      ///< Start scroll
// #define SET_VERTICAL_SCROLL_AREA 0xA3             ///< Set scroll range


// SSD1306_i2c::SSD1306_i2c(uint8_t addr, uint16_t w, uint16_t h, uint8_t* buf)
// {
//     Serial.println("constructor");
//     Wire.begin();

//     address = addr;
//     width = w;
//     height = h;

//     buffer = buf;
// }

// SSD1306_i2c::~SSD1306_i2c()
// {
// }

// void SSD1306_i2c::begin(void)
// {
//     Serial.println("begin");

//     command(DISPLAYOFF);
	
//     command(SETDISPLAYCLOCKDIV);
//     command(0x80); // set divide ratio

//     command(SETMULTIPLEX); // multiplex ratio(1 to 64)
//     command(0x3f); // 1/64 duty

//     command(SETDISPLAYOFFSET);
//     command(0x00); // not offset

//     command(CHARGEPUMP);
//     command(0x14); //  (0x10)disable

//     command(SETSTARTLINE);

//     command(NORMALDISPLAY);  // ie not INVERTDISPLAY

//     command(DISPLAYALLON_RESUME);//Disable Entire Display On

//     command(0xa1);//--set segment re-map 128 to 0

//     command(COMSCANDEC);//--Set COM Output Scan Direction 64 to 0

//     command(SETCOMPINS);//--set com pins hardware configuration
//     command(0x12);

//     command(0x81);//--set contrast control register
//     command(0xcf);

//     command(SETPRECHARGE);//--set pre-charge period
//     command(0xf1);

//     command(SETVCOMDETECT);//--set vcomh
//     command(0x40);
    
//     command(DISPLAYON);//--turn on oled panel
// }

// void SSD1306_i2c::I2C_Write_Byte(uint8_t value, uint8_t cmd)
// {
//   Serial.println("write byte");

//   Wire.beginTransmission(address);
//   Wire.write(cmd);
//   Wire.write(value);
//   Wire.endTransmission();
// }

// void SSD1306_i2c::display(void)
// {
//     Serial.println("display");

//     for (uint8_t page = 0; page < pages; page++)
//     {         
//         command(SETPAGE + page);/* set page address */     
//         command(SETLOWCOLUMN);   /* set low column address */      
//         command(SETHIGHCOLUMN);  /* set high column address */           
        
//         for(uint8_t i = 0; i < width; i++)
//         {
//           data(buffer[i+ page * width]);// write data one
//         }        
//     }
// }

// void SSD1306_i2c::pixel(int x, int y, bool color)
// {
//     Serial.println("pixel");

//     if (x > width || y > height) return;
    
//     if (color)
//     {
//         buffer[x+(y/8)*width] |= 1<<(y%8);
//     }

//     else
//     {
//         buffer[x+(y/8)*width] &= ~(1<<(y%8));
//     }
        
// }

// void SSD1306_i2c::bitmap(uint8_t x,uint8_t y,const uint8_t *pBmp, uint8_t chWidth, uint8_t chHeight)
// {
//     Serial.println("bitmap");

// 	uint8_t byteWidth = (chWidth + 7)/8;

// 	for(uint8_t j = 0;j < chHeight;j++)
//     {
// 		for(uint8_t i = 0;i <chWidth;i++)
//         {
// 			if(pgm_read_byte(pBmp + j * byteWidth + i / 8) & (128 >> (i & 7)))
//             {
// 			    pixel(x + i, y + j, 1);
// 			}
// 		}
// 	}		
// }


// void SSD1306_i2c::er_oled_clear(void)
// {
//     Serial.println("clear");

// 	int i;
// 	for(i = 0;i < width * height / 8;i++)
// 	{
// 		buffer[i] = 0;
// 	}
// }




// void SSD1306_i2c::er_oled_pixel(int x, int y, char color)
// {
//     Serial.println("pixel er");

//     if(x > width || y > height)return ;
//     if(color)
//         buffer[x+(y/8)*width] |= 1<<(y%8);
//     else
//         buffer[x+(y/8)*width] &= ~(1<<(y%8));
// }


// void SSD1306_i2c::er_oled_bitmap(uint8_t x,uint8_t y,const uint8_t *pBmp, uint8_t chWidth, uint8_t chHeight)
// {
//     Serial.println("bitmap er");

// 	uint8_t i, j, byteWidth = (chWidth + 7)/8;
// 	for(j = 0;j < chHeight;j++){
// 		for(i = 0;i <chWidth;i++){
// 			if(pgm_read_byte(pBmp + j * byteWidth + i / 8) & (128 >> (i & 7))){
// 				er_oled_pixel(x + i,y + j, 1);
// 			}
// 		}
// 	}		
// }

// void SSD1306_i2c::er_oled_display()
// {
//     Serial.println("display er");

//         uint8_t page,i;   
//     for (page = 0; page < pages; page++) {         
//         command(0xB0 + page);/* set page address */     
//         command(0x00);   /* set low column address */      
//         command(0x10);  /* set high column address */           
//         for(i = 0; i< width; i++ ) {
//           data(buffer[i+page*width]);// write data one
//         }        
//     }
// }
