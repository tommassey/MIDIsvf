#include "receiver.h"
#define MESSAGE_LENGTH 4


enum messageByteNames
{
    COMMAND_BYTE = 0,
    DATA_BYTE_1 = 1,
    DATA_BYTE_2 = 2,
    TERMINATOR = '\n'
};



void receiverInit(void)
{
  //callCommandFunction(command_LFO1_setAmp);
}


const byte commandMax = 32;   //  size of command buffer
byte commandCount = 0;
byte currentCommand = 0;
serialMessage commandList[commandMax];
// Sample input: 255,255,255\n
char inputBuffer[6];



//uint16_t newCommand[3] = {};
byte newByte[2] = {};
int counter = 0;



bool getSerialMessage(void)  //  return true if new message added to commandList
{
  if(Serial2.available() > 0)
  {
    // readBytesUntil 
    // terminates if length characters have been read, timeout, or if the terminator character  detected
    // returns the number of characters placed in the buffer (0 means no valid data found)
    
    uint8_t count = Serial2.readBytesUntil(TERMINATOR, inputBuffer, 5);

    if (count == 0)  //  0 means no data found
    {
      Serial.println("count == 0. No valid serial message found");
      return false;
    }

    else if (count == MESSAGE_LENGTH)  // valid message length
    {
      serialMessage newMessage;

      newMessage.messageType       = inputBuffer[COMMAND_BYTE];
      newMessage.data[DATA_BYTE_1] = inputBuffer[DATA_BYTE_1];
      newMessage.data[DATA_BYTE_2] = inputBuffer[DATA_BYTE_2];

      commandList[commandCount] = newMessage;

      commandCount++;
      if (commandCount >= commandMax) commandCount = 0;

      Serial.print ("New serial message,   type = ");
      Serial.print (newMessage.messageType);
      Serial.print (",  data byte 1 = ");
      Serial.print (newMessage.data[DATA_BYTE_1]);
      Serial.print (", byte 2 = ");
      Serial.println (newMessage.data[DATA_BYTE_2]);
      return true;
    }
    
    else
    {
      Serial.print("count == ");
      Serial.print(count);
      Serial.println(" . No valid serial message found");
      return false;
    }
  }

  return false;
}

// serialMessage passCommand(void)
// {
//   serialMessage newMessage = commandList[commandCount];
//   commandCount--;
//   return newMessage;
// }

byte getCommandCount(void)
{
  return commandCount;
}


void commandListWorker(void)
{
  byte command = commandList[currentCommand].messageType;
  byte data1 = commandList[currentCommand].data[0];
  byte data2 = commandList[currentCommand].data[1];

  uint16_t val = ((uint16_t)data2 << 8) | data1;   // combine two bytes to make uint16

  recieveCommand(command, val);  // send command to menu

  commandCount--;
  currentCommand++;

  if (currentCommand >= commandMax) currentCommand = 0;
}

void receiverTask(void)
{
    getSerialMessage();

    if (commandCount > 0)
    {
      commandListWorker();
    }
}





















// enum messageByteNames
// {
//     commandByte = 0,
//     dataByte = 1,

// };






// // Sample input: 1,2\n
// char inputBuffer[40];
// uint16_t newCommand[3] = {};
// byte newByte[2] = {};
// int counter = 0;



// serialMessage getSerialMessage(void)  
// {
//     serialMessage newMessage;

//     newMessage.messageType = 0;
//     newMessage.data = 0;

//     if(Serial2.available() > 0)
//     {
//         Serial2.readBytesUntil('\n', inputBuffer, 10);

//         Serial.print("buffer = ");
//         Serial.println(inputBuffer);



//         char* pch = strtok(inputBuffer,",\r\n");

//     while(pch != NULL)
//     {
//       //newCommand[counter] = atoi(pch);
//       newByte[counter] = *pch;
//       pch = strtok(NULL, ",\r\n");
//       counter++;
//     }

//     counter = 0;

//     newMessage.messageType = newByte[commandByte];
//     newMessage.data = (newByte[dataByte]);    //  -48 temp so we can test with ascii via serial monitor

//     Serial.print ("New serial message,   type = ");
//     Serial.print (newMessage.messageType);
//     Serial.print (",  data = ");
//     Serial.println (newMessage.data);

    
    
//   }
//     return newMessage;
// }

