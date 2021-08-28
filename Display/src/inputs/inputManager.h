#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <Arduino.h>
#include <Encoder.h>
#include <Bounce2.h>

#include "hardware/pinDefines.h"

#include "services/buttonService.h"
#include "services/encoderService.h"
#include "inputNames.h"



void inputManager_init(void);

int8_t checkInputs(void);
void checkEncoders(void);
int16_t getEncoderChange(void);
void resetInputFlag(void);









#endif  // INPUT_MANAGER_H