/*	Author: Luke McFadden
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 13  Exercise 1
 *	Exercise Description:
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * Demo Link: https://youtu.be/RI3a3a2uT3Y
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include "scheduler.h"
#endif

void A2D_init()     
    {
      ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
    // ADEN: Enables analog-to-digital conversion
    // ADSC: Starts analog-to-digital conversion
    // ADATE: Enables auto-triggering, allowing for constant
    //      analog to digital conversions.
    }

unsigned short input;
unsigned short temp = 0x00;

enum Joystick {display};
int JoystickSM(int state)
{
    switch (state)  // Transitions begin
    {
        case display:
            state = display;
            break;

        default:
            state = display;
            break;
    }               // Transitions end

    switch (state)  // State actions begin
    {
        case display:
            input = ADC;
            PORTC = (input >> 2);
            PORTD = ((input & 0x03) << 6);
            break;
    }               // State actions end

    return state;
}

int main(void) 
{
    DDRA = 0x00; PORTA = 0xFF;  // Joystick input
    DDRC = 0xFF; PORTC = 0x00;  // LED Array #1-8 output
    DDRD = 0xFF; PORTD = 0x00;  // LED Array #9-10 output

    static task task1;
    task *tasks[] = {&task1};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    task1.state = display;
    task1.period = 50;
    task1.elapsedTime = task1.period;
    task1.TickFct = &JoystickSM;

    unsigned long GCD = tasks[0]->period;
    for (unsigned short k = 1; k < numTasks; k++)
    {
        GCD = findGCD(GCD, tasks[k]->period);
    }

    TimerSet(GCD);
    TimerOn();
    A2D_init();

    while (1) 
    {
        for (int i = 0; i < numTasks; i++ ) {
            if ( tasks[i]->elapsedTime == tasks[i]->period ) {
                tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
                tasks[i]->elapsedTime = 0;
            }
            tasks[i]->elapsedTime += GCD;
        }

        while (!TimerFlag);
        TimerFlag = 0;
    }

    return 1;
}
