/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 6  Exercise 1
 *	Exercise Description:
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Link:
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

enum States {Start, cycle} state;

const unsigned char array[3] = {1, 2, 4};
unsigned char tempB = 0x00;
unsigned char counter;

void Tick()
{
	switch (state)	// Transitions begin
	{
		case Start:
			state = cycle;
			break;

		case cycle:
			state = cycle;
			break;

		default:
			state = Start;
			break;
	}				// Transitions end

	switch (state)	// State actions begin
	{
		case Start:
			break;

		case cycle:
			tempB = array[counter];

			if (counter < 2)
				counter++;
			else if (counter == 2)					
				counter = 0;

			break;
	}				// State actions end
}



int main(void) 
{
    DDRB = 0xFF; PORTB = 0x00; // Port B outputs

    state = Start;
    TimerSet(1000);
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
