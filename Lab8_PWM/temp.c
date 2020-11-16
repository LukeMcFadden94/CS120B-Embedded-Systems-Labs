/*	Author: Luke McFadden
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab 8  Exercise 1
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

#define input (~PINA & 0x07)

void set_PWM(double frequency)
{
    static double current_frequency;

    if (frequency != current_frequency)
    {
        if (!frequency)
            TCCR3B &= 0x08;
        else
            TCCR3B |= 0x03;

        if (frequency < 0.954)
            OCR3A = 0xFFFF;
        else if (frequency > 31250)
            OCR3A = 0x0000;
        else
            OCR3A = (short)(8000000 / (128 * frequency)) - 1;

        TCNT3 = 0;
        current_frequency = frequency;
    }
}

void PWM_on()
{
    TCCR3A = (1 << COM3A0);
    TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
    set_PWM(0);
}

void PWM_off()
{
    TCCR3A = 0x00;
    TCCR3B = 0x00;
}

enum States {start, wait, soundC, soundD, soundE} state;

void Tick()
{
    switch (state)  // Transitions begin
    {
        case start:
            state = wait;
            break;

        case wait:
            switch(input)
            {
                case 1:
                    state = soundC;
                    break;

                case 2:
                    state = soundD;
                    break;

                case 4:
                    state = soundE;
                    break;

                default:
                    state = start;
                    break;
            }
            break;

        case soundC:
            if (input == 1)
                state = soundC;
            else
                state = wait;
            break;

        case soundD:
            if (input == 2)
                state = soundD;
            else
                state = wait;
            break;

        case soundE:
            if (input == 4)
                state = soundE;
            else
                state = wait;
            break;      
    }               // Transitions end

    switch (state)  // State actions begin
    {
        case start:
            break;

        case wait:
            set_PWM(0);
            break;

        case soundC:
            set_PWM(261.63);
            break;

        case soundD:
            set_PWM(293.66);
            break;

        case soundE:
            set_PWM(329.63);
            break;
    }               // State actions end
}

int main(void) 
{
    DDRA = 0x00; PORTA = 0xFF;  // PORT A is input
    DDRB = 0xFF; PORTB = 0x00;  // PORT B is output

    TimerSet(1000);
    TimerOn();
    PWM_on();
    state = start;

    while (1) 
    {
        Tick();

        while (!TimerFlag);
        TimerFlag = 0;
    }

    return 1;
}
