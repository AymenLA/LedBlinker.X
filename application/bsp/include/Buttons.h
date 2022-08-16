#ifndef _BUTTONS_H_
#define	_BUTTONS_H_

#include <stdbool.h>

#ifdef	__cplusplus
extern "C" {
#endif
    
/*** Button Definitions *********************************************/
typedef enum
{
    BUTTON_NONE,
    BUTTON_S2,
    BUTTON_S3
    //S1 is MCLR
} BUTTON;

bool BUTTON_IsPressed(BUTTON button);

void BUTTON_Enable(BUTTON button);

#ifdef	__cplusplus
}
#endif

#endif	/* _BUTTONS_H_ */

