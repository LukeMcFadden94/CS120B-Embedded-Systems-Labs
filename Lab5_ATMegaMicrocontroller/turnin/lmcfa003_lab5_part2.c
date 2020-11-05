/* Author: Luke McFadden
 * Partner(s) Name (if applicable):  
 * Lab Section: 023
 * Assignment: Lab 5  Exercise 2
 * Exercise Description: 
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: https://youtu.be/_jZv2vtJQrg
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

#define A0 (~PINA & 0x01)
#define A1 (~PINA & 0x02)

enum States {Start, Base, Increment, Decrement, Wait, Reset} state;
unsigned char tempVal = 0x00;

//volatile unsigned char TimerFlag = 0;
//void TimerISR() {TimerFlag = 1;}
//unsigned char counter = 0x00;

void Tick() 
{        

	switch(state)	// Transitions begin
	{
		case Start:
			tempVal = 0x07;
			state = Base;	
			break;

		case Base:
			if (A0) 			// Increment by 1
				state = Increment;	
			else if (A1) 	// Decrement by 1
				state = Decrement;		
			else if (A0 && A1) 		// Reset to 0
				state = Reset;
			else
				state = Base; 
			break;

		case Increment:
			//if (A0)		// Remain	
				state = Wait;	
			// else if (A0 && A1)	// Reset
			// 	state = Reset;
			// else
			// 	state = Base;	
			break;

		case Decrement:
			//if (A1)		// Remain
				state = Wait;
			//else if (A0 && A1)
			//	state = Reset;		// Reset
			// else
			// 	state = Base;
			break;

		case Wait:
			if (!A0 && !A1)			// Return to base
				state = Base;
			else if (A0 && A1)
				state = Reset;		// Reset
			else
				state = Wait;		// Wait
			break;

		case Reset:
			if (!A0 && !A1)
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
			//tempVal = 0x07;	// Start at PORTC = 7
			break;

		case Base:	
			break;

		case Increment:		// Add 1
			if (tempVal < 9)
				tempVal += 1;
			break;

		case Decrement:		// Subtract 1
			if (tempVal > 0)
			 	tempVal -= 1;
			break;

		case Wait:
			break;

		case Reset:
			tempVal = 0;
			break;

		default:
			break;
	}	// State actions end

	PORTC = tempVal;
}

int main(void) 
{
    DDRA = 0x00; PORTA = 0xFF;	// PORTA is input
    DDRC = 0xFF; PORTC = 0x00;	// PORTC is output

    state = Start;
    TimerSet(500);
    TimerOn();

    while (1) 
    {
    	Tick();

    	//PORTC = tempVal;
    	//tempA = ~PINA & 0x0F;

    	while(!TimerFlag) {}
    		TimerFlag = 0;
    		//PORTC = counter;
    }

    return 1;
}
