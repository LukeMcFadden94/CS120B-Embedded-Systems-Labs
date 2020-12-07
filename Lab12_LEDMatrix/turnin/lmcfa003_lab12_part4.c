/*	Author: Luke McFadden
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab 12  Exercise 4
 *	Exercise Description: 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * Demo Link:
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

#define A7 (~PINA & 0x80)   // Move up
#define A6 (~PINA & 0x40)   // Move down
#define A5 (~PINA & 0x20)   // Move left
#define A4 (~PINA & 0x10)   // Move right

enum MatrixShift {display};
enum Buttons {standby, up, down, left, right, wait} bState;

static unsigned char patternArr[5] = {0x00, 0x3C, 0x24, 0x3C, 0x00};       
static unsigned char rowArr[5] = {0xFF, 0xFD, 0xFB, 0xF7, 0xFF};         
unsigned char pattern = 0x00;        // 0 - off, 1 - on
unsigned char row = 0xFF;            // 0 - displayed, 1 - not displayed
unsigned char pos = 0;
unsigned char horizontal = 2;               // Goes 0 through 4, adjusts pattern
unsigned char vertical = 1;                 // Goes 0 through 2, adjusts row

int shiftRow(int state)
{
    switch (state)  // Transitions begin
    {
        case display:
            state = display;
            break;


        default:
            state = display;
    }               // Transitions end

    switch (state)  // State actions begin
    {
        case display:
            if (pos < 4)
                pos++;          
            else if (pos == 4)           
                pos = 0;
            
            //pattern = patternArr[pos];
            row = rowArr[pos];

            if (horizontal == 0)           
                pattern = patternArr[pos] << 2;           
            else if (horizontal == 1)          
                pattern = patternArr[pos] << 1;           
            else if (horizontal == 2)  // Do nothing           
                pattern = patternArr[pos];        
            else if (horizontal == 3)            
                pattern = patternArr[pos] >> 1;           
            else if (horizontal == 4)           
                pattern = patternArr[pos] >> 2;
            
            //if (vertical == 0)

              
            break;
    }               // State actions end

    PORTC = pattern;
    PORTD = row;
    return state;
}

// A7 - Move up
// A6 - Move down
// A5 - Move left
// A4 - Move right

int buttonFunc(int bState)
{
    switch (bState)
    {
        case standby:
            // if (A7 && !A6 && !A5 && !A4)
            //     bState = up;
            // else if (!A7 && A6 && !A5 && !A4)
            //     bState = down;
            // else if (!A7 && !A6 && A5 && !A4)
            //     bState = left;
            // else if (!A7 && !A6 && !A5 && A4)
            //     bState = right;
            if (A7)
                bState = up;
            else if (A6)
                bState = down;
            else if (A5)
                bState = left;
            else if (A4)
                bState = right;
            else
                bState = standby;

            case up:
                bState = wait;
                break;

            case down:
                bState = wait;
                break;
                
            case left:
                bState = wait;
                break;

            case right:
                bState = wait;
                break;   

            case wait:
                if (A7 || A6 || A5 || A4)
                    bState = wait;
                else if (!A7 && !A6 && !A5 && !A4)
                    bState = standby;
                break;

            default:
                bState = standby;
    }

    switch (bState)
    {
        case standby:
            break;

        case up:
            if (vertical < 2)
                vertical = vertical + 1;
            break;

        case down:
            if (vertical > 0)
                vertical = vertical - 1;
            break;

        case left:
            if (horizontal > 0)
                horizontal = horizontal - 1;
            break;

        case right:
            if (horizontal < 4)
                horizontal = horizontal + 1;
            break;

        case wait:
            break;
    }

    return bState;
}

int main(void) 
{
    int state = 0;
    int bState = 0;

    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;

    TimerSet(200);
    TimerOn();

    state = display;
    bState = standby;

    while (1) 
    {
        //state = display;
        //bState = standby;
        shiftRow(state);
        buttonFunc(bState);

        while (!TimerFlag);
        TimerFlag = 0;
    }

    return 1;
}
