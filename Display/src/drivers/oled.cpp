#include "oled.h"

#include "resources/fonts.h"
#include "resources/bitmaps.h"



#ifndef _swap_int16_t
#define _swap_int16_t(a, b)                                                    \
  {                                                                            \
    int16_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }
#endif



oled::oled(/* args */)
{
    initScreenBuffer();

  spi.sendCommand(0xD5);  // reset clock
  spi.sendCommand(0x80);  // reset clock

  spi.sendCommand(0xD9);  // reset mux
  spi.sendCommand(0x22);  // reset mux
  
  spi.sendCommand(0x26);  // right scroll
  spi.sendCommand(0x00);  // a dummy
  spi.sendCommand(0x00);  // b start page
  spi.sendCommand(0x07);  // c interval
  spi.sendCommand(0x07);  // d end page
  spi.sendCommand(0x01);  // e start column
  spi.sendCommand(0xf);  // f end column
}


oled::oled(uint16_t width, uint16_t height)
{
    WIDTH = width;
    HEIGHT = height;
    initScreenBuffer();

    spi.sendCommand(0x26);  // right scroll
  spi.sendCommand(0x00);  // a dummy
  spi.sendCommand(0x00);  // b start page
  spi.sendCommand(0x07);  // c interval
  spi.sendCommand(0x07);  // d end page
  spi.sendCommand(0x01);  // e start column
  spi.sendCommand(0x7f);  // f end column
}


oled::~oled()
{
    if (buffer) free(buffer);
}



//===================================================================  Initialisation

void oled::initScreenBuffer()
{
    uint16_t bytes = ((WIDTH + 7) / 8) * HEIGHT;
    
    if ((buffer = (uint8_t*)malloc(bytes)))
    {
      memset(buffer, 0, bytes);
    }

}



//===================================================================  Basics

void oled::display()  // write the buffer to the oled
{   
  uint8_t* pBuf = buffer;

  for (uint8_t page = 0; page < PAGES; page++) {         
      spi.sendCommand(0xB0 + page);/* set page address */     
      spi.sendCommand(0x00);   /* set low column address */      
      spi.sendCommand(0x10);  /* set high column address */      
      //digitalWrite(OLED_DC, HIGH);
      //SPIWrite(pBuf, WIDTH); /* write data  one page*/
      //pBuf += WIDTH;        
      for(uint8_t i = 0; i< WIDTH; i++ ) {
        spi.setDChigh();
        spi.sendBytes(pBuf, 1);// write data one
        *pBuf++;
      }        
  }
}



void oled::clear(void)
{
	for(uint16_t i = 0; i < WIDTH * HEIGHT / 8; i++)
	{
		buffer[i] = 0;
	}
}


//===================================================================  Scrolling


void oled::startScroll()
{
  spi.sendCommand(0x2F);  //  activate scroling
}



void oled::stopScroll()
{
  spi.sendCommand(0x2E);  //  stop scroling
}



void oled::invert(void)
{
  spi.sendCommand(0xa7);//--set Negative display  
}

void oled::uninvert(void)
{
  spi.sendCommand(0xa6);//--set normal display 
}



//===================================================================  Drawing shapes



void oled::pixel(int x, int y, char color)
{
    if (x > WIDTH || y > HEIGHT) return;

    if (color) buffer[x+(y/8)*WIDTH] |= 1<<(y%8);
    else buffer[x+(y/8)*WIDTH] &= ~(1<<(y%8));
}





void oled::writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    _swap_int16_t(x0, y0);
    _swap_int16_t(x1, y1);
  }

  if (x0 > x1) {
    _swap_int16_t(x0, x1);
    _swap_int16_t(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0 <= x1; x0++) {
    if (steep) {
      pixel(y0, x0, color);
    } else {
      pixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

void oled::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
  writeLine(x, y, x, y + h - 1, color);
}

void oled::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
  writeLine(x, y, (x + w - 1), y, color);
}

void oled::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  drawFastHLine(x, y, w, color);
  drawFastHLine(x, (y-h), w, color);
  drawFastVLine(x, (y-h), h, color);
  drawFastVLine((x+w), (y-h), h, color);
}


void oled::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  for (int16_t i = x; i < x + w; i++)
  {
    drawFastVLine(i, y, h, color);
  }
}
  






