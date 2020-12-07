/*	Author: Luke McFadden
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab 12  Exercise 3
 *	Exercise Description: 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * Demo Link: https://youtu.be/ciIgigFEBpM
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

#define A7 (~PINA & 0x40)   // Move up
#define A6 (~PINA & 0x80)   // Move down

enum MatrixShift {loop};

static unsigned char patternArr[5] = {0x00, 0x3C, 0x24, 0x3C, 0x00};       
static unsigned char rowArr[5] = {0xFF, 0xFD, 0xFB, 0xF7, 0xFF};         
unsigned char pattern = 0x00;        // 0 - off, 1 - on
unsigned char row = 0xFF;            // 0 - displayed, 1 - not displayed
unsigned char pos = 0;

int shiftRow(int state)
{
    switch (state)  // Transitions begin
    {
        case loop:
            state = loop;
            break;

        default:
            state = loop;
    }               // Transitions end

    switch (state)  // State actions begin
    {
        case loop:
            if (pos < 4)
                pos++;          
            else if (pos == 4)           
                pos = 0;
            
            pattern = patternArr[pos];
            row = rowArr[pos];
            break;
    }               // State actions end

    PORTC = pattern;
    PORTD = row;
    return state;
}

int main(void) 
{
    int state = 0;

    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;

    TimerSet(1);
    TimerOn();


    while (1) 
    {
        state = loop;
        shiftRow(state);

        while (!TimerFlag);
        TimerFlag = 0;
    }

    return 1;
}
