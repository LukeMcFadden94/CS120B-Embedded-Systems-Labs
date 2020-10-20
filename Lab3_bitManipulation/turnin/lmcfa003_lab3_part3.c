/*	Author: Luke McFadden
 *	Lab Section: 023
 *	Assignment: Lab 3  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) 
{
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs, initialize to 0s
	unsigned char tempA = 0x00;
	unsigned char tempA_2 = 0x00;

    /* Insert your solution below */
    while (1) 
    {
    	// Set temp variable A to PIN A3-A0
    	tempA = PINA & 0x0F;
        // Set temp variable A_2 to PIN A6-A4
        tempA_2 = PINA & 0x70;

        // For tempA
        // Fuel is 0 - PORTC6 lit
        if (tempA == 0)
        {
            tempA = 0x40;
        }
        // Fuel is 1 or 2 - PORTC5 + C6 lit
    	else if ((tempA == 1) || (tempA == 2))
        {
            tempA = 0x60;
        }
        // Fuel is 3 or 4 - PORTC4 + C5 + C6 lit
        else if ((tempA == 3) || (tempA == 4))
        {
           tempA = 0x70;
        }
        // Fuel is 5 or 6 - PORTC3 + C4 + C5 lit
        else if ((tempA == 5) || (tempA == 6))
        {
           tempA = 0x38;
        }
        // Fuel is 7, 8 or 9 - PORTC2 + C3 + C4 + C5 lit
        else if ((tempA == 7) || (tempA == 8) || (tempA == 9))
        {
           tempA = 0x3C;
        }
        // Fuel is 10, 11 or 12 - PORTC1 + C2 + C3 + C4 + C5 lit
        else if ((tempA == 10) || (tempA == 11) || (tempA == 12))
        {
           tempA = 0x3E;
        }
        // Fuel is 13, 14 or 15 - PORTC0 + C1 + C2 + C3 + C4 + C5 lit
        else if ((tempA == 13) || (tempA == 14) || (tempA == 15))
        {
           tempA = 0x3F;
        }
        // Error case
        else
        {
            printf("Error!\n");
        }


        // For tempA_2
        // Key in ignition AND driver is seated, but no seatbelt fastened
        if (tempA_2 == 0x30)
        {
            // Modify tempA and turn PORTC7 light on
            tempA = tempA | 0x80;
        }
    	
        // Write to PORTC 
    	PORTC = tempA;
    }

    return 1;
}