void oled::sine(uint8_t rate, uint8_t amp)
{
  
  // //  rate comes in at a max of 255, higher is a slower LFO
  // //  we need it to range between 0 and 14, higher is more cycles onscreen (so faster LFO)

  // uint8_t invRate = 255 - rate;   // invert rate so higher is faster

  // uint8_t cycles = invRate / 18;   // 255 / 18 = 14.16666

  // if (cycles > 14) cycles = 14;
  // if (cycles < 1) cycles = 1;

  // if (cycles == sinCyclesPrevious) return;
  // sinCyclesPrevious = cycles;



  //  amp comes in at a max of 255, higher is a louder LFO
  //  we need it to range between 0 and 35, higher is higher

  uint8_t scaledAmp = amp / 8;

  if (scaledAmp > 35) scaledAmp = 35;
  if (scaledAmp < 1) scaledAmp = 1;

  uint8_t thickness = (rate + 1) * (1 / (scaledAmp/35));

  if (thickness > 8) thickness = 8;
  if (thickness < 4) thickness = 4;
  //uint16_t lineStart = (thickness / 2) - 1;

  clear();

  float y;
  float z;

  for (uint16_t i=0; i<(WIDTH); i++)  
  {   
    y = i * 0.049;   // 0.049 magic number to fit sine to screen
    z = (sin(y * rate) * scaledAmp) + centreY;
    uint16_t lineStartY = (z - (thickness / 2)); // draw vertical line instead of pixel
    drawFastVLine(i, lineStartY, thickness, 1);  // to make waveform thicker
  }

  display();

}


void oled::smallSine(uint8_t centreY, uint8_t rate, int8_t amp, uint8_t phase, uint8_t delay)
{
  uint8_t offset = phase / 2;
  
  // //  rate comes in at a max of 255, higher is a slower LFO
  // //  we need it to range between 0 and 14, higher is more cycles onscreen (so faster LFO)

  // uint8_t invRate = 255 - rate;   // invert rate so higher is faster

  // uint8_t cycles = invRate / 18;   // 255 / 18 = 14.16666

  //uint8_t rate = 3;

  // if (cycles > 14) cycles = 14;
  // if (cycles < 1) cycles = 1;

  // if (cycles == sinCyclesPrevious) return;
  // sinCyclesPrevious = cycles;



  //  amp comes in at a max of 99, higher is a louder LFO
  //  we need it to range between 0 and 10, higher is higher

  float scaledAmp = (amp / 10);

  uint8_t thickness = rate;// * (1 / (scaledAmp/35));

   if (thickness > 4) thickness = 4;
   if (thickness < 3) thickness = 3;

   uint8_t delayHzLineBottom = centreY + 1;
   uint8_t delayHzLineTop = centreY - 1;

   uint8_t delayVtLineLeft = delay - 1;
   uint8_t delayVtLineRight = delay + 1;

  float y;
  float z;
  int16_t modAmp = 0;

 
  //  draw horiz line for delay
  if (delay > 0)
  {
    for (int i = delayHzLineTop; i < delayHzLineBottom; i++)
    {
      drawFastHLine(0, i, delay, 1);  // to make waveform thicker
    }
    
    y = (delay + offset) * 0.049;
    z = (sin(y * rate) * ((smallLFOmaxHeight - 4) / 2)) * (scaledAmp / 10);
    // Serial.print("z   ");
    // Serial.println(z);

    for (int i = delayVtLineLeft; i < delayVtLineRight; i++)
    {
      drawFastVLine(i, centreY, z, 1);  // to make waveform thicker
    }
  }
  


  for (uint16_t i=delay; i<(WIDTH); i++)  
  {   
    //Serial.print("scaled   ");
    //Serial.println(scaledAmp);
       
    if (scaledAmp > 0)  // amp above 0 - non-inverted sine
    {
      //Serial.print("amp over 0   ");
      modAmp = scaledAmp;
      
      //Serial.println(scaledAmp);

      y = (i + offset) * 0.049;   // 0.049 magic number to fit sine to screen
    }
    
    else if (scaledAmp <= 0)  // amp below 0 - inverted sine
    {

      //Serial.print("amp under 0   ");

      modAmp = scaledAmp * -1;
      //Serial.println(scaledAmp);

      y = -(i + offset) * 0.049;   // 0.049 magic number to fit sine to screen
    }
    

    //z = (sin(y * rate) * scaledAmp) + centreY;
    z = (sin(y * rate) * modAmp) + centreY;

    uint16_t lineStartY = (z - (thickness / 2)); // draw vertical line instead of pixel
    drawFastVLine(i, lineStartY, thickness, 1);  // to make waveform thicker
  }

  //display();

}





