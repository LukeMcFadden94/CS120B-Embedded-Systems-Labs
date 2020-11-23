/*	Author: Luke McFadden
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab 9  Exercise 3
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

#define A2 (~PINA & 0x04)

enum ThreeLEDsSM {Start1, led1, led2, led3} SMone;
enum BlinkingLEDSM {Start2, on, off} SMtwo;
enum CombineLEDsSM {Start3, display} SMthree;
enum SpeakerSM {Start4, soundOff, soundOn} SMfour;
unsigned char threeLEDs = 0;
unsigned char blinkingLED = 0;
unsigned short threeLEDCounter = 0;
unsigned short blinkLEDCounter = 0;
unsigned short speakerVar = 0;
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

void Tick4()
{
    switch (SMfour)     // Transitions begin
    {
        case Start4:
            SMfour = soundOff;
            break;

        case soundOff:
            if (A2)
                SMfour = soundOn;
            else
                SMfour = soundOff;
            break;

        case soundOn:
            if (A2)
                SMfour = soundOn;
            else
                SMfour = soundOff;
            break;

        default:
            SMfour = Start4;
            break;
    }                   // Transitions end

    switch (SMfour)     // State actions begin
    {
        case Start4:
            break;

        case soundOff:
            speakerVar = 0;
            break;

        case soundOn:
            speakerVar = 1;
            break;
    }                   // State actions end
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
            if (threeLEDCounter == 3)   // Every 300ms, one of the 3 LEDs in B0 to B2 will change
            {
                Tick1();
                threeLEDCounter = 0;
            }

            if (blinkLEDCounter == 10)  // Every 1000ms (1 second), the 4th LED will turn on or off
            {
                Tick2();
                blinkLEDCounter = 0;
            }

            //Tick4();

            if (threeLEDs == 1)
                output = 0x01;
            else if (threeLEDs == 2) 
                output = 0x02;
            else if (threeLEDs == 3)
                output = 0x04;

            output = output | blinkingLED;

            if (speakerVar == 0)
                output = output & 0x0F;
            else if (speakerVar == 1)
                output = output | 0x10;

            threeLEDCounter++;
            blinkLEDCounter++;
    }                   // State actions end
}

int main(void) 
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    SMone = Start1;
    SMtwo = Start2;
    SMthree = Start3;
    SMfour = Start4;
    TimerSet(100);      // 100ms period
    TimerOn();

    while (1) 
    {
        Tick4();
        Tick3();

        PORTB = output;

        while (!TimerFlag);
        TimerFlag = 0;
    }

    return 1;
}
