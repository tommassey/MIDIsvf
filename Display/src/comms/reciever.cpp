#include "reciever.h"

enum messageByteNames
{
    commandByte = 0,
    dataByte = 1,

};






// Sample input: 1,2\n
char inputBuffer[40];
uint16_t newCommand[3] = {};
byte newByte[2] = {};
int counter = 0;



serialMessage getSerialMessage(void)  
{
    serialMessage newMessage;

    newMessage.messageType = 0;
    newMessage.data = 0;

    if(Serial2.available() > 0)
    {
        Serial2.readBytesUntil('\n', inputBuffer, 10);

        Serial.print("buffer = ");
        Serial.println(inputBuffer);



        char* pch = strtok(inputBuffer,",\r\n");

    while(pch != NULL)
    {
      //newCommand[counter] = atoi(pch);
      newByte[counter] = *pch;
      pch = strtok(NULL, ",\r\n");
      counter++;
    }

    counter = 0;

    newMessage.messageType = newByte[commandByte];
    newMessage.data = (newByte[dataByte]);    //  -48 temp so we can test with ascii via serial monitor

    Serial.print ("New serial message,   type = ");
    Serial.print (newMessage.messageType);
    Serial.print (",  data = ");
    Serial.println (newMessage.data);

    
    
  }
    return newMessage;
}