void oled::triangle(uint8_t rate, uint8_t amp)
{
  if (rate > 14) rate = 14;
  if (rate < 1) rate = 1;

  if (amp > 1) amp = 1;
  if (amp < 0.001) amp = 0.001;  // ensure amp is never 0

  uint8_t thickness = ((rate * 2) + 4);

  if (thickness > 12) thickness = 12;
  if (thickness < 4) thickness = 4;


  float stepY = rate; 

  //stepY = 14;

  uint16_t finishX = WIDTH;// - 1;

  clear();

  for (uint16_t i = 0; i < WIDTH; i++)  
  {
    if (triangleDirectionisUp)
    {      
      uint16_t lineStartY = (triY - (thickness / 2)); // work out bottom of the vertical line we're going to ...
      drawFastVLine(i, lineStartY, thickness, 1);     // draw instead of pixel to make waveform appear thicker

      triY = (triY + stepY);

      if (triY >= HEIGHT-1)
      {
        triangleDirectionisUp = false;
      }
    }

    else if (!triangleDirectionisUp)
    {
      uint16_t lineStartY = (triY - (thickness / 2)); // work out bottom of the vertical line we're going to ...
      drawFastVLine(i, lineStartY, thickness, 1);     // draw instead of pixel to make waveform appear thicker
  
      triY = (triY - stepY);

      if (triY <= 0)
      {
        triangleDirectionisUp = true;
      }
    }
  }
    display();
}


void oled::smallTriangle(uint8_t centreY, uint8_t rate, int8_t amp, uint8_t phase)
{
  float halfLFO = smallLFOmaxHeight / 2;
  float scaledAmp = ((float)amp / 99);
  Serial.print("scaled amp = ");
  Serial.println(scaledAmp);
  float triTop = centreY + (halfLFO * scaledAmp);//) * scaledAmp;
  float triBottom = centreY - (halfLFO * scaledAmp);  //) * scaledAmp;

  Serial.print("top = ");
  Serial.println(triTop);
  Serial.print("bottom = ");
  Serial.println(triBottom);


  uint8_t y = (centreY - (smallLFOmaxHeight / 2) * scaledAmp);


  if (scaledAmp < 0)
  {
    triangleDirectionisUp = true;
  }

  if (scaledAmp >= 0)
  {
    triangleDirectionisUp = false;
  }

  for (uint8_t x = 0; x < WIDTH; x = x + rate)
  {
    drawFastHLine(x, y, rate, 1);
    


    
    if (triangleDirectionisUp && (y < triTop))
    {
      y++;
    }

    if (!triangleDirectionisUp && (y > triBottom))
    {
      y--;
    }


      if (y >= triTop)
      {
        triangleDirectionisUp = false;
        //Serial.println("tri down");
      }
      if (y <= triBottom)
      {
        triangleDirectionisUp = true;
        //Serial.println("tri up");
      }

  }


}


//void oled::smallTriangle(uint8_t centreY, uint8_t rate, int8_t amp, uint8_t phase)
// {

//   uint8_t triangleTop = centreY + 12;
//   uint8_t triangleBottom = centreY - 12;

//   if (rate > 14) rate = 14;
//   if (rate < 1) rate = 1;

//   if (amp > 1) amp = 1;
//   if (amp < 0.001) amp = 0.001;  // ensure amp is never 0

//   uint8_t thickness = (rate / 2) + 1;

//   if (thickness > 8) thickness = 8;
//   if (thickness < 2) thickness = 2;


//   float stepY = rate; 

//   //stepY = 14;

//   uint16_t finishX = WIDTH;// - 1;

//   clear();

//   triY = centreY;

//   for (uint16_t i = 0; i < WIDTH; i++)  
//   {
//     if (triangleDirectionisUp)
//     {      
//       uint16_t lineStartY = (triY - (thickness / 2)); // work out bottom of the vertical line we're going to ...
//       drawFastVLine(i, lineStartY, thickness, 1);     // draw instead of pixel to make waveform appear thicker

//       triY = (triY + stepY);

//       if (triY >= triangleTop)
//       {
//         triangleDirectionisUp = false;
//       }
//     }

//     else if (!triangleDirectionisUp)
//     {
//       uint16_t lineStartY = (triY - (thickness / 2)); // work out bottom of the vertical line we're going to ...
//       drawFastVLine(i, lineStartY, thickness, 1);     // draw instead of pixel to make waveform appear thicker
  
//       triY = (triY - stepY);

