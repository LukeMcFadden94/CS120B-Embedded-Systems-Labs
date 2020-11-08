/*	Author: Luke McFadden
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab 6  Exercise 2
 *	Exercise Description:
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Link: https://youtu.be/Q85uejm9Njw
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

#define A0 (~PINA & 0x01)
#define max (counter == 2)
#define min (counter == 0)

enum States {Start, cycleUp, cycleDown, wait, stop} state;

const unsigned char array[3] = {1, 2, 4};
unsigned char tempB = 0x00;
unsigned char counter = 0;

void Tick()
{
	switch (state)	// Transitions begin
	{
		case Start:
			state = cycleUp;
			break;

		case cycleUp:		// Increment, LED order goes 0 > 1 > 2
			if (A0)
				state = wait;
			else if (!A0)
			{		
				if (max)
					state = cycleDown;
				else
					state = cycleUp;
			}
			break;


		case cycleDown:		// Decrement, LED order goes 2 > 1 > 0
			if (A0)
				state = wait;
			else if (!A0)
			{
				if (min)
					state = cycleUp;
				else
					state = cycleDown;
			}
			break;

		case wait:
			if (A0)
				state = wait;
			else if (!A0)
				state = stop;
			break;

		case stop:
			if (A0)
				state = Start;
			else if (!A0)
				state = stop;
			break;

		default:
			state = Start;
			break;
	}				// Transitions end

	switch (state)	// State actions begin
	{
		case Start:
			tempB = 1;
			counter = 1;
			break;

		case cycleUp:		// Increment, LED order goes 0 > 1 > 2
			tempB = array[counter];
			if (counter < 2)
				counter++;		
			break;

		case cycleDown:		// Decrement, LED order goes 2 > 1 > 0
			tempB = array[counter];

			if (counter > 0)
				counter--;
			break;	

		case wait:
			break;

		case stop:
			break;
	}				// State actions end
}



int main(void) 
{
	DDRA = 0x00; PORTA = 0xFF; // PORT A is input
    DDRB = 0xFF; PORTB = 0x00; // Port B s output

    state = Start;
    TimerSet(300);
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
