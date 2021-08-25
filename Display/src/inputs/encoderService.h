#ifndef ENCODER_SERVICE_H
#define ENCODER_SERVICE_H

#include <Arduino.h>
#include <Encoder.h>


enum EncoderMovement
{
    down,
    down_fast,
    down_med,
    down_slow,
    no_movement,
    up_slow,
    up_med,
    up_fast,
    up
};


class EncoderService
{
    /*
        we poll update(); in the main loop with a timer

        acceleration works by detecting the raw encoder count movement between pollings
        depending on the bands set by accelerationHigh/Med/Low, it returns a step value 
        from small/med/bigStep to be added to the encoder value count

    */
    public:
    EncoderService(Encoder* enc);


    

    void update(void);   //  poll this to update encoder



    private:

        Encoder* encoder;  // = Encoder(pin1, pin2);
        
        

        

        

        uint8_t _pin1;
        uint8_t _pin2;

        //Encoder encoder = Encoder(_pin1, _pin2);

        int32_t rawCount = 0;
        int32_t value = 0;


        // for encoder 'acceleration'

        const int8_t accelerationHigh = 7;  //  bands to determine how fast encoder is moving
        const int8_t accelerationMed = 3;
        const int8_t accelerationLow = 1;
        
        const int16_t smallStep = 1;         //  how many get added when in relevant band
        const int16_t medStep = 10;  
        const int16_t bigStep = 50;

        int16_t checkForAcceleration(int32_t rawCount);
        void applyAcceleration(EncoderMovement movement);



};





#endif // ENCODER_SERVICE_H