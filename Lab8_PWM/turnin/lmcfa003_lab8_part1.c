/*  Author: Luke McFadden
 *  Partner(s) Name: 
 *  Lab Section: 023
 *  Assignment: Lab 8  Exercise 1
 *  Exercise Description: 
 *
 *  I acknowledge all content contained herein, excluding template or example
 *  code, is my own original work.
 *
  * Demo Link: https://youtu.be/-2y9ThT7ei4
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif


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

unsigned char fC = 0;
unsigned char fD = 0;
unsigned char fE = 0;

void Tick()
{
    fC = (~PINA) & 0x01;
    fD = (~PINA) & 0x02;
    fE = (~PINA) & 0x04;

    switch (state)  // Transitions begin
    {
        case start:
            state = wait;
            break;

        case wait:
            if (fC && !fD && !fE)
                state = soundC;
            else if(!fC && fD && !fE)
                state = soundD;
            else if(!fC && !fD && fE)
                state = soundE;
            else
                state = wait;

            break;

        case soundC:
            if (fC && !fD && !fE)
                state = soundC;
            else
                state = wait;
            break;

        case soundD:
            if (!fC && fD && !fE)
                state = soundD;
            else
                state = wait;
            break;

        case soundE:
            if (!fC && !fD && fE)
                state = soundE;
            else
                state = wait;
            break;    

        default:
            state = start;
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

    PWM_on();
    state = start;

    while (1) 
    {
        Tick();
    }

    return 1;
}
