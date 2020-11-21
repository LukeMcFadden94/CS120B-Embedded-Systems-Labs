/*	Author: Luke McFadden
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab 9  Exercise 1
 *	Exercise Description: 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * Demo Link: https://youtu.be/QxjXkcz6sR8
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

enum ThreeLEDsSM {Start1, led1, led2, led3} SMone;
enum BlinkingLEDSM {Start2, on, off} SMtwo;
enum CombineLEDsSM {Start3, display} SMthree;
unsigned char threeLEDs = 0;
unsigned char blinkingLED = 0;
unsigned char output = 0x00;

void Tick1()
{
    switch (SMone)  // Transitions begin
    {
        case Start1:
            SMone = led1;
            break;

        case led1:
            SMone = led2;
            break;

        case led2:
            SMone = led3;
            break;

        case led3:
            SMone = led1;
            break;

        default:
            SMone = Start1;
            break;
    }               // Transitions end

    switch (SMone)  // State actions begin
    {
        case Start2:
            threeLEDs = 0;
            break;

        case led1:
            threeLEDs = 1;
            break;

        case led2:
            threeLEDs = 2;
            break;

        case led3:
            threeLEDs = 3;
            break;
    }               // State actions end
}

void Tick2()
{
    switch (SMtwo)    // Transitions begin
    {
        case Start2:
            SMtwo = on;
            break;

        case on:
            SMtwo = off;
            break;

        case off:
            SMtwo = on;
            break;

        default:
            SMtwo = Start2;
            break;
    }               // Transitions end

    switch (SMtwo)    // State actions begin
    {
        case Start2:
            break;

        case on:
            blinkingLED = 0x08;
            break;

        case off:
            blinkingLED = 0x00;
            break;        
    }               // State actions end
}

void Tick3() 
{
    switch (SMthree)    // Transitions begin
    {
        case Start3:
            SMthree = display;
            break;

        case display:
            SMthree = display;
            break;

        default:
            SMthree = Start3;
            break;
    }                   // Transitions end

    switch (SMthree)    // State actions begin
    {
        case Start3:
            output = 0x00;
            break;

        case display:
            if (threeLEDs == 1)
                output = 0x01;
            else if (threeLEDs == 2)
                output = 0x02;
            else if (threeLEDs == 3)
                output = 0x04;
            
            output = output | blinkingLED;
    }                   // State actions end
}

int main(void) 
{
    DDRB = 0xFF; PORTB = 0x00;

    SMone = Start1;
    SMtwo = Start2;
    SMthree = Start3;
    TimerSet(1000);
    TimerOn();

    while (1) 
    {
        Tick1();
        Tick2();
        Tick3();

        PORTB = output;

        while (!TimerFlag);
        TimerFlag = 0;
    }

    return 1;
}
