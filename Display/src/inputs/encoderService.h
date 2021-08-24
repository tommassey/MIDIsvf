#ifndef ENCODER_SERVICE_H
#define ENCODER_SERVICE_H

#include <Arduino.h>
#include <Encoder.h>



class EncoderService
{
    
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

        const uint8_t smallStep = 2;
        const uint8_t medStep = 10;  // for encoder 'acceleration'
        const uint8_t bigStep = 50;



};





#endif // ENCODER_SERVICE_H