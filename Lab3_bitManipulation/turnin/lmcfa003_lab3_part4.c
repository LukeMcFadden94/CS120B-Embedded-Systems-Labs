/*	Author: Luke McFadden
 *	Lab Section: 023
 *	Assignment: Lab 3  Exercise 4
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
    DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs, initialize to 0s
	unsigned char tempAtoB = 0x00; 
    unsigned char tempAtoC = 0x00; 

    /* Insert your solution below */
    while (1) 
    {
        // PINA7-A4 goes to PORTB3-B0; get PINA upper nibble, shift down 4 positions
    	tempAtoB = ((PINA & 0xF0) >> 4);

        // PINA3-A0 goes to PORTC7-C4; get PINA lower nibble, shift up 4 positions
        tempAtoC = ((PINA & 0x0F) << 4);

      
        // Write to PORTB using tempAtoB
    	PORTB = tempAtoB;

        // Write to PORTC using tempAtoC
        PORTC = tempAtoC;
    }

    return 1;
}
