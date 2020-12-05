/*	Author: Luke McFadden
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab 12  Exercise 1
 *	Exercise Description: 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * Demo Link: https://youtu.be/XrOP8Ue8lhw
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

#define A7 (~PINA & 0x40)   // Move up
#define A6 (~PINA & 0x80)   // Move down

enum MatrixShift {standby, shiftDown, shiftUp, wait};

static unsigned int arr[5] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF};
static unsigned char pattern = 0xFF;        // 0 - off, 1 - on
static unsigned char row = 0xFE;            // 0 - displayed, 1 - not displayed
unsigned char pos = 0;

int shiftRow(int state)
{
    switch (state)  // Transitions begin
    {
        case standby:
            if (!A6 && A7)
                state = shiftDown;
            else if (A6 && !A7)
                state = shiftUp;
            else
                state = standby;
            break;

        case shiftDown:
            state = wait;
            break;

        case shiftUp:
            state = wait;
            break;

        case wait:
            if (A6 || A7)
                state = wait;
            else if (!A6 && !A7)
                state = standby;
            break;

        default:
            state = standby;
    }               // Transitions end

    switch (state)  // State actions begin
    {
        case standby:
            break;

        case shiftDown:
            if (pos < 4)
                pos = pos + 1;
            row = arr[pos];
            break;

        case shiftUp:
            if (pos > 0)
                pos = pos - 1;
            row = arr[pos];
            break;

        case wait:
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

    TimerSet(100);
    TimerOn();


    while (1) 
    {
        state = standby;
        shiftRow(state);

        while (!TimerFlag);
        TimerFlag = 0;
    }

    return 1;
}
