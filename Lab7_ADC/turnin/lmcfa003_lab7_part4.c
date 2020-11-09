/*	Author: Luke McFadden
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab 7  Exercise 4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Link: https://youtu.be/HBKbhn9mrEg
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void ADC_init()
{
    ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

int main(void) 
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    unsigned short max = 128;
    unsigned char light = 0x00;

    ADC_init();


    while (1) 
    {
        unsigned short x = ADC;

        if (x >= max)
            light = 0xFF;
        else if (x >= 112)
            light = 0x7F;
        else if (x >= 96)
            light = 0x3F;
        else if (x >= 80)
            light = 0x1F;
        else if (x >= 64)
            light = 0x0F;
        else if (x >= 48)
            light = 0x07;
        else if (x >= 32)
            light = 0x03;
        else if (x >= 16)
            light = 0x01;
        else if (x <= 0)
            light = 0x00;

        PORTB = light;
    }

    return 1;
}
