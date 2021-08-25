#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <Arduino.h>
#include "inputs.h"
#include "buttonService.h"
#include "encoderService.h"


void inputManager_init(void);

void checkButtons(void);
void checkEncoders(void);







#endif  // INPUT_MANAGER_H