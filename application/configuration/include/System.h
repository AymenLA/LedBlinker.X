
#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include "Buttons.h"
#include "Leds.h"


#include "io_mapping.h"

#define MAIN_RETURN void

/*** System States **************************************************/
typedef enum
{
    SYSTEM_STATE_START,
    SYSTEM_STATE_SUSPEND,
    SYSTEM_STATE_RESUME
} SYSTEM_STATE;

void SYSTEM_Initialize( SYSTEM_STATE state );

//void SYSTEM_Tasks(void);
#define SYSTEM_Tasks()

#endif /* _SYSTEM_H_ */
