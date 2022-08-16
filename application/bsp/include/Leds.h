#ifndef _LEDS_H_
#define	_LEDS_H_

#include <stdbool.h>

#ifdef	__cplusplus
extern "C" {
#endif

/** Type defintions *********************************/
typedef enum
{
    LED_NONE,
    LED_D1,
    LED_D2,
    LED_D3,
    LED_D4
//      D7 = Bus powered - hard wired to power supply
//      D8 = Self powered - hard wired to power supply
} LED;

#define LED_COUNT 4

void LED_On(LED led);

void LED_Off(LED led);

void LED_Toggle(LED led);

bool LED_Get(LED led);

void LED_Enable(LED led);

#ifdef	__cplusplus
}
#endif

#endif	/* _LEDS_H_ */

