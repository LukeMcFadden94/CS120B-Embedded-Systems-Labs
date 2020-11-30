/*	Author: Luke McFadden
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 10  Exercise 1
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
#include "scheduler.h"
#include "keypad.h"
#include "bit.h"
#endif

enum pauseButtonSM {pauseWait, pausePress, pauseRelease} pauseSM;
enum toggleLED0 {toggleLED0Wait, toggleLED0Blink} LEDzero;
enum toggleLED1 {toggleLED1Wait, toggleLED1Blink} LEDone;
enum display {displayState} displaySM;

unsigned char led0_output = 0x00;
unsigned char led1_output = 0x00;
unsigned char keypad_output = 0x00;
unsigned char pause = 0;

int toggleLED0SMTick (int state)
{
    switch (state)     
    {                   // Transitions begin
        case toggleLED0Wait:
            state = !pause? toggleLED0Blink: toggleLED0Wait;
            break;

        case toggleLED0Blink:
            state = pause? toggleLED0Wait: toggleLED0Blink;
            break;

        default:
            state = toggleLED0Wait;
    }                   // Transitions end

    switch (state)
    {                   // State actions begin
        case toggleLED0Wait:
            break;

        case toggleLED0Blink:
            led0_output = (led0_output == 0x00) ? 0x01 : 0x00;
            break;
    }                   // State actions end

    return state;
}

int toggleLED1SMTick (int state)
{
    switch (state)     
    {                   // Transitions begin
        case toggleLED1Wait:
            state = !pause? toggleLED1Blink: toggleLED1Wait;
            break;

        case toggleLED1Blink:
            state = pause? toggleLED1Wait: toggleLED1Blink;
            break;

        default:
            state = toggleLED1Wait;
    }                   // Transitions end

    switch (state)
    {                   // State actions begin
        case toggleLED1Wait:
            break;

        case toggleLED1Blink:
            led0_output = (led0_output == 0x00) ? 0x01 : 0x00;
            break;
    }                   // State actions end

    return state;
}

int pauseButtonSMTick (int state)
{
    unsigned char press = ~PINA & 0x01;

    switch (state)      // Transitions begin
    {
        case pauseWait:
            state = press == 0x01? pausePress: pauseWait;
            break;

        case pausePress:
            state = pauseRelease;
            break;

        case pauseRelease:
            state = press == 0x00? pauseWait: pausePress;
            break;

        default:
            state = pauseWait;
            break;
    }                   // Transitions end

    switch (state)
    {                   // State actions begin
        case pauseWait:
            break;

        case pausePress:
            pause = (pause == 0) ? 1 : 0;
            break;

        case pauseRelease:
            break;
    }                   // State actions end

    return state;
}

int displaySMTick (int state)
{
    //unsigned char output;

    switch (state)
    {                   // Transitions begin
        case displayState:
            state = displayState;
            break;

        default:
            state = displayState;
            break;
    }                   // Transitions end

    switch (state)
    {                   // State actions begin
        case displayState:
        //     output = led0_output | led1_output << 1;
        //     break;
            if (keypad_output != 0x1F)
            {
                PORTB = 0x80;
            }
            else
            {
                PORTB = 0x00;
            }
            break;
    }                   // State actions end

    //PORTB = output;
    return state;
}

enum keypad_States {keypad_keypad};

int keypadSMTick(int kp_currState) 
{
    unsigned char x;

    switch (kp_currState) 
    {
        case keypad_keypad:
            x = GetKeypadKey();
            switch (x) 
            {
                case '\0': keypad_output = 0x1F; break;
                case '1': keypad_output = 0x01; break;
                case '2': keypad_output = 0x02; break;
                case '3': keypad_output = 0x03; break;
                case '4': keypad_output = 0x04; break;
                case '5': keypad_output = 0x05; break;
                case '6': keypad_output = 0x06; break;
                case '7': keypad_output = 0x07; break;
                case '8': keypad_output = 0x08; break;
                case '9': keypad_output = 0x09; break;
                case 'A': keypad_output = 0x0A; break;
                case 'B': keypad_output = 0x0B; break;
                case 'C': keypad_output = 0x0C; break;
                case 'D': keypad_output = 0x0D; break;
                case '*': keypad_output = 0x0E; break;
                case '0': keypad_output = 0x00; break;
                case '#': keypad_output = 0x0F; break;
                default: keypad_output = 0x1B; break;
            }

            kp_currState = keypad_keypad;
            break;

        default:
            kp_currState = keypad_keypad;
            break;
    }

    switch (kp_currState) 
    {
        default: 
            break;
    }
    
    return kp_currState;
}



int main(void) 
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xF0; PORTC = 0x0F;

    static task task1, task2, task3, task4, task5;
    task *tasks[] = { &task1, &task2, &task3, &task4, &task5};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    const char start = -1;

    task1.state = start;
    task1.period = 50;
    task1.elapsedTime = task1.period;
    task1.TickFct = &pauseButtonSMTick;

    task2.state = start;
    task2.period = 500;
    task2.elapsedTime = task2.period;
    task2.TickFct = &toggleLED0SMTick;

    task3.state = start;
    task3.period = 1000;
    task3.elapsedTime = task3.period;
    task3.TickFct = &toggleLED1SMTick;

    task4.state = start;
    task4.period = 10;
    task4.elapsedTime = task4.period;
    task4.TickFct = &displaySMTick;

    task5.state = start;
    task5.period = 100;
    task5.elapsedTime = task5.period;
    task5.TickFct = &keypadSMTick;

    unsigned long GCD = tasks[0]->period;
    unsigned short k;
    for (k = 1; k < numTasks; k++)
    {
        GCD = findGCD(GCD, tasks[k]->period);
    }

    TimerSet(GCD);
    TimerOn();

    unsigned short i;

    while(1) 
    {
        for ( i = 0; i < numTasks; i++ ) {
            if ( tasks[i]->elapsedTime == tasks[i]->period ) {
                tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
                tasks[i]->elapsedTime = 0;
            }
            tasks[i]->elapsedTime += 1;
        }
        while(!TimerFlag);
        TimerFlag = 0;
    }

    return 0;
}