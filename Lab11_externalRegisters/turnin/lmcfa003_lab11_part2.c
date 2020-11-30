/*	Author: Luke McFadden
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 11 Exercise 2
 *	Exercise Description: 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * Demo Link: https://youtu.be/UimC3f4LTZU
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

#define D0 (~PIND & 0x01)   // Increment
#define D1 (~PIND & 0x02)   // Decrement

enum buttons {display1, display2, display3, off} buttonsSM;
enum led0 {nothing} ledZeroSM;
enum led1 {a1, a2, a3, a4, a5} ledOneSM;
enum led2 {b1, b2, b3, b4, b5, b6, b7, b8, b9} ledTwoSM;
enum led3 {c1, c2, c3, c4, c5, c6} ledThreeSM;

unsigned char go = 1;
unsigned char tempC = 0;

void buttonsSMTick()
{
    switch (buttonsSM)  // Transitions begin
    {
        case display1:
            if (!D0 && !D1)         // No change
                buttonsSM = display1;
            else if (D0 && !D1)     // Increment
                buttonsSM = display2;
            else if (!D0 && D1)     // Decrement
                buttonsSM = display3;
            else if (D0 && D1)      // Turn off
                buttonsSM = off;
            break;

        case display2:
            if (!D0 && !D1)         // No change
                buttonsSM = display2;
            else if (D0 && !D1)     // Increment
                buttonsSM = display3;
            else if (!D0 && D1)     // Decrement
                buttonsSM = display1;
            else if (D0 && D1)      // Turn off
                buttonsSM = off;
            break;

        case display3:
            if (!D0 && !D1)         // No change
                buttonsSM = display3;
            else if (D0 && !D1)     // Increment
                buttonsSM = display1;
            else if (!D0 && D1)     // Decrement
                buttonsSM = display2;
            else if (D0 && D1)      // Turn off
                buttonsSM = off;
            break;

        case off:
            if (D0 && D1)           // Turn on
                buttonsSM = display1;
            else
                buttonsSM = off;
            break;

        default:
            buttonsSM = display1;
    }               // Transitions end

    switch (buttonsSM)  // State actions begin
    {
        case display1:
            //go = 1; 
            led1SMTick();
            break;

        case display2:
            //go = 2;
            led2SMTick();
            break;

        case display3:
            //go = 3;
            led3SMTick();
            break;

        case off:
            //go = 0;
            led0SMTick();
            break;
    }               // State actions end
}

void led0SMTick()
{
    switch (ledZeroSM)
    {
        case nothing:
            ledZeroSM = nothing;
            break;

        default:
            ledZeroSM = nothing;
    }

    switch (ledZeroSM)
    {
        case nothing:
            tempC = 0x00;
            break;
    }
}

void led1SMTick()
{
    switch (ledOneSM)   // Transitions begin
    {
        case a1:
            ledOneSM = a2;
            break;

        case a2:
            ledOneSM = a3;
            break;

        case a3:
            ledOneSM = a4;
            break;

        case a4:
            ledOneSM = a5;
            break;

        case a5:
            ledOneSM = a1;
            break;

        default:
            ledOneSM = a1;
    }                   // Transitions end

    switch (ledOneSM)   // State actions begin
    {
        case a1:
            tempC = 0x18;
            break;

        case a2:
            tempC = 0x3C;
            break;

        case a3:
            tempC = 0x7E;
            break;

        case a4:
            tempC = 0xFF;
            break;

        case a5:
            tempC = 0x00;
            break;
    }                   // State actions end
}

void led2SMTick()
{
    switch (ledTwoSM)   // Transitions begin
    {
        case b1:
            ledTwoSM = b2;
            break;

        case b2:
            ledTwoSM = b3;
            break;

        case b3:
            ledTwoSM = b4;
            break;

        case b4:
            ledTwoSM = b5;
            break;

        case b5:
            ledTwoSM = b6;
            break;

        case b6:
            ledTwoSM = b7;
            break;

        case b7:
            ledTwoSM = b8;
            break;

        case b8:
            ledTwoSM = b9;
            break;

        case b9:
            ledTwoSM = b1;
            break;

        default:
            ledTwoSM = b1;
    }                   // Transitions end

    switch (ledTwoSM)   // State actions begin
    {
        case b1:
            tempC = 0x80;
            break;

        case b2:
            tempC = 0x81;
            break;

        case b3:
            tempC = 0xC1;
            break;

        case b4:
            tempC = 0xC3;
            break;

        case b5:
            tempC = 0xE3;
            break;

        case b6:
            tempC = 0xE7;
            break;

        case b7:
            tempC = 0xF7;
            break;

        case b8:
            tempC = 0xFF;
            break;

        case b9:
            tempC = 0x00;
            break;
    }                   // State actions end
}

void led3SMTick()
{
    switch (ledThreeSM)   // Transitions begin
    {
        case c1:
            ledThreeSM = c2;
            break;

        case c2:
            ledThreeSM = c3;
            break;

        case c3:
            ledThreeSM = c4;
            break;

        case c4:
            ledThreeSM = c5;
            break;

        case c5:
            ledThreeSM = c6;
            break;

        case c6:
            ledThreeSM = c1;
            break;

        default:
            ledThreeSM = c1;
    }                   // Transitions end

    switch (ledThreeSM)   // State actions begin
    {
        case c1:
            tempC = 0x99;
            break;

        case c2:
            tempC = 0x00;
            break;

        case c3:
            tempC = 0x66;
            break;

        case c4:
            tempC = 0x00;
            break;

        case c5:
            tempC = 0x5A;
            break;

        case c6:
            tempC = 0x00;
            break;
    }                   // State actions end
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

    buttonsSM = display1;
    ledZeroSM = nothing;
    ledOneSM = a1;
    ledTwoSM = b1;
    ledThreeSM = c1;

    TimerSet(200);
    TimerOn();

    while (1) 
    {
        buttonsSMTick();
        transmit_data(tempC);

        // if (go == 0)
        //     led0SMTick();
        // else if (go == 1)
        //     led1SMTick();
        // else if (go == 2)
        //     led2SMTick();
        // else if (go == 3)
        //     led3SMTick();

        while (!TimerFlag);
        TimerFlag = 0;
    }

    return 1;
}
