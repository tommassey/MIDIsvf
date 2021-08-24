#include "encoderService.h"



EncoderService::EncoderService(Encoder* enc)
{
    encoder = enc;
}

void EncoderService::update(void)
{
  int32_t newRawCount = encoder->read();
 
  if (newRawCount != 0)           //  if it's changed
  {
    if (newRawCount > 7)    //  and it's bigger
    {
      Serial.println("big");
      value = (value + bigStep);
    }
    
    if (newRawCount > 3)    //  and it's bigger
    {
      value = (value + medStep);
    }
    
    if (newRawCount > 1)    //  and it's bigger
    {
      value++;
    }

    if (newRawCount < -1)    //  and it's smaller
    {
      value--;
    }
    
    if (newRawCount < -3)    //  and it's smaller
    {
      value = (value - medStep);
    }
    
    if (newRawCount < -6)    //  and it's smaller
    {
      Serial.println("big");
      value = (value - bigStep);
    }

    encoder->write(0);  // reset encoder for next read

    Serial.print("Encoder = ");
    Serial.println(value);
  } 
}