//       if (triY <= triangleBottom)
//       {
//         triangleDirectionisUp = true;
//       }
//     }
//   }
//     display();
// }




void oled::saw(uint8_t rate, float amp)
{
  if (rate > 14) rate = 14;
  if (rate < 1) rate = 1;

  if (amp > 1) amp = 1;
  if (amp < 0.001) amp = 0.001;  // ensure amp is never 0

  uint8_t thickness = ((rate / 2) + 1);

  if (thickness > 6) thickness = 6;
  if (thickness < 4) thickness = 4;

  clear();

  float stepY = rate; 
  
  for (uint16_t i=0; i<=(WIDTH); i++)  
  {
    uint16_t lineStartY = (sawY - (thickness / 2)); // draw vertical line instead of pixel
    drawFastVLine(i, lineStartY, thickness, 1);  // to make waveform thicker
    
    sawY = (sawY - stepY);

    if (sawY <= 0)  // when we get to the bottom of the ramp
    {
      sawY = HEIGHT;

      for (uint8_t j = 1; j <= (thickness/2); j++)  // draw the vertical line at start of ramp
      {
        drawFastVLine((i+(j - 1)), 0, HEIGHT, 1);
      }
    }
  }

  display();
}


void oled::square(uint8_t rate, float amp)
{
  float squareX = (float)WIDTH / (float)rate;  // length of the tops and bottoms
  uint16_t squareHalfX = squareX / 2;  // halfway point where we change from hi to lo
  uint16_t squareXpos = 0;  //  how far along the cycle

  uint16_t squareTopY = 0;   //  top of square in pixels
  uint16_t squareBotY = 63;   //  top of square in pixels


  clear();
  

  if ((WIDTH % rate) != 0)
  {
    drawFastVLine(0, 0, 64, 1);  // draw upright
  }
  

  for (uint16_t i = 0; i <= (WIDTH); i++)
  {    
    if(squareIsAtTop && (squareXpos <= squareHalfX))
    {
      pixel((i), squareTopY, 1);
      squareXpos++;
      if (squareXpos >= squareHalfX)
      {
        drawFastVLine(i, 0, 64, 1);  // draw upright
        squareIsAtTop = false;
      }
    }
    else if(!squareIsAtTop && (squareXpos <= squareX))
    {
      pixel((i), squareBotY, 1);
      squareXpos++;
      if (squareXpos >= squareX)
      {
        drawFastVLine(i, 0, 64, 1);  // draw upright
        squareIsAtTop = true;
        squareXpos = 0;
      }
    }
  }
    display();
}



