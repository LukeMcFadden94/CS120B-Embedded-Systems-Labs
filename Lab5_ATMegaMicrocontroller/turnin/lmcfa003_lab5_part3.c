/* Author: Luke McFadden
 * Partner(s) Name (if applicable):  
 * Lab Section: 023
 * Assignment: Lab 5  Exercise 3
 * Exercise Description: 
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: https://youtu.be/dcr0-ErHsQI
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

#define A0 (~PINA & 0x01)

enum States {Start, seq1, wait1, seq2, wait2, seq3, wait3, seq4, wait4} state;
unsigned char tempB = 0x00;

//volatile unsigned char TimerFlag = 0;
//void TimerISR() {TimerFlag = 1;}
//unsigned char counter = 0x00;

void Tick() 
{        

	switch(state)	// Transitions begin
	{
		case Start:
			//tempVal = 0x00;
			state = seq1;	
			break;

		case seq1:
			if (A0) 	
				state = wait1;	
			break;

		case wait1:
			if (!A0)
				state = seq2;
			break;

		case seq2:
			if (A0)		
				state = wait2;		
			break;

		case wait2:
			if (!A0)
				state = seq3;
			break;

		case seq3:
			if (A0)		
				state = wait3;
			break;

		case wait3:
			if (!A0)
				state = seq4;
			break;

		case seq4:
			if (A0)			
				state = wait4;
			break;

		case wait4:
			if (!A0)
				state = seq1;
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

		case seq1:
			tempB = 0x07;	
			break;

		case wait1:		
			break;

		case seq2:		
			tempB = 0x38;
			break;

		case wait2:
			break;

		case seq3:
			tempB = 0x19;
			break;

		case wait3:
			break;

		case seq4:
			tempB = 0x25;
			break;

		case wait4:
			break;

		default:
			break;
	}	// State actions end

	PORTB = tempB;
}

int main(void) 
{
    DDRA = 0x00; PORTA = 0xFF;	// PORTA is input
    DDRB = 0xFF; PORTB = 0x00;	// PORTB is output

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
