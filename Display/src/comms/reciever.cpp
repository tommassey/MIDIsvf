#include "reciever.h"

enum messageByteNames
{
    commandByte = 0,
    dataByte = 1,

};






// Sample input: 1,2,3\n
char inputBuffer[40];
byte newCommand[3] = {};
int counter = 0;



serialMessage getSerialMessage(void)  
{
    serialMessage newMessage;

    if(Serial.available() > 0)
    {
        Serial.readBytesUntil('\n', inputBuffer, 100);
        char* pch = strtok(inputBuffer,",\r\n");

    while(pch != NULL)
    {
      newCommand[counter] = atoi(pch);
      pch = strtok(NULL, ",\r\n");
      counter++;
    }

    counter = 0;

    newMessage.messageType = newCommand[commandByte];
    newMessage.data = newCommand[dataByte];

    Serial.print ("New serial message,   type = ");
    Serial.print (newMessage.messageType);
    Serial.print (",  data = ");
    Serial.println (newCommand[dataByte]);
    
  }
    return newMessage;
}

