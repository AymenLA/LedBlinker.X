#include <stdbool.h>

#include "Leds.h"
#include <xc.h>

// *****************************************************************************
// *****************************************************************************
// Section: File Scope or Global Constants
// *****************************************************************************
// *****************************************************************************
#define LED_D1_LAT LATDbits.LATD0
#define LED_D2_LAT LATDbits.LATD1
#define LED_D3_LAT LATDbits.LATD2
#define LED_D4_LAT LATDbits.LATD3

#define LED_D1_TRIS TRISDbits.TRISD0
#define LED_D2_TRIS TRISDbits.TRISD1
#define LED_D3_TRIS TRISDbits.TRISD2
#define LED_D4_TRIS TRISDbits.TRISD3

#define LED_ON  1
#define LED_OFF 0

#define INPUT  1
#define OUTPUT 0


// *****************************************************************************
// *****************************************************************************
// Section: Macros or Functions
// *****************************************************************************
// *****************************************************************************

void LED_On(LED led)
{
    switch(led)
    {
        case LED_D1:
            LED_D1_LAT = LED_ON;
            break;

        case LED_D2:
            LED_D2_LAT = LED_ON;
            break;
			
        case LED_D3:
            LED_D3_LAT = LED_ON;
            break;
			
        case LED_D4:
            LED_D4_LAT = LED_ON;
            break;

        case LED_NONE:
            break;
    }
}

void LED_Off(LED led)
{
    switch(led)
    {
        case LED_D1:
            LED_D1_LAT = LED_OFF;
            break;

        case LED_D2:
            LED_D2_LAT = LED_OFF;
            break;
			
        case LED_D3:
            LED_D3_LAT = LED_OFF;
            break;
			
        case LED_D4:
            LED_D4_LAT = LED_OFF;
            break;

        case LED_NONE:
            break;
    }
}

void LED_Toggle(LED led)
{
    switch(led)
    {
        case LED_D1:
            LED_D1_LAT ^= 1;
            break;

        case LED_D2:
            LED_D2_LAT ^= 1;
            break;
			
        case LED_D3:
            LED_D3_LAT ^= 1;
            break;
			
        case LED_D4:
            LED_D4_LAT ^= 1;
            break;

        case LED_NONE:
            break;
    }
}

bool LED_Get(LED led)
{
    switch(led)
    {
        case LED_D1:
            return ( (LED_D1_LAT == LED_ON) ? true : false );

        case LED_D2:
            return ( (LED_D2_LAT == LED_ON) ? true : false );
			
        case LED_D3:
            return ( (LED_D3_LAT == LED_ON) ? true : false );
			
        case LED_D4:
            return ( (LED_D4_LAT == LED_ON) ? true : false );

        case LED_NONE:
            return false;
    }
    
    return false;
}

void LED_Enable(LED led)
{
    switch(led)
    {
        case LED_D1:
            LED_D1_TRIS = OUTPUT;
            break;

        case LED_D2:
            LED_D2_TRIS = OUTPUT;
            break;
			
        case LED_D3:
            LED_D3_TRIS = OUTPUT;
            break;
			
        case LED_D4:
            LED_D4_TRIS = OUTPUT;
            break;

        case LED_NONE:
            break;
    }
}


/*******************************************************************************
 End of File
*/