void oled::smallSquare(uint8_t centreY, uint8_t rate, int8_t amp, uint8_t phase, uint8_t delay)
{

  float squareX = (float)WIDTH / ((float)rate);  // length in pixels of a whole cylce
  uint16_t squareHalfX = squareX / 2;            // halfway point where we change from hi to lo

  float scaledPhase = (float)phase / 255.0;
  uint8_t offset = squareX * scaledPhase;       //  percentage of a cycle we skip due to phase shift

  // Serial.print("smallsquare phase = ");
  // Serial.println(scaledPhase);
  // Serial.print("smallsquare scaled offset = ");
  // Serial.println(offset);


  uint8_t delayHzLineBottom = centreY + 1;   //  for the delay line
  uint8_t delayHzLineTop = centreY - 1;
  uint8_t delayVtLineLeft = delay - 1;
  uint8_t delayVtLineRight = delay + 1;


  //  amplitude scaled as a number of pixels to fit one side (+ or -) of the waveform.  2 * scaledAmp = peak to peak
  float scaledAmp = ((float)smallLFOhalfHeight - 2) * ((float)amp / 99.0);  //  99 is amp max

  // Serial.print("smallsquare amp = ");
  // Serial.println(amp);
  // Serial.print("smallsquare scaled amp = ");
  // Serial.println(scaledAmp);




if ((delay > 0))
{
  //  draw horiz line for delay
  for (int i = delayHzLineTop; i < delayHzLineBottom; i++)
  {
    drawFastHLine(0, i, (delay), 1);  // to make waveform thicker
  }
  
  // draw first vertical line
  if (offset <= squareHalfX)   //  if phase is set to first half of cycle
  {
    for (int i = delayVtLineLeft; i < delayVtLineRight; i++)
    {
      drawFastVLine(i, centreY, scaledAmp, 1);  // to make waveform thicker
    }
  }
  else   //  if phase is set to second half of cycle
  {
    for (int i = delayVtLineLeft; i < delayVtLineRight; i++)
    {
      drawFastVLine(i, centreY, -scaledAmp, 1);  // to make waveform thicker
    }
  }
}

//  draw first phase adjusted cycle

uint8_t currentXpos = delay;


  if (offset <= squareHalfX) //  if the phase is less than a half cycle
  {
    drawFastHLine(currentXpos, (centreY + scaledAmp), (squareHalfX - offset), 1);
    drawFastHLine(currentXpos, (centreY + scaledAmp - 1), (squareHalfX - offset), 1);  //  thicker

    currentXpos = currentXpos + squareHalfX - offset;

    drawFastVLine(currentXpos, (centreY + scaledAmp), -((scaledAmp * 2) - 1),  1);
    drawFastVLine(currentXpos + 1, (centreY + scaledAmp), -((scaledAmp * 2) - 1),  1);  //  thicker

    squareIsAtTop = true;

    drawFastHLine(currentXpos, (centreY - scaledAmp), squareHalfX, 1);  
    drawFastHLine(currentXpos, (centreY - scaledAmp + 1), squareHalfX, 1);  // thicker

    currentXpos = currentXpos + squareHalfX;

    drawFastVLine(currentXpos, (centreY + scaledAmp), -((scaledAmp * 2) - 1),  1);
    drawFastVLine(currentXpos - 1, (centreY + scaledAmp), -((scaledAmp * 2) - 1),  1); // thicker

  }

  if (offset > squareHalfX) //  if the phase is more than a half cycle
  {
    squareIsAtTop = true;
    drawFastHLine((currentXpos), (centreY - scaledAmp), (squareHalfX + (squareHalfX - offset)), 1);
    drawFastHLine((currentXpos), (centreY - scaledAmp + 1), (squareHalfX + (squareHalfX - offset)), 1);  // thicker
    currentXpos = currentXpos + squareX - offset;

    drawFastVLine(currentXpos, (centreY + scaledAmp), -((scaledAmp * 2) - 1),  1);
    drawFastVLine(currentXpos - 1, (centreY + scaledAmp), -((scaledAmp * 2) - 1),  1);  // thicker
  }
  



  for (uint8_t i = (currentXpos); i < WIDTH; i = i + squareX)  // draw complete cycle
  {
    drawFastHLine(currentXpos, (centreY + scaledAmp), squareHalfX, 1);  
    drawFastHLine(currentXpos, (centreY + scaledAmp - 1), squareHalfX, 1);  // to make waveform thicker
    currentXpos = currentXpos + squareHalfX;
  
    drawFastVLine(currentXpos, (centreY + scaledAmp), -((scaledAmp * 2) - 1),  1);
    drawFastVLine(currentXpos + 1, (centreY + scaledAmp), -((scaledAmp * 2) - 1),  1);  // thicker

    squareIsAtTop = true;

    drawFastHLine(currentXpos, (centreY - scaledAmp), squareHalfX, 1);  
    drawFastHLine(currentXpos, (centreY - scaledAmp + 1), squareHalfX, 1);  // thicker
    currentXpos = currentXpos + squareHalfX;

    drawFastVLine(currentXpos, (centreY + scaledAmp), -((scaledAmp * 2) - 1),  1);
    drawFastVLine(currentXpos - 1, (centreY + scaledAmp), -((scaledAmp * 2) - 1),  1);  // thicker

    squareIsAtTop = false;
  }


}








