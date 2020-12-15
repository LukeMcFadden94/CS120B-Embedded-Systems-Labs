/*  Author: Luke McFadden
 *  Partner(s) Name: 
 *  Lab Section:
 *  Assignment: Lab 15  Exercise 2
 *  Exercise Description: 
 *
 *  I acknowledge all content contained herein, excluding template or example
 *  code, is my own original work.
 *
 * Demo Link: https://youtu.be/bsKRJL9CfAo
 */

/*******************************

    Program for Leader ATmega

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

// int state;

// enum Demo_States {shift};
// int Demo_Tick(int state) 
// {

//     // Local Variables
//     static unsigned char pattern = 0x80;    // LED pattern - 0: LED off; 1: LED on
//     static unsigned char row = 0xFE;    // Row(s) displaying pattern. 
//                             // 0: display pattern on row
//                             // 1: do NOT display pattern on row
//     // Transitions
//     switch (state) 
//     {
//         case shift: 
//             break;
//         default:    
//             state = shift;
//             break;
//     }   
//     // Actions
//     switch (state) {
//         case shift: 
//             if (row == 0xEF && pattern == 0x01) 
//             { // Reset demo 
//                 pattern = 0x80;         
//                 row = 0xFE;
//             } 
//             else if (pattern == 0x01) 
//             { // Move LED to start of next row
//                 pattern = 0x80;
//                 row = (row << 1) | 0x01;
//             } 
//             else 
//             { // Shift LED one spot to the right on current row
//                 pattern >>= 1;
//             }
//             break;
//         default:
//         break;
//     }
//     PORTC = pattern;    // Pattern to display
//     PORTA = row;        // Row(s) displaying pattern    
//     return state;
// }

unsigned char pattern = 0x00;
//unsigned char speed = 0x00;
unsigned char data = 0x00;
unsigned char keypad_output = 0x00;
unsigned char flag = 0;
unsigned char x = 0x00;

//enum Transmit {reset, keypad, getPattern, getSpeed, transmit};
enum Transmit {reset, keypad, getPattern, transmit};
int TransmitSM(int state)
{
    switch (state)
    {                   // Transitions begin
        case reset:
            state = keypad;
            break;

        // case keypad:
        //     if (flag == 0)
        //         state = getPattern;
        //     else if (flag == 1)
        //         state = getSpeed;
        //     break;

        // case getPattern:
        //     state = keypad;
        //     break;

        // case getSpeed:
        //     state = transmit;
        //     break;

        case keypad:
            state = getPattern;
            break;

        case getPattern:
            state = transmit;
            break;

        case transmit:
            state = reset;
            break;

        default:
            state = reset;
            break;
    }                   // Transitions end

    switch (state)
    {                   // State actions begin
        case reset:
            data = 0x00;
            pattern = 0x00;
            break;

        case keypad:
            x = GetKeypadKey();
            switch (x) 
            {
                case '\0': keypad_output = keypad_output; break;
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
                default: keypad_output = 0x00; break;
            }
            // if (flag == 0)      // Got pattern, change to get speed
            //     flag = 1;
            // else if (flag == 1) // Got speed, change to get pattern
            //     flag = 0;
            break;

        case getPattern:
            pattern = keypad_output;
            break;

        // case getSpeed:
        //     speed = keypad_output;
        //     break;

        case transmit:
            data = data | pattern;
            //data = data | speed;

            SPI_MasterTransmit(data);
            break;
    }                   // State actions end

    return state;
}

int main(void) 
{
    
    //DDRA = 0xFF; PORTA = 0x00;  // Rows of LED Matrix
    DDRB = 0xFF; PORTB = 0x00;  // SPI
    //DDRC = 0xFF; PORTC = 0x00;  // Columns of LED Matrix
    DDRD = 0xF0; PORTD = 0x0F;  // Keypad

    static task task1;
    task *tasks[] = {&task1};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    task1.state = reset;
    task1.period = 100;
    task1.elapsedTime = task1.period;
    task1.TickFct = &TransmitSM;

    unsigned long GCD = tasks[0]->period;
    for (unsigned short k = 1; k < numTasks; k++)
    {
        GCD = findGCD(GCD, tasks[k]->period);
    }

    TimerSet(GCD);
    TimerOn();

    SPI_MasterInit();

    unsigned short i;
    while (1) 
    {
        for (i = 0; i < numTasks; i++ ) {
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
