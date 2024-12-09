// Author: Kenjie Aaron Gozali and Rohit Jathniel Vasandani
// Net ID:
// Date: 14/11/2024
// Assignment: Lab 4
//

// Description: This file implements the initialization of an external
// switch.
//----------------------------------------------------------------------//

#include "switch.h"
#include <avr/io.h>

/*
 * Initializes pull-up resistor on D0    and sets it into input mode
 */
void initSwitchPD2()
{
    // pull up resistor on D0 set it into input mode
    DDRE &= ~(1 << DDE5);
    PORTE |= (1 << PORTE5);

    // to trigger the external interrupt switch by clearing put isc01 to 1 and ics00 to 0 so that the switch can detect an input/interrupt when the
    // switch is going from high to low
    EICRA |= (1 << ISC51);  // Set ISC51 to 1
    EICRA &= ~(1 << ISC50); // Clear ISC50 to 0

    // this below enables the int0 for the external intterupt int0
    EIMSK |= (1 << INT5);
}
