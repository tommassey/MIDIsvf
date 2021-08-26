#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <Arduino.h>
#include <Encoder.h>
#include <Bounce2.h>

#include "hardware/pinDefines.h"

#include "services/buttonService.h"
#include "services/encoderService.h"


void inputManager_init(void);

void checkButtons(void);
void checkEncoders(void);









#endif  // INPUT_MANAGER_H