void oled::smallSaw(uint8_t centreY, uint8_t rate, int8_t amp, uint8_t phase, uint8_t delay)
{

  delay = delay / 2;

  float sawX = (float)WIDTH / ((float)rate);  // length in pixels of a whole cylce

  float scaledPhase = (float)phase / 255.0;
  uint8_t offset = sawX * scaledPhase;       //  number of pixels in first cycle we skip due to phase shift



  //  amplitude scaled as a number of pixels to fit one side (+ or -) of the waveform.  2 * scaledAmp = peak to peak
  float scaledAmp = ((float)smallLFOhalfHeight ) * ((float)amp / 99.0);  //  99 is amp max
  
  

  //float sawHeight = ((float)smallLFOhalfHeight*2) * ((float)amp / 99.0);

  //Serial.print("sawHeight = ");
  //Serial.println(sawHeight);


  //float angle = atan(sawHeight/sawX);
  float angle = atan(smallLFOmaxHeight/sawX);

  Serial.print("phase = ");
  Serial.print(phase);
  Serial.print("        angle = ");
  Serial.println(angle);

  float heightFirstPeak;
  int16_t triX;
  float triangleHeight;

  
  if (phase < 128)
  {
    Serial.println("phase under 128");
    triX = (sawX/2) - offset;
    triangleHeight = triX * tan(angle); // * ((float)amp / 99.0);  //  99 is amp max;

    if (amp > 0)
    {
      heightFirstPeak = triangleHeight * ((float)amp / 99.0);
    }
    else // (amp <= 0)
    {
      heightFirstPeak = triangleHeight * ((float)amp / 99.0);
    }
    
  }
  else // if (phase >= 128)
  {
    Serial.println("phase over 128");
    triX = offset;
    triangleHeight = triX * tan(angle); // * ((float)amp / 99.0);  //  99 is amp max;
    
    if (amp > 0)
    {
      heightFirstPeak = (smallLFOhalfHeight - triangleHeight)  * ((float)amp / 99.0);
    }
    else // (amp <= 0)
    {
      //heightFirstPeak = (triangleHeight  * ((float)amp / 99.0)) + smallLFOhalfHeight;
      heightFirstPeak = (smallLFOhalfHeight - triangleHeight)  * ((float)amp / 99.0);
    }
    
  }

  Serial.print("triangle  x = ");
  Serial.print(triX);
  Serial.print("     height = ");
  Serial.println(triangleHeight);



  

  

 //heightFirstPeak = ((sawX - offset) * tan(angle)) / 2; // * ((float)amp / 99.0);  //  99 is amp max;
  
  Serial.print("heightFirstPeak = ");
  Serial.println(heightFirstPeak);


  int8_t peak = heightFirstPeak + centreY; //* ((float)amp / 99.0);// / 2) + smallLFOhalfHeight;// + scaledAmp;// + (smallLFOhalfHeight); // + (scaledAmp / 2);

  Serial.print("peak = ");
  Serial.println(peak);

  //peak = peak;

  uint8_t currentXpos = delay - offset;
  uint8_t nextXpos = currentXpos + sawX;
  //uint8_t nextXpos = sawX - offset;
  int8_t sawTop = centreY - scaledAmp;
  int8_t sawBottom = centreY + scaledAmp;



if ((delay > 0))
{
  //  draw horiz line for delay
  //for (int i = delayHzLineTop; i < delayHzLineBottom; i++)
  //{
    drawFastHLine(0, centreY, (delay), 1);  // to make waveform thicker
  //}
}

 
  // draw first vertical line
  writeLine(delay, centreY, delay, peak, 1);


//  draw first phase adjusted cycle

    //  first line

    writeLine(delay, ((peak * 1)), nextXpos, sawTop, 1);

    currentXpos = nextXpos;
    nextXpos = nextXpos + sawX;

    drawFastVLine(currentXpos, (centreY + scaledAmp), -((scaledAmp * 2) - 1),  1);
    

  for (uint8_t i = (currentXpos); i < WIDTH; i = i + sawX)  // draw complete cycle
  {
    writeLine(currentXpos, sawBottom, nextXpos, sawTop, 1);

    currentXpos = nextXpos;
    nextXpos = nextXpos + sawX;

    drawFastVLine(currentXpos, (centreY + scaledAmp), -((scaledAmp * 2) - 1),  1);
  }


}



//============================================================================== text


void oled::printChar(unsigned char x, unsigned char y, char acsii, char size, char mode)
{
    unsigned char i, j, y0=y;
    char temp;
    unsigned char ch = acsii - ' ';
    for(i = 0;i<size;i++) {
        if(size == 8)
        {
            if(mode)temp = pgm_read_byte(&font[ch][i]);
            else temp = ~pgm_read_byte(&font[ch][i]);
        }
        if(size == 12)
        {
            if(mode)temp = pgm_read_byte(&Font1206[ch][i]);
            else temp = ~pgm_read_byte(&Font1206[ch][i]);
        }
        else 
        {            
            if(mode)temp = pgm_read_byte(&Font1608[ch][i]);
            else temp = ~pgm_read_byte(&Font1608[ch][i]);
        }
        for(j =0;j<8;j++)
        {
            if(temp & 0x80) pixel(x, y, 1);
            else pixel(x, y, 0);
            temp <<= 1;
            y++;
            if((y-y0) == size)
            {
                y = y0;
                x++;
                break;
            }
        }
    }
}



