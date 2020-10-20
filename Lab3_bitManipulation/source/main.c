/*  Author: Luke McFadden
 *  Lab Section: 023
 *  Assignment: Lab 3  Exercise 5
 *  Exercise Description: [optional - include for your own benefit]
 *
 *  I acknowledge all content contained herein, excluding template or example
 *  code, is my own original work.
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) 
{
    /* Insert DDR and PORT initializations */
    DDRD = 0x00; PORTD = 0xFF; // Configure port D's 8 pins as inputs
    DDRB = 0xFE; PORTB = 0x01; // Configure port B's first pin as input, other 7 pins as outputs
    unsigned short tempD = 0x00;
    unsigned short tempB = 0x00;
    unsigned short weight = 0x00;
    unsigned short outputB = 0x00;
    while (1) 
    {
        // Set tempD to PIND
        tempD = PIND;
        // Set tempB to only have PINB's first binary value
        tempB = PINB & 0x01;
        outputB = 0x00;

       // In binary, PORTD7-0+PORTB0 has 9 binary places, and could
       // look like 0001 1111 1111 at max value (511).
       // PORTD represents 0001 1111 111X, but looks like 0000 1111 1111 currently.
       // PORTB represents XXXX XXXX XXX1.
       // Need to shift PORTD left by 1 place, assign to weight.
       // Add value of PORTB to weight.
     
        weight = tempD << 1;
        
        // If tempB holds a value (can only be 0 or 1), then add 1 to tempD
        if (tempB > 0)
        {
        	weight += 1;
        }
        
        // weight now holds at most 9 place values, and the combination of PIND and PINB

        // If weight >= 70, enable airbag by outputting to PORTB1
        if (weight >= 70)
        {
        	outputB = 0x02;
        }
        // // If weight > 5 AND < 70, disable airbag by outputting to PORTB2
        else if ((weight > 5) && (weight < 70))
        {
        	outputB = outputB | 0x04;
        }
        // If weight < 5, do nothing
        else
        {
        	// Nothing
        }

        // Output onto PORTB
        PORTB = outputB;
        

    }

    return 1;
}
