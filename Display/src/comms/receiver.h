#ifndef RECEIVER_H
#define RECEIVER_H

#include <Arduino.h>
#include "../../Shared/commandList.h"
#include "menu/menu.h"



// struct serialMessage
// {
//     byte messageType = 0;
//     byte data = 0;
// };

struct serialMessage
{
    byte messageType = 0;
    byte data[2] = {0};
};

void receiverInit(void);
void receiverTask(void);

// bool getSerialMessage(void);
// byte getCommandCount(void);

// serialMessage passCommand(void);
//void checkSerial(oled);




#endif