void oled::string(uint8_t x, uint8_t y, const char *pString, uint8_t Size, uint8_t Mode)
{
    while (*pString != '\0') {       
        if (x > (WIDTH - Size / 2)) {
            x = 0;
            y += Size;
            if (y > (HEIGHT - Size)) {
                y = x = 0;
            }
        }
        
        printChar(x, y, *pString, Size, Mode);
        x += Size / 2;
        pString++;
    }
}

void oled::string32pix(uint8_t x, uint8_t y, const char *pString)
{
    while (*pString != '\0') {       
        if (x > (WIDTH - 32 / 2)) {
            x = 0;
            y += 32;
            if (y > (HEIGHT - 32)) {
                y = x = 0;
            }
        }
        
        char3216(x, y, *pString);
        x += 32 / 2;
        pString++;
    }
}

void oled::string16pix(uint8_t x, uint8_t y, const char *pString)
{
    while (*pString != '\0') {       
        if (x > (WIDTH - 16)) {
            x = 0;
            y += 16;
            if (y > (HEIGHT - 16)) {
                y = x = 0;
            }
        }
        
        char1616(x, y, *pString);
        x += 16;
        pString++;
    }
}



void oled::char1616(uint8_t x, uint8_t y, uint8_t chChar)
{
	uint8_t i, j;
	uint8_t chTemp = 0, y0 = y, chMode = 0;

	for (i = 0; i < 32; i++) {
		chTemp = pgm_read_byte(&Font1612[chChar - 0x30][i]);
		for (j = 0; j < 8; j++) {
			chMode = chTemp & 0x80? 1 : 0; 
			pixel(x, y, chMode);
			chTemp <<= 1;
			y++;
			if ((y - y0) == 16) {
				y = y0;
				x++;
				break;
			}
		}
	}
}






void oled::char3216(uint8_t x, uint8_t y, uint8_t chChar)
{
    uint8_t i, j;
    uint8_t chTemp = 0, y0 = y, chMode = 0;

    for (i = 0; i < 64; i++) {
        chTemp = pgm_read_byte(&Font3216[chChar - 0x30][i]);
        for (j = 0; j < 8; j++) {
            chMode = chTemp & 0x80? 1 : 0; 
            pixel(x, y, chMode);
            chTemp <<= 1;
            y++;
            if ((y - y0) == 32) {
                y = y0;
                x++;
                break;
            }
        }
    }
}




void oled::text(uint8_t x, uint8_t y, uint8_t chChar)
{
    uint8_t i, j;
    uint8_t chTemp = 0, y0 = y, chMode = 0;

    for (i = 0; i < 64; i++) {
        chTemp = pgm_read_byte(&Font3216[chChar - 0x30][i]);
        for (j = 0; j < 8; j++) {
            chMode = chTemp & 0x80? 1 : 0; 
            pixel(x, y, chMode);
            chTemp <<= 1;
            y++;
            if ((y - y0) == 32) {
                y = y0;
                x++;
                break;
            }
        }
    }
}




//===================================================================   bitmaps

void oled::bitmap(uint8_t x,uint8_t y,const uint8_t *pBmp, uint8_t chWidth, uint8_t chHeight)
{
	uint8_t i, j, byteWidth = (chWidth + 7)/8;
	for(j = 0;j < chHeight;j++){
		for(i = 0;i <chWidth;i++){
			if(pgm_read_byte(pBmp + j * byteWidth + i / 8) & (128 >> (i & 7))){
				pixel(x + i,y + j, 1);
			}
		}
	}		
}







//  test function

