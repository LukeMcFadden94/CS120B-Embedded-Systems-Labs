/*  Author: Luke McFadden
 *  Lab Section: 023
 *  Assignment: Lab 3  Exercise 1
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
    DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
    DDRB = 0x00; PORTB = 0xFF; // Configure port B's 8 pins as inputs
    DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
    unsigned char tempA = 0x00;
    unsigned char tempB = 0x00;
    unsigned char counter = 0x00;


    /* Insert your solution below */
    while (1) 
    {
        // Set temp variables to PINs A and B
        tempA = PINA;
        tempB = PINB;

        // Count and add amount of 1's within PINs A and B
        counter = ((tempA & 0x80) >> 7) + ((tempA & 0x40) >> 6) + ((tempA & 0x20) >> 5) + ((tempA & 0x10) >> 4) + ((tempA & 0x08) >> 3) + ((tempA & 0x04) >> 2) + ((tempA & 0x02) >> 1) + (tempA & 0x01); 
        counter = counter + ((tempB & 0x80) >> 7) + ((tempB & 0x40) >> 6) + ((tempB & 0x20) >> 5) + ((tempB & 0x10) >> 4) + ((tempB & 0x08) >> 3) + ((tempB & 0x04) >> 2) + ((tempB & 0x02) >> 1) + (tempB & 0x01); 
        
        // Give PORT C total amount of 1's
        PORTC = counter;
    }

    return 1;
}
