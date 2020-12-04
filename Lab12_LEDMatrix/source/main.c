/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
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

enum States {} state;

void Tick()
{
    switch (state)  // Transitions begin
    {

    }               // Transitions end

    switch (state)  // State actions begin
    {

    }               // State actions end
}

int main(void) 
{
    
    TimerSet(1000);
    TimerOn();
    state = Start;

    while (1) 
    {
        Tick();

        while (!TimerFlag);
        TimerFlag = 0;
    }

    return 1;
}
