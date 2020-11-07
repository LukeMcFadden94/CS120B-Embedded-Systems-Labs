/*	Author: Luke McFadden
 *	Lab Section: 023
 *	Assignment: Lab 4  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

enum States {Start, Base, Increment, Decrement, Wait, Reset} state;

//volatile unsigned char TimerFlag = 0;
//void TimerISR() {TimerFlag = 1;}
//unsigned char counter = 0x00;

void Tick() 
{          
	switch(state)	// Transitions begin
	{
		case Start:
			//counter = 0x07;
			state = Base;	
			break;

		case Base:
			if (PINA == 0x01) {			// Increment by 1
				state = Increment;	
				if (PORTC < 0x09)
					PORTC = PORTC + 1;
			}
			else if (PINA == 0x02) {	// Decrement by 1
				state = Decrement;	
				if (PORTC > 0x00)
					PORTC = PORTC - 1;
			}
			else if (PINA == 0x03) 		// Reset to 0
				state = Reset;
			else
				state = Base; 
			break;

		case Increment:
			if (PINA == 0x01)		// Remain	
				state = Wait;	
			else if (PINA == 0x03)	// Reset
				state = Reset;
			else
				state = Base;	
			break;

		case Decrement:
			if (PINA == 0x02)		// Remain
				state = Wait;
			else if (PINA == 0x03)
				state = Reset;		// Reset
			else
				state = Base;
			break;

		case Wait:
			if (PINA == 0x03)		// Reset
				state = Reset;
			else if (PINA == 0x00)
				state = Base;		// Return to base
			else
				state = Wait;		// Wait
			break;

		case Reset:
			if (PINA == 0x00)
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
			PORTC = 0x07;	// Start at PORTC = 7
			break;

		case Base:	
			break;

		case Increment:		// Add 1
			// if (PORTC < 9)
			// 	PORTC += 1;
			break;

		case Decrement:		// Subtract 1
			// if (PORTC > 0)
			//  	PORTC -= 1;
			break;

		case Wait:
			break;

		case Reset:
			PORTC = 0x00;
			break;

		default:
			break;
	}	// State actions end
}

int main(void) 
{
    DDRA = 0x00; PORTA = 0xFF;	// PORTA is input
    //DDRB = 0xFF; PORTB = 0x00;	// PORTB is output
    DDRC = 0xFF; PORTC = 0x00;	// PORTC is output

    state = Start;
    TimerSet(500);
    TimerOn();

    while (1) 
    {
    	Tick();
    	while(!TimerFlag) {}
    		TimerFlag = 0;
    		//PORTC = counter;
    }

    return 1;
}
