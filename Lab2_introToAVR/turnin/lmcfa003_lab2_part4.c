/*	Author: Luke McFadden
 *	Lab Section: 023
 *	Assignment: Lab 2  Exercise 4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif	

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0xFF; // Configure port B's 8 pins as inputs
	DDRC = 0x00; PORTC = 0xFF; // Configure port C's 8 pins as inputs
	DDRD = 0xFF; PORTD = 0x00; // Configure port D's 8 pins as outputs, initialize to 0s
	unsigned char tempA = 0x00; // Temporary variable to hold the value of A
	unsigned char tempB = 0x00; // Temporary variable to hold the value of B
	unsigned char tempC = 0x00; // Temporary variable to hold the value of C
	unsigned char tempD = 0x00; // Temporary variable to hold the value of D
	unsigned char weightOG = 0x00; // Temporary variable to hold total weight
	unsigned char weight2 = 0x00; // Temporary variable to hold total weight2
	
	while(1) 
	{
		// Set temp values to PINA, PINB, and PINC
		tempA = PINA;
		tempB = PINB;
		tempC = PINC;	

		// Assign weight variable the weight combination of all 3 sensors
		weightOG = tempA + tempB + tempC;
	
		// Mask off first 2 bits of weight variable
		weight2 = weightOG & 0xFC; 

		// Assign tempD7-2 the bits of tempA7-2
		tempD = weight2;

		//If weight exceeds 140kg, set tempD's first bit to 1
		if (weightOG > 140)
		{
			tempD = tempD | 0x01;
		}

		// If weight between seat A and seat C differs greater than 80kg, set 2nd bit of tempD to 1
		if (((tempA - tempC) > 80) || ((tempC - tempA) > 80))
		{
			tempD = tempD | 0x02;
		}

		// Write to PORTD
		PORTD = tempD;
	}
	
	return 0;
}
