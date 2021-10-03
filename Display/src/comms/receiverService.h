#ifndef RECEIVER_SERVICE_H
#define RECEIVER_SERVICE_H

#include <Arduino.h>
#include "receiver.h"
#include "display/displayService.h"


//void (*(commandFunction[command_total]))(uint16_t);

//extern byte commandCount;
//extern serialMessage commandList[255];



void receiverServiceInit(displayService* disp);
void receiverServiceTask(void);






#endif  // RECEIVER_SERVICE_H