void oled::test()
{
  clear();
//   bitmap(0, 0, PIC2, 128, 64);
//   display();
// //   delay(100);  
// //   spi.sendSPIcommand(0xa7);//--set Negative display 
// //   delay(100);
// //   spi.sendSPIcommand(0xa6);//--set normal display

  
//   clear();
//   /* display images of bitmap matrix */
//   bitmap(0, 2, Signal816, 16, 8); 
//   bitmap(24, 2,Bluetooth88, 8, 8); 

  // string(10, 52, "www.buydisplay.com", 12, 1); 
 

  // char3216(0, 16, '1');
  // char3216(16, 16, '2');
  // char3216(32, 16, ':');
  // char3216(48, 16, '3');
  // char3216(64, 16, '4');
  // char3216(80, 16, ':');
  // char3216(96, 16, '5');
  // char3216(112, 16, '6');

  // display(); 
  // delay(1000);
  // clear();




//   // spi.sendSPIcommand(0x26);  // right scroll
//   // spi.sendSPIcommand(0x00);  // a dummy
//   // spi.sendSPIcommand(0x00);  // b start page
//   // spi.sendSPIcommand(0x07);  // c interval
//   // spi.sendSPIcommand(0x07);  // d end page
//   // spi.sendSPIcommand(0x00);  // e start column
//   // spi.sendSPIcommand(0x7f);  // f end column
//   //   //spi.sendSPIcommand(0x2F);  //  activate scroling
//   //   spi.sendSPIcommand(0x2E);  //  stop scroling

// //int i = 0;

// // clear();
// // sine(9, 10);
// // display();
// // delay(3);
// // spi.sendSPIcommand(0x2F);  //  activate scroling
// // while (1)
// // {}


int i = 0; 

//while(i < 1)
//{
  bitmap(0,0, static1, 128, 64);
  display();
  delay(10);
  spi.sendCommand(0xa7);//--set Negative display 
  delay(10);
  spi.sendCommand(0xa6);//--set normal display
  clear();
  bitmap(0,0, static2, 128, 64);
  display();
  delay(10);
  spi.sendCommand(0xa7);//--set Negative display 
  delay(10);
  spi.sendCommand(0xa6);//--set normal display
  bitmap(0,0, nml3, 128, 64);
  display();
  delay(100);
  clear();
  bitmap(0,0, nml3, 128, 64);
  display();
  spi.sendCommand(0xa7);//--set Negative display 
  delay(100);
  spi.sendCommand(0xa6);//--set normal display
  delay(200);
  spi.sendCommand(0xa7);//--set Negative display 
  delay(50);
  //spi.sendSPIcommand(0xa6);//--set normal display
  delay(2000);
  clear();
  bitmap(0,0, static3, 128, 64);
  display();
  delay(1);
  clear();
  
  
    
    display();

  delay(100);
  //i++;
//} 



// spi.sendSPIcommand(0xa6);//--set normal display
// spi.sendSPIcommand(0x2E);  //  sttop scroling




// // while (1)
// // {
// //   //saw(8,1);
// //   for (uint8_t i = 1; i < 8; i++)
// //   {
// //     square(i, (i * 10));
// //     spi.sendSPIcommand(0x2E);  //  stop scroling

// //     display();
// //     //spi.sendSPIcommand(0x2F);  //  activate scroling

// //     delay(1000);
// //   }
// //   for (uint8_t i = 8; i > 1; i--)
// //   {
// //     square(i, (i * 10));
// //     spi.sendSPIcommand(0x2E);  //  stop scroling

// //     display();
// //     //spi.sendSPIcommand(0x2F);  //  activate scroling

// //     delay(1000);
// //   }
// // }

// //sine(1,1);

// //const char text[] = "BONZER";

// //string(20,36, text, 12, 1);
  
  
//   //writeLine(0,31,127,31, 1);

}


void oled::splashScreen(void)
{

  Serial.println("splashscreen");

  clear();
  bitmap(0,0, PIC1, 128, 64);
  display();
  delay(200);
  spi.sendCommand(0xa7);//--set Negative display 
  delay(1);
  spi.sendCommand(0xa6);//--set normal display
  clear();
  bitmap(0,0, static2, 128, 64);
  display();
  delay(10);
  spi.sendCommand(0xa7);//--set Negative display 
  delay(10);
  spi.sendCommand(0xa6);//--set normal display
  bitmap(0,0, nml3, 128, 64);
  display();
  delay(100);
  clear();
  bitmap(0,0, nml3, 128, 64);
  display();
  spi.sendCommand(0xa7);//--set Negative display 
  delay(100);
  spi.sendCommand(0xa6);//--set normal display
  delay(200);
  spi.sendCommand(0xa7);//--set Negative display 
  delay(50);
  //spi.sendSPIcommand(0xa6);//--set normal display
  delay(200);
  clear();
  bitmap(0,0, static3, 128, 64);
  display();
  
  //while (1)
  //{
  bitmap(0,0, static1, 128, 64);
  display();
  delay(30);
  //spi.sendCommand(0xa7);//--set Negative display 
  delay(10);
  spi.sendCommand(0xa6);//--set normal display
  clear();
  bitmap(0,0, static2, 128, 64);
  display();
  delay(10);
  clear();
  bitmap(0,0, static1, 128, 64);
  display();
  delay(20);
  spi.sendCommand(0xa7);//--set Negative display 
  delay(10);
  //spi.sendCommand(0xa6);//--set normal display
  clear();
  bitmap(0,0, static3, 128, 64);
  display();
  delay(16);
  //}

  




}