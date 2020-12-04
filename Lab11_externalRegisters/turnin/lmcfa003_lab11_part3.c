/*	Author: Luke McFadden
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 11 Exercise 3
 *	Exercise Description: 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * Demo Link: https://youtu.be/NBKYnzAeu7M
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

#define D0 (~PIND & 0x01)   // Increment for reg1
#define D1 (~PIND & 0x02)   // Decrement for reg1

#define D2 (~PIND & 0x04)   // Increment for reg2
#define D3 (~PIND & 0x08)   // Decrement for reg2

enum buttons {display1, display2, display3, off} buttonsSM;
enum buttons2 {display4, display5, display6, off2} buttonsSMTwo;
enum led0 {nothing} ledZeroSM;
enum led1 {a1, a2, a3, a4, a5} ledOneSM;
enum led2 {b1, b2, b3, b4, b5, b6, b7, b8, b9} ledTwoSM;
enum led3 {c1, c2, c3, c4, c5, c6} ledThreeSM;
enum led0_2 {nothing2} ledZeroTwoSM;
enum led1_2 {a12, a22, a32, a42, a52} ledOneTwoSM;
enum led2_2 {b12, b22, b32, b42, b52, b62, b72, b82, b92} ledTwoTwoSM;
enum led3_2 {c12, c22, c32, c42, c52, c62} ledThreeTwoSM;


unsigned char go = 1;
unsigned char go2 = 1;
unsigned char tempC = 0;
unsigned char tempC2 = 0;

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

///////////////// Reg 2 starts

void led0_2SMTick()
{
    switch (ledZeroTwoSM)
    {
        case nothing2:
            ledZeroTwoSM = nothing2;
            break;

        default:
            ledZeroTwoSM = nothing2;
    }

    switch (ledZeroTwoSM)
    {
        case nothing2:
            tempC2 = 0x00;
            break;
    }
}

void led1_2SMTick()
{
    switch (ledOneTwoSM)   // Transitions begin
    {
        case a12:
            ledOneTwoSM = a22;
            break;

        case a22:
            ledOneTwoSM = a32;
            break;

        case a32:
            ledOneTwoSM = a42;
            break;

        case a42:
            ledOneTwoSM = a52;
            break;

        case a52:
            ledOneTwoSM = a12;
            break;

        default:
            ledOneTwoSM = a12;
    }                   // Transitions end

    switch (ledOneTwoSM)   // State actions begin
    {
        case a12:
            tempC2 = 0x18;
            break;

        case a22:
            tempC2 = 0x3C;
            break;

        case a32:
            tempC2 = 0x7E;
            break;

        case a42:
            tempC2 = 0xFF;
            break;

        case a52:
            tempC2 = 0x00;
            break;
    }                   // State actions end
}

void led2_2SMTick()
{
    switch (ledTwoTwoSM)   // Transitions begin
    {
        case b12:
            ledTwoTwoSM = b22;
            break;

        case b22:
            ledTwoTwoSM = b32;
            break;

        case b32:
            ledTwoTwoSM = b42;
            break;

        case b42:
            ledTwoTwoSM = b52;
            break;

        case b52:
            ledTwoTwoSM = b62;
            break;

        case b62:
            ledTwoTwoSM = b72;
            break;

        case b72:
            ledTwoTwoSM = b82;
            break;

        case b82:
            ledTwoTwoSM = b92;
            break;

        case b92:
            ledTwoTwoSM = b12;
            break;

        default:
            ledTwoTwoSM = b12;
    }                   // Transitions end

    switch (ledTwoTwoSM)   // State actions begin
    {
        case b12:
            tempC2 = 0x80;
            break;

        case b22:
            tempC2 = 0x81;
            break;

        case b32:
            tempC2 = 0xC1;
            break;

        case b42:
            tempC2 = 0xC3;
            break;

        case b52:
            tempC2 = 0xE3;
            break;

        case b62:
            tempC2 = 0xE7;
            break;

        case b72:
            tempC2 = 0xF7;
            break;

        case b82:
            tempC2 = 0xFF;
            break;

        case b92:
            tempC2 = 0x00;
            break;
    }                   // State actions end
}

void led3_2SMTick()
{
    switch (ledThreeTwoSM)   // Transitions begin
    {
        case c12:
            ledThreeTwoSM = c22;
            break;

        case c22:
            ledThreeTwoSM = c32;
            break;

        case c32:
            ledThreeTwoSM = c42;
            break;

        case c42:
            ledThreeTwoSM = c52;
            break;

        case c52:
            ledThreeTwoSM = c62;
            break;

        case c62:
            ledThreeTwoSM = c12;
            break;

        default:
            ledThreeTwoSM = c12;
    }                   // Transitions end

    switch (ledThreeTwoSM)   // State actions begin
    {
        case c12:
            tempC2 = 0x99;
            break;

        case c22:
            tempC2 = 0x00;
            break;

        case c32:
            tempC2 = 0x66;
            break;

        case c42:
            tempC2 = 0x00;
            break;

        case c52:
            tempC2 = 0x5A;
            break;

        case c62:
            tempC2 = 0x00;
            break;
    }                   // State actions end
}

/////////////////// Reg 2 ends

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

void buttonsSMTick2()
{
    switch (buttonsSMTwo)  // Transitions begin
    {
        case display4:
            if (!D2 && !D3)         // No change
                buttonsSMTwo = display4;
            else if (D2 && !D3)     // Increment
                buttonsSMTwo = display5;
            else if (!D2 && D3)     // Decrement
                buttonsSMTwo = display6;
            else if (D2 && D3)      // Turn off
                buttonsSMTwo = off2;
            break;

        case display5:
            if (!D2 && !D3)         // No change
                buttonsSMTwo = display5;
            else if (D2 && !D3)     // Increment
                buttonsSMTwo = display6;
            else if (!D2 && D3)     // Decrement
                buttonsSMTwo = display4;
            else if (D2 && D3)      // Turn off
                buttonsSMTwo = off2;
            break;

        case display6:
            if (!D2 && !D3)         // No change
                buttonsSMTwo = display6;
            else if (D2 && !D3)     // Increment
                buttonsSMTwo = display4;
            else if (!D2 && D3)     // Decrement
                buttonsSMTwo = display5;
            else if (D2 && D3)      // Turn off
                buttonsSMTwo = off2;
            break;

        case off2:
            if (D2 && D3)           // Turn on
                buttonsSMTwo = display4;
            else
                buttonsSMTwo = off2;
            break;

        default:
            buttonsSMTwo = display4;
    }               // Transitions end

    switch (buttonsSMTwo)  // State actions begin
    {
        case display4:
            //go = 1; 
            led1_2SMTick();
            break;

        case display5:
            //go = 2;
            led2_2SMTick();
            break;

        case display6:
            //go = 3;
            led3_2SMTick();
            break;

        case off2:
            //go = 0;
            led0_2SMTick();
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

void transmit_data2(unsigned char data) 
{
    int i;
    for (i = 0; i < 8 ; ++i) 
    {
        // Sets SRCLR to 1 allowing data to be set
        // Also clears SRCLK in preparation of sending data
        PORTA = 0x08;
        // set SER = next bit of data to be sent.
        PORTA |= ((data >> i) & 0x01);
        // set SRCLK = 1. Rising edge shifts next bit of data into the shift register
        PORTA |= 0x02;  
    }
    // set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
    PORTA |= 0x04;
    // clears all lines in preparation of a new transmission
    PORTA = 0x00;
}

int main(void) 
{
    DDRA = 0xFF; PORTA = 0x00;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0x00; PORTD = 0xFF;

    buttonsSM = display1;
    buttonsSMTwo = display4;
    ledZeroSM = nothing;
    ledOneSM = a1;
    ledTwoSM = b1;
    ledThreeSM = c1;
    ledZeroTwoSM = nothing2;
    ledOneTwoSM = a12;
    ledTwoTwoSM = b12;
    ledThreeTwoSM = c12;

    TimerSet(200);
    TimerOn();

    while (1) 
    {
        buttonsSMTick();
        buttonsSMTick2();
        transmit_data(tempC);
        transmit_data2(tempC2);


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
