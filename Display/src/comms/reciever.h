#ifndef RECIEVER_H
#define RECIEVER_H

#include <Arduino.h>




struct serialMessage
{
    byte messageType = 0;
    byte data = 0;
};

serialMessage getSerialMessage(void);

//void checkSerial(oled);




#endif