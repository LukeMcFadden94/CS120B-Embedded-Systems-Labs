/*	Author: Luke McFadden
 *	Lab Section: 023
 *	Assignment: Lab 2  Exercise 3
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
	unsigned char totalWeight = 0x00; // Temporary variable to hold total total weight
	unsigned char weightDiff = 0x00; // Temporary variable to hold weight difference
	
	while(1) 
	{
		// Set temp values to PINA, PINB, and PINC
		tempA = PINA;
		tempB = PINB;
		tempC = PINC;	

		// Assign totalWeight variable the weight combination of all 3 sensors
		totalWeight = tempA + tempB + tempC;
	
		// Assign tempD the total weight, mask off first 2 binary digits
		tempD = totalWeight & 0xFC;

		// If total weight > 140, assign weightDiff the difference between total weight and the max weight capacity
		if (totalWeight > 140)
		{
			weightDiff = totalWeight - 140;
			// Assign tempD7-2 the weightDiff7-2 bits, and make tempD0 1 because weight capacity was exceeded
			tempD = weightDiff & 0xFC;
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
