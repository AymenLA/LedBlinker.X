#include <stdint.h>
#include "Leds.h"
#include "AppLeds.h"
#include "System.h"
#include "io_mapping.h"

void APP_LEDStatus(void)
{
    uint32_t ledCount = 0U;

    while(ledCount <= 1000U)
    { 
        if(ledCount <= 250U)
        {
            LED_Off(LED_STATE);
        }
        else if(ledCount > 250U)
        {
            LED_Off(LED_STATE);
        }
        else
        {
            /* do nothing */
        }
        
        /* Increment the millisecond counter. */
        ledCount++;
    }
}

/*******************************************************************************
 End of File
*/
