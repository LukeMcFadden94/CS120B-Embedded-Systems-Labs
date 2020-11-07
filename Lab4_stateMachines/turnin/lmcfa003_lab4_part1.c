/*	Author: Luke McFadden
 *	Lab Section: 023
 *	Assignment: Lab 4  Exercise 1
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

enum States {Start, L0on_L1off, inBetween_Press1, L0off_L1on, inBetween_Press2} state;

//volatile unsigned char TimerFlag = 0;
//void TimerISR() {TimerFlag = 1;}

void Tick() 
{           
	switch(state)	// Transitions begin
	{
		case Start:
			state = L0on_L1off;	break;

		case L0on_L1off:
			if(PINA == 1)
				state = inBetween_Press1;		
			else
				state = L0on_L1off; 
			break;

		case inBetween_Press1:
			if(PINA == 1)
				state = inBetween_Press1;
			else
				state = L0off_L1on;
			break;

		case L0off_L1on:
			if (PINA == 1)
				state = inBetween_Press2;
			else
				state = L0off_L1on;
			break;

		case inBetween_Press2:
			if (PINA == 1)
				state = inBetween_Press2;
			else
				state = L0on_L1off;
			break;

		default:
			state = Start; break;
	}	// Transitions end

	switch(state)	// State actions begin
	{
		case Start:
			break;

		case L0on_L1off:
			PORTB = 0x01;
			//B0 = 1;
			//B1 = 0; 
			break;

		case inBetween_Press1:
			PORTB = 0x02;
			break;

		case L0off_L1on:
			//PORTB = 0x02;
			//B0 = 0;
			//B1 = 1; 
			break;

		case inBetween_Press2:
			PORTB = 0x01;
			break;

		default:
			break;
	}	// State actions end
}

int main(void) 
{
    DDRA = 0x00; PORTA = 0xFF;	// PORTA is input
    DDRB = 0xFF; PORTB = 0x00;	// PORTB is output
  	//unsigned short tempA = 0x00;
  	//unsigned short tempB = 0x00;

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
