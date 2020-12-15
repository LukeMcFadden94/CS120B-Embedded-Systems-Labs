/*	Author: Luke McFadden
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 15  Exercise 2
 *	Exercise Description: 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * Demo Link: https://youtu.be/bsKRJL9CfAo
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
unsigned char output1 = 0xF0;
unsigned char output2 = 0xAA;
unsigned char output3 = 0x80;
unsigned char output4 = 0x00;
//unsigned char output4 = 

enum pattern1 {AOne, ATwo};
int pattern1SM (int state)
{
    switch (state)
    {                   // Transitions begin
        case AOne:
            state = ATwo;
            break;

        case ATwo:
            state = AOne;

        default:
            state = AOne;
            break;
    }                   // Transitions end

    switch (state)
    {                   // State actions begin
        case AOne:
            output1 = 0xF0;
            break;

        case ATwo:
            output1 = 0x0F;
            break;     
    }                   // State actions end

    return state;
}

enum pattern2 {BOne, BTwo};
int pattern2SM (int state)
{
    switch (state)
    {                   // Transitions begin
        case BOne:
            state = BTwo;
            break;

        case BTwo:
            state = BOne;

        default:
            state = BOne;
            break;
    }                   // Transitions end

    switch (state)
    {                   // State actions begin
        case BOne:
            output2 = 0xAA;
            break;

        case BTwo:
            output2 = 0x55;
            break;     
    }                   // State actions end

    return state;
}

unsigned char flip = 0; // 0 for decrease, 1 for increase

enum pattern3 {COne, CTwo};
int pattern3SM (int state)
{
    switch (state)
    {                   // Transitions begin
        case COne:
            if (flip == 0)
                state = COne;
            else if (flip == 1)
                state = CTwo;
            break;

        case CTwo:
            if (flip == 1)
                state = CTwo;
            else if (flip == 0)
                state = COne;
            break;

        default:
            state = COne;
            break;
    }                   // Transitions end

    switch (state)
    {                   // State actions begin
        case COne:
            if (output3 > 1)
                output3 = output3 >> 1;
            else if (output3 == 1)
                flip = 1;
            break;

        case CTwo:
            if (output3 < 128)
                output3 = output3 << 1;
            else if (output3 == 128)
                flip = 0;
            break;     
    }                   // State actions end

    return state;
}

enum pattern4 {DOne, DTwo, DThree, DFour, DFive, DSix};
int pattern4SM (int state)
{
    switch (state)
    {                   // Transitions begin
        case DOne:
            state = DTwo;
            break;

        case DTwo:
            state = DThree;
            break;

        case DThree:
            state = DFour;
            break;

        case DFour:
            state = DFive;
            break;

        case DFive:
            state = DSix;
            break;

        case DSix:
            state = DOne;
            break;

        default:
            state = DOne;
            break;
    }                   // Transitions end

    switch (state)
    {                   // State actions begin
        case DOne:
            output4 = 0x81;
            break;

        case DTwo:
            output4 = 0x42;
            break;

        case DThree:
            output4 = 0x24;
            break;

        case DFour:
            output4 = 0x18;
            break;

        case DFive:
            output4 = 0x24;
            break;

        case DSix:
            output4 = 0x42;
            break;

        break;
          
    }                   // State actions end

    return state;
}

enum output {wait, show1, show2, show3, show4};
int outputSM (int state)
{
    switch (state)
    {                   // Transitions begin
        case wait:
            if (receivedData == 0x0A)
                state = show1;
            else if (receivedData == 0x0B)
                state = show2;
            else if (receivedData == 0x0C)
                state = show3;
            else if (receivedData == 0x0D)
                state = show4;
            break;

        case show1:
            if (receivedData == 0x0A)
                state = show1;
            else
                state = wait;
            break;

        case show2:
            if (receivedData == 0x0B)
                state = show2;
            else
                state = wait;
            break;

        case show3:
            if (receivedData == 0x0C)
                state = show3;
            else
                state = wait;
            break;

        case show4:
            if (receivedData == 0x0D)
                state = show4;
            else
                state = wait;
        
        default:
            state = wait;
            break;
    }                   // Transitions end

    switch (state)
    {                   // State actions begin
        case wait:
            PORTC = 0x00;
            break;

        case show1:
            PORTC = output1;
            break; 

        case show2:
            PORTC = output2;
            break; 

        case show3:
            PORTC = output3;
            break;

        case show4:
            PORTC = output4;
            break;     
    }                   // State actions end

    return state;
}


int main(void) 
{
    DDRB = 0x00; PORTB = 0xFF;  // SLI
    DDRC = 0xFF; PORTC = 0x00;  // LED Array
 
    static task task1, task2, task3, task4, task5;
    task *tasks[] = {&task1, &task2, &task3, &task4, &task5};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    task1.state = AOne;
    task1.period = 300;
    task1.elapsedTime = task1.period;
    task1.TickFct = &pattern1SM;

    task2.state = BOne;
    task2.period = 300;
    task2.elapsedTime = task2.period;
    task2.TickFct = &pattern2SM;

    task3.state = COne;
    task3.period = 300;
    task3.elapsedTime = task3.period;
    task3.TickFct = &pattern3SM;

    task4.state = DOne;
    task4.period = 500;
    task4.elapsedTime = task4.period;
    task4.TickFct = &pattern4SM;

    task5.state = wait;
    task5.period = 300;
    task5.elapsedTime = task5.period;
    task5.TickFct = &outputSM;

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
