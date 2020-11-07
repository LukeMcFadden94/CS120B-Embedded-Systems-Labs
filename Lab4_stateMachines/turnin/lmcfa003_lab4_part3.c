/*	Author: Luke McFadden
 *	Lab Section: 023
 *	Assignment: Lab 4  Exercise 3
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

enum States {Start, Locked, inBetween_Press1, inputWas_Pound, heldPound_PressY, releasedPound_fromPoundAndY, Unlocked} state;

//volatile unsigned char TimerFlag = 0;
//void TimerISR() {TimerFlag = 1;}

void Tick() 
{           
	switch(state)	// Transitions begin
	{
		case Start:
			state = Locked;	break;

		case Locked:
			if (PINA == 0x04)	// # was pressed
				state = inBetween_Press1;		
			else
				state = Locked; 
			break;

		case inBetween_Press1:
			if (PINA == 0x04)
				state = inBetween_Press1;
			else if (PINA == 0x06)
				state = heldPound_PressY;
			else if (PINA == 0x00)
				state = inputWas_Pound;
			else
				state = Locked;
			break;

		case inputWas_Pound:
			if (PINA == 0x02)
				state = Unlocked;
			else
				state = Locked;
			break;

		case heldPound_PressY:
			if (PINA == 0x06)
				state = heldPound_PressY;
			else if (PINA == 0x02)
				state = releasedPound_fromPoundAndY;
			break;

		case releasedPound_fromPoundAndY:
			if (PINA == 0x02)
				state = releasedPound_fromPoundAndY;
			else if (PINA == 0x00)
				state = Unlocked;
			else
				state = Locked;
			break;

		case Unlocked:
			if (PINA == 0x08)
				state = Locked;
			else
				state = Unlocked;
			break;

		default:
			state = Start; break;
	}	// Transitions end

	switch(state)	// State actions begin
	{
		case Start:
			break;

		case Locked:	// Door starts locked
			PORTB = 0x00;
			PORTC = 0x00;
			break;

		case inBetween_Press1:
			PORTC = 0x01;
			break;

		case inputWas_Pound:
			PORTC = 0x02;
			break;

		case heldPound_PressY:
			PORTC = 0x03;
			break;

		case releasedPound_fromPoundAndY:
			PORTC = 0x04;
			break;

		case Unlocked:	// Door is unlocked
			PORTB = 0x01;
			PORTC = 0x05;
			break;

		default:
			break;
	}	// State actions end
}

int main(void) 
{
    DDRA = 0x00; PORTA = 0xFF;	// PORTA is input
    DDRB = 0xFF; PORTB = 0x00;	// PORTB is output
    DDRC = 0xFF; PORTC = 0x00;	// PORTC is output

    state = Start;
    TimerSet(500);
    TimerOn();

    while (1) 
    {
    	//PINA = 0;
    	//PORTB = 0;
    	Tick();
    	while(!TimerFlag) {}
    		TimerFlag = 0;
    }

    return 1;
}
