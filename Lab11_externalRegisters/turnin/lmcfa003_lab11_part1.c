/*	Author: Luke McFadden
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 11 Exercise 1
 *	Exercise Description: 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * Demo Link: https://youtu.be/3jwZhYpn8Go
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

#define D0 (~PIND & 0x01)   // Increment
#define D1 (~PIND & 0x02)   // Decrement

enum buttons {wait, increment, decrement, beforeRelease, reset} state;

unsigned char tempC = 0;

void buttonsSMTick()
{
    switch (state)  // Transitions begin
    {
        case wait:
            if ((D0 && !D1) && tempC < 15)      // Increment if < 15
                state = increment;
            else if ((!D0 && D1) && tempC > 0)  // Decrement if > 0
                state = decrement;
            else if (D0 && D1)                      // Reset to 0
                state = reset;
            break;

        case increment:
            state = beforeRelease;
            break;

        case decrement:
            state = beforeRelease;
            break;

        case beforeRelease:
            if (D0 && D1)           // Reset
                state = reset;
            else if (D0 != D1)      // One button is still pressed
                state = beforeRelease;
            else if (!D0 && !D1)    // No buttons pressed
                state = wait;
            break;

        case reset:
            state = beforeRelease;
            break;

        default:
            state = wait;
    }               // Transitions end

    switch (state)  // State actions begin
    {
        case wait:
            break;

        case increment:
            tempC++;
            break;

        case decrement:
            tempC--;
            break;

        case beforeRelease:
            break;

        case reset:
            tempC = 0;
            break;
    }               // State actions end
}

void transmit_data(unsigned char data) 
{
    int i;
    for (i = 0; i < 8 ; ++i) 
    {
        // Sets SRCLR to 1 allowing data to be set
        // Also clears SRCLK in preparation of sending data
        PORTC = 0x08;
        // set SER = next bit of data to be sent.
        PORTC |= ((data >> i) & 0x01);
        // set SRCLK = 1. Rising edge shifts next bit of data into the shift register
        PORTC |= 0x02;  
    }
    // set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
    PORTC |= 0x04;
    // clears all lines in preparation of a new transmission
    PORTC = 0x00;
}

int main(void) 
{
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0x00; PORTD = 0xFF;

    state = wait;

    TimerSet(100);
    TimerOn();

    while (1) 
    {
        buttonsSMTick();
        transmit_data(tempC);

        while (!TimerFlag);
        TimerFlag = 0;
    }

    return 1;
}
