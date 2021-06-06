#include "potSmoothing.h"



pot::pot(uint8_t _pin, uint16_t* _externalValue, bool* _changeFlag)
{
    pin = _pin;
    externalValue = _externalValue;
    changeFlag = _changeFlag;

}

pot::~pot()
{
}


void read();    // read pin and add value to array, set changeFlag high

void pot::read()
{
    Serial.println("pot read");
  //total = total - readings[readIndex];      // subtract the last reading
  readings[readIndex] = analogRead(pin);    // read from the sensor
  total = total + readings[readIndex];      // add the reading to the total
  *changeFlag = true;
  //readIndex = readIndex + 1;                // advance to the next position in the array

//   // if we're at the end of the array...
//   if (readIndex >= numReadings) {
//     // ...wrap around to the beginning:
//     readIndex = 0;
//   }

  // calculate the average:
//   average = total / numReadings;
  // send it to the computer as ASCII digits
}

void pot::update()  //  if changeFlag is high, work out average and write the updated value to the external pointer location
{
    readIndex++;                // advance to the next position in the array
    if (readIndex >= numReadings) readIndex = 0;
    average = total / numReadings;
    *externalValue = average;
    Serial.print("pot value = ");
    Serial.println(average);
    total = total - readings[readIndex];      // subtract the last reading
    *changeFlag = false;
    
}
    


// const int numReadings = 10;

// uint16_t readings[numReadings] = {0};      // the readings from the analog input
// int readIndex = 0;              // the index of the current reading
// int total = 0;                  // the running total
// int average = 0;                // the average

// int inputPin = A0;

// void setup() {
//   // initialize serial communication with computer:
//   Serial.begin(9600);
//   // initialize all the readings to 0:
//   for (int thisReading = 0; thisReading < numReadings; thisReading++) {
//     readings[thisReading] = 0;
//   }
// }

// void loop() {
//   // subtract the last reading:
//   total = total - readings[readIndex];
//   // read from the sensor:
//   readings[readIndex] = analogRead(inputPin);
//   // add the reading to the total:
//   total = total + readings[readIndex];
//   // advance to the next position in the array:
//   readIndex = readIndex + 1;

//   // if we're at the end of the array...
//   if (readIndex >= numReadings) {
//     // ...wrap around to the beginning:
//     readIndex = 0;
//   }

//   // calculate the average:
//   average = total / numReadings;
//   // send it to the computer as ASCII digits
//   Serial.println(average);
//   delay(1);        // delay in between reads for stability
// }
