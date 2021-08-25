#include "encoderService.h"


EncoderService::EncoderService(Encoder* enc)
{
    encoder = enc;
}

uint16_t EncoderService::getValue(void)
{
  return value;
}

void EncoderService::update(void)
{
  
  int32_t newRawCount = encoder->read();

  //Serial.println(newRawCount);

 
  if (newRawCount != 0)           //  if it's changed
  {
    //Serial.println("enc not zero");

    int16_t newMovement = checkForAcceleration(newRawCount);  //  get movement

    int32_t newValue = value + newMovement;

    if (newValue != value)
    {
      //Serial.print("rawCount = ");
      //Serial.print(newRawCount);
      //Serial.print("     step size = ");
      //Serial.print(newMovement);
      Serial.print("  Encoder value = ");
      Serial.println(newValue);
      
      value = newValue;
    }
  } 

  encoder->write(0);  // reset encoder for next read
}

int16_t EncoderService::checkForAcceleration(int32_t rawCount)
{
  // increasing
  if (rawCount > accelerationHigh)
  {
    //Serial.print(" big+ ");
    return bigStep;
  }
  
  if (rawCount > accelerationMed)
  {
    //Serial.print(" med+ ");
    return medStep;
  }
  
  if (rawCount > accelerationLow)
  {
    //Serial.print(" sml+ ");
    return smallStep;
  }

  // near zero
  if ((rawCount == 0) || (rawCount == 1) || (rawCount == -1))
  {
    //Serial.print(" none ");
    return 0;
  }

  // decreasing
  if (rawCount < -accelerationHigh)
  {
    //Serial.print(" big- ");
    return (0 - bigStep);
  }

  if (rawCount < -accelerationMed)
  {
    //Serial.print(" med- ");
    return (0 - medStep);
  }

  if (rawCount < -accelerationLow)
  {
    //Serial.print(" sml- ");
    return (0 - smallStep);
  }

  else return 0;
}


