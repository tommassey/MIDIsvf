// #include "waveforms.h"


// void initSineTable(){
//   for (int i = 0; i < sinSteps; i++){
//     sineTable[i] = 2048+2048*sin(i*(2*3.14)/sinSteps);
//     Serial.println(sineTable[i]);
//   }
// }

// void initNMLtable()
// {
//     // zero
//     for (uint8_t i = 0; i < 80; i++)
//     {
//         NMLtable[i] = 0;
//     }

//     // top of N
//     for (uint8_t i = 80; i < 120; i++)
//     {
//         NMLtable[i] = 4095;
//     }

//     // slope N
//     uint16_t part = (2048/40);
//     for (uint16_t i = 120, j = 4095; i < 160; i++, j-=part)
//     {
//         NMLtable[i] = j;
//     }

//     // 2ndtop of N
//     for (uint16_t i = 160; i < 200; i++)
//     {
//         NMLtable[i] = 4095;
//     }
    
//     // gap
//     for (uint16_t i = 200; i < 240; i++)
//     {
//         NMLtable[i] = 0;
//     }

//     // top of M
//     for (uint16_t i = 240; i < 280; i++)
//     {
//         NMLtable[i] = 4095;
//     }

//     // slope M
    
//     for (uint16_t i = 280, j = 4095; i < 320; i++, j-=part)
//     {
//         NMLtable[i] = j;
//     }
//     for (uint16_t i = 320, j = 2048; i < 360; i++, j+=part)
//     {
//         NMLtable[i] = j;
//     }

//     // 2nd top of M
//     for (uint16_t i = 360; i < 400; i++)
//     {
//         NMLtable[i] = 4095;
//     }

//     // bottom
//     for (uint16_t i = 400; i < 440; i++)
//     {
//         NMLtable[i] = 0;
//     }

//     // top of L
//     for (uint16_t i = 440; i < 480; i++)
//     {
//         NMLtable[i] = 4095;
//     }

//     // bar of L
//     for (uint16_t i = 480; i < 560; i++)
//     {
//         NMLtable[i] = 1024;
//     }

//     // flat
//     for (uint16_t i = 560; i < 640; i++)
//     {
//         NMLtable[i] = 0;
//     }





//     for(uint16_t i = 0; i < NMLsteps-1; i++)
//     {
//         Serial.print("step = ");
//         Serial.println(NMLtable[i]);
//     }
// }