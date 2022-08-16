#include <stdint.h>
#include "AppButtons.h"
#include "AppLeds.h"


void APP_DeviceTasks(void) 
{
   APP_ButtonsStatus();
   APP_LEDStatus();
}