// /***************************************************
// //Web: http://www.buydisplay.com
// EastRising Technology Co.,LTD
// Examples for ER-OLEDM0.96-1
// Display is Hardward SPI 4-Wire SPI Interface 
// Tested and worked with:
// Works with Arduino 1.6.0 IDE  
// Test OK : Arduino DUE,Arduino mega2560,Arduino UNO Board 
// ****************************************************/

// /*
//   == Hardware connection ==
//     OLED   =>    Arduino
//   *1. GND    ->    GND
//   *2. VCC    ->    3.3V
//   *3. SCL    ->    SCK
//   *4. SDA    ->    MOSI
//   *5. RES    ->    8 
//   *6. DC     ->    9
//   *7. CS     ->    10
// */


// #include <SPI.h>
// #include "er_oled.h"

// uint8_t oled_buf[WIDTH * HEIGHT / 8];

// void setup() {
//   Serial.begin(9600);
//   Serial.print("OLED Example\n");

//   /* display an image of bitmap matrix */
//   er_oled_begin();
//   er_oled_clear(oled_buf);
//   er_oled_bitmap(0, 0, PIC1, 128, 64, oled_buf);
//   er_oled_display(oled_buf);
//   delay(3000);  
//   command(0xa7);//--set Negative display 
//   delay(3000);
//   command(0xa6);//--set normal display
  
//   er_oled_clear(oled_buf);
//   er_oled_bitmap(0, 0, PIC2, 128, 64, oled_buf);
//   er_oled_display(oled_buf);
//   delay(3000);
  
//   command(0xa7);//--set Negative display 
//   delay(3000); 
//   command(0xa6);//--set normal display  
  
//   er_oled_clear(oled_buf);
//   /* display images of bitmap matrix */
//   er_oled_bitmap(0, 2, Signal816, 16, 8, oled_buf); 
//   er_oled_bitmap(24, 2,Bluetooth88, 8, 8, oled_buf); 
//   er_oled_bitmap(40, 2, Msg816, 16, 8, oled_buf); 
//   er_oled_bitmap(64, 2, GPRS88, 8, 8, oled_buf); 
//   er_oled_bitmap(90, 2, Alarm88, 8, 8, oled_buf); 
//   er_oled_bitmap(112, 2, Bat816, 16, 8, oled_buf); 

//   er_oled_string(10, 52, "www.buydisplay.com", 12, 1, oled_buf); 
 

//   er_oled_char3216(0, 16, '1', oled_buf);
//   er_oled_char3216(16, 16, '2', oled_buf);
//   er_oled_char3216(32, 16, ':', oled_buf);
//   er_oled_char3216(48, 16, '3', oled_buf);
//   er_oled_char3216(64, 16, '4', oled_buf);
//   er_oled_char3216(80, 16, ':', oled_buf);
//   er_oled_char3216(96, 16, '5', oled_buf);
//   er_oled_char3216(112, 16, '6', oled_buf);

//   er_oled_display(oled_buf); 
// }

// void loop() {

// }