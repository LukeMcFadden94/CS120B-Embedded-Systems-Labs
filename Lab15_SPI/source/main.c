/*	Author: Luke McFadden
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 15  Exercise 1
 *	Exercise Description: 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * Demo Link: https://youtu.be/BJUwlRBpaxA
 */

/*******************************

    Program for Follower ATmega

*******************************/

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "timer.h"
#include "keypad.h"
#include "scheduler.h"
#include "SPI.h"
#include "bit.h"

/****************************************

    LED Matrix wired as:

    PA4: pin 13 ROW 5       PC0: pin 9  COL 1
    PA3: pin 3  ROW 4       PC1: pin 14 COL 2
    PA2: pin 4  ROW 3       PC2: pin 8  COL 3
    PA1: pin 10 ROW 2       PC3: pin 12 COL 4
    PA0: pin 6  ROW 1       PC4: pin 5  COL 5
                            PC5: pin 1  COL 6
                            PC6: pin 7  COL 7
                            PC7: pin 2  COL 8

    pin 11 unused

***************************************/

/**********************

    SPI pins:
    PINB4: SS       (Slave Select)
    PINB5: MOSI     (Master Out, Slave In)
    PINB6: MISO     (Master In, Slave Out)
    PINB7: SCK      (Serial Clock, output from Master)

    DDR_SPI == DDRB
    DD_MOSI == PINB5
    DD_MISO == PINB6
    DD_SCK == PINB7

**********************/

enum LEDArray {display};
int displaySM (int state)
{
    switch (state)
    {                   // Transitions begin
        case display:
            state = display;
            break;

        default:
            state = display;
            break;
    }                   // Transitions end

    switch (state)
    {                   // State actions begin
        case display:
            PORTC = receivedData;
            break;     
    }                   // State actions end

    return state;
}

int main(void) 
{
    DDRB = 0x00; PORTB = 0xFF;  // SLI
    DDRC = 0xFF; PORTC = 0x00;  // LED Array
 
    static task task1;
    task *tasks[] = {&task1};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    task1.state = display;
    task1.period = 1000;
    task1.elapsedTime = task1.period;
    task1.TickFct = &displaySM;

    unsigned long GCD = tasks[0]->period;
    for (unsigned short k = 1; k < numTasks; k++)
    {
        GCD = findGCD(GCD, tasks[k]->period);
    }

    TimerSet(GCD);
    TimerOn();

    SPI_SlaveInit();

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
