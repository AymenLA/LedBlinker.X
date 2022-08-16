#include <xc.h>
#include "System.h"
#include "Leds.h"
#include "Buttons.h"

/** CONFIGURATION Bits **********************************************/
#pragma config PLLDIV   = 1         // (No Prescaler)
#pragma config CPUDIV   = OSC1_PLL2
#pragma config USBDIV   = 1         // USB clock source comes directly from the primary oscillator block with no postscale
#pragma config FOSC     = INTOSC_HS
#pragma config FCMEN    = OFF
#pragma config IESO     = OFF
#pragma config PWRT     = OFF
#pragma config BOR      = OFF
#pragma config BORV     = 3
#pragma config VREGEN   = OFF      //USB voltage regulator disabled
#pragma config WDT      = OFF
#pragma config WDTPS    = 32768
#pragma config MCLRE    = ON
#pragma config LPT1OSC  = OFF
#pragma config PBADEN   = OFF
//#pragma config CCP2MX   = ON
#pragma config STVREN   = ON
#pragma config LVP      = OFF
//#pragma config ICPRT    = OFF       // Dedicated In-Circuit Debug/Programming
#pragma config XINST    = OFF       // Extended Instruction Set
#pragma config CP0      = OFF
#pragma config CP1      = OFF
//#pragma config CP2      = OFF
//#pragma config CP3      = OFF
#pragma config CPB      = OFF
//#pragma config CPD      = OFF
#pragma config WRT0     = OFF
#pragma config WRT1     = OFF
//#pragma config WRT2     = OFF
//#pragma config WRT3     = OFF
#pragma config WRTB     = OFF       // Boot Block Write Protection
#pragma config WRTC     = OFF
//#pragma config WRTD     = OFF
#pragma config EBTR0    = OFF
#pragma config EBTR1    = OFF
//#pragma config EBTR2    = OFF
//#pragma config EBTR3    = OFF
#pragma config EBTRB    = OFF


/*********************************************************************
* Function: void SYSTEM_Initialize( SYSTEM_STATE state )
*
* Overview: Initializes the system.
*
* PreCondition: None
*
* Input:  SYSTEM_STATE - the state to initialize the system into
*
* Output: None
*
********************************************************************/
void SYSTEM_Initialize( SYSTEM_STATE state )
{
    switch(state)
    {
        case SYSTEM_STATE_START:
            LED_Enable(LED_STATE);
            LED_Enable(LED_CUSTOM);
            
            BUTTON_Enable(BUTTON_CUSTOM);
            
            break;
            
        case SYSTEM_STATE_SUSPEND:
            break;
            
        case SYSTEM_STATE_RESUME:
            break;
    }
}

			
#if(__XC8_VERSION < 2000)
    #define INTERRUPT interrupt
#else
    #define INTERRUPT __interrupt()
#endif			

void INTERRUPT SYS_InterruptHigh(void)
{
    #if defined(APP_INTERRUPT)
        APPTasks();
    #endif
}

