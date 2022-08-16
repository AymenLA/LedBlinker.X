#include <stdbool.h>
#include <xc.h>
#include "Buttons.h"

// *****************************************************************************
// *****************************************************************************
// Section: File Scope or Global Constants
// *****************************************************************************
// *****************************************************************************
//      S1       MCLR reset
#define S2_PORT  PORTBbits.RB4      //AN11
#define S3_PORT  PORTBbits.RB5      

#define S2_TRIS  TRISBbits.TRISB4
#define S3_TRIS  TRISBbits.TRISB5

#define BUTTON_PRESSED      0
#define BUTTON_NOT_PRESSED  1

#define PIN_INPUT           1
#define PIN_OUTPUT          0

#define PIN_DIGITAL         1
#define PIN_ANALOG          0


// *****************************************************************************
// *****************************************************************************
// Section: Macros or Functions
// *****************************************************************************
// *****************************************************************************

bool BUTTON_IsPressed(BUTTON button)
{
    switch(button)
    {
        case BUTTON_S2:
            return ( (S2_PORT == BUTTON_PRESSED) ? true : false);
			
        case BUTTON_S3:
            return ( (S3_PORT == BUTTON_PRESSED) ? true : false);

        case BUTTON_NONE:
            return false;
    }
    
    return false;
}

void BUTTON_Enable(BUTTON button)
{
    switch(button)
    {
        case BUTTON_S2:
            S2_TRIS = PIN_INPUT;
            break;
			
        case BUTTON_S3:
            S3_TRIS = PIN_INPUT;
            break;

        case BUTTON_NONE:
            break;
    }
}


/*******************************************************************************
 End of File
*/
