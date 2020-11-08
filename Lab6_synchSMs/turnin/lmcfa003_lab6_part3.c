/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 6  Exercise 3
 *	Exercise Description:
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Link: https://youtu.be/2jfOGE1IXTA
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

#define A0 (~PINA & 0x01)
#define A1 (~PINA & 0x02)
#define both (A0 && A1)

enum States {Start, Base, Increment, Decrement, Wait, Reset} state;

unsigned char tempB = 0x07;
unsigned char countPlus = 0;
unsigned char countMinus = 0;

void Tick() 
{          
	switch(state)	// Transitions begin
	{
		case Start:
			state = Base;	
			break;

		case Base:
			if (A0) 			// Increment by 1
				state = Increment;	
			else if (A1) 		// Decrement by 1
				state = Decrement;	
			else if (both) 		// Reset to 0
				state = Reset;
			else
				state = Base; 
			break;

		case Increment:
			if (A0 && !A1)		// Remain	
				state = Wait;	
			else if (both)	// Reset
				state = Reset;
			else
				state = Base;	
			break;

		case Decrement:
			if (!A0 && A1)		// Remain
				state = Wait;
			else if (both)
				state = Reset;		// Reset
			else
				state = Base;
			break;

		case Wait:
			if (countPlus == 10)		// Increment from A0 held
				state = Increment;
			else if (countMinus == 10)// Decrement from A1 held
				state = Decrement;
			else if (A0 || A1)			// Wait
				state = Wait;
			else if (both)				// Reset
				state = Reset;
			else if (!both)
				state = Base;			// Return to base
			break;

		case Reset:
			if (!both)
				state = Base;
			else
				state = Reset;
			break;

		default:
			state = Start; 
			break;
	}	// Transitions end

	switch(state)	// State actions begin
	{
		case Start:
			tempB = 7;	// Start at PORTB = 7
			break;

		case Base:	
			break;

		case Increment:		// Add 1
			countPlus = 0;
			countMinus = 0;

			if (tempB < 9)
				tempB += 1;
			break;

		case Decrement:		// Subtract 1
			countPlus = 0;
			countMinus = 0;

			if (tempB > 0)
				tempB -= 1;
			break;

		case Wait:
			if (A0 && !A1)
				countPlus += 1;
			else if (!A0 && A1)
				countMinus += 1;
			break;

		case Reset:
			tempB = 0;
			break;

		default:
			break;
	}	// State actions end
}


int main(void) 
{
	DDRA = 0x00; PORTA = 0xFF; // PORT A is input
    DDRB = 0xFF; PORTB = 0x00; // Port B s output

    state = Start;
    TimerSet(100);
    TimerOn();

    while (1) 
    {
    	Tick();

    	PORTB = tempB;

    	while (!TimerFlag);
    	TimerFlag = 0;
    }

    return 1;
}
