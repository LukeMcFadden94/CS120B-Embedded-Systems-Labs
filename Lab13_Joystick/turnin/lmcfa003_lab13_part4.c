/*	Author: Luke McFadden
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 13  Exercise 4
 *	Exercise Description:
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * Demo Link: https://youtu.be/5LHiP0uQ64o
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

// Pins on PORTA are used as input for A2D conversion
//    The default channel is 0 (PA0)
// The value of pinNum determines the pin on PORTA
//    used for A2D conversion
// Valid values range between 0 and 7, where the value
//    represents the desired pin for A2D conversion
void Set_A2D_Pin(unsigned char pinNum)  
{
    ADMUX = (pinNum <= 0x07) ? pinNum : ADMUX;
    // Allow channel to stabilize
    static unsigned char i = 0;
    for ( i=0; i<15; i++ ) { asm("nop"); } 
}


unsigned short rate = 100;
static unsigned char pattern = 0x80;   // LED pattern - 0: LED off; 1: LED on
static unsigned char row = 0x01;       // Row(s) displaying pattern. 
                                // 0: display pattern on row
                                // 1: do NOT display pattern on row

// Middle value of joystick is 512.
// However at rest, the LED will continue to move, 
// suggesting that something is making it so that the ADC
// value isn't always perfectly at 512 when the joystick is at rest.
// I have found that when joystick is at rest, the ADC varies between
// 540 and 560 - attempting to make the range any tigher allows for the LED
// to move unexpectedly, even when joystick is at rest.

// enum Speed {stop, rateLeft, rateRight};
// int SpeedSM(int state)
// {
//     unsigned short input = ADC;

//     switch (state)  // Transitions begin
//     {
//         // Left is 0 through <540
//         // Right is 560 - 1024
//         case stop:                         
//             if ((input >= 540) && (input < 560))        // 540 - <560
//                 state = stop;
//             else if (input < 540)                       // 0 - <540
//                 state = rateLeft;
//             else if (input >= 560)                      // 560 - 1024
//                  state = rateRight;
//             break;

//         case rateLeft:
//             if (input < 540)                            // 0 - <540
//                     state = rateLeft;
//             else if ((input >= 540) && (input < 560))   // 540 - <560
//                 state = stop;
//             else if (input >= 560)                      // 560 - 1024
//                  state = rateRight;
//             break;

//         case rateRight:
//             if (input >= 560)                           // 560 - 1024
//                  state = rateRight;
//             else if (input < 540)                       // 0 - <540
//                 state = rateLeft;
//             else if ((input >= 540) && (input < 560))   // 540 - <560
//                 state = stop;
//             break;

//         default:
//             state = stop;
//             break;
//     }               // Transitions end

//     switch (state)  // State actions begin
//     {
//         case stop: 
//             TimerSet(100);
//             break;

//         case rateLeft:                                          // 0 - <540   
//             // 0 - <90 == 100
//             // 90 - <260 == 250
//             // 260 - <410 == 500
//             // 410 - <540 == 1000
//             if (input < 90)
//                 TimerSet(100);
//             else if ((input >= 90) && (input < 260))
//                 TimerSet(250);
//             else if ((input >= 260) && (input < 410))
//                 TimerSet(500);
//             else if ((input >= 410) && (input < 540))
//                 TimerSet(1000);
//             break;

//         case rateRight:                                          // 560 - 1024  
//             // 560 - <672 == 1000
//             // 672 - <800 == 500
//             // 800 - <946 == 250
//             // 946 - 1024 == 100
//             if ((input >= 560) && (input < 672))
//                 TimerSet(1000);
//             else if ((input >= 672) && (input < 800))
//                 TimerSet(500);
//             else if ((input >= 800) && (input < 946))
//                 TimerSet(250);
//             else if (input >= 946)
//                 TimerSet(100);
//             break;
//     }               // State actions end

//     return state;
// }

unsigned short horizontal;
unsigned short vertical;

enum GetADC {update};
int GetADCSM(int state)
{
    // PIN A0 controls L & R
    // PIN A1 controls U & D
    switch (state)  // Transitions begin
    {
        case update:
            state = update;
            break;

        default:
            state = update;
            break;
    }               // Transitions end

    switch (state)  // State actions begin
    {
        case update:    
            Set_A2D_Pin(0x00);
            horizontal = ADC;
            //for (int i = 0; i < 15; i++){}
            Set_A2D_Pin(0x01);
            vertical = ADC;
            break;
    }               // State actions end

    return state;
}


enum Joystick {noMove, right, left, up, down, downRight, upRight, downLeft, upLeft};
int JoystickSM(int state)
{
    switch (state)  // Transitions begin
    {
        case noMove:
            if (((horizontal >= 530) && (horizontal <= 570)) && ((vertical >= 530) && (vertical <= 570)))
                state = noMove;
            else if ((horizontal > 570) && ((vertical >= 530) && (vertical <= 570)))    
                state = right;
            else if ((horizontal < 530) && ((vertical >= 530) && (vertical <= 570)))    
                state = left;
            else if ((vertical > 570) && ((horizontal >= 530) && (horizontal <= 570)))
                state = up;
            else if ((vertical < 530) && ((horizontal >= 530) && (horizontal <= 570)))
                state = down;
            else if ((horizontal > 570) && (vertical < 530))       
                state = downRight;
            else if ((horizontal > 570) && (vertical > 570))       
                state = upRight;
            else if ((horizontal < 530) && (vertical < 530))       
                state = downLeft;
            else if ((horizontal < 530) && (vertical > 570))       
                state = upLeft;
            break;

        case right:
            if (((horizontal >= 530) && (horizontal <= 570)) && ((vertical >= 530) && (vertical <= 570)))
                state = noMove;
            else if ((horizontal > 570) && ((vertical >= 530) && (vertical <= 570)))    
                state = right;
            else if ((horizontal < 530) && ((vertical >= 530) && (vertical <= 570)))    
                state = left;
            else if ((vertical > 570) && ((horizontal >= 530) && (horizontal <= 570)))
                state = up;
            else if ((vertical < 530) && ((horizontal >= 530) && (horizontal <= 570)))
                state = down;
            else if ((horizontal > 570) && (vertical < 530))       
                state = downRight;
            else if ((horizontal > 570) && (vertical > 570))       
                state = upRight;
            else if ((horizontal < 530) && (vertical < 530))       
                state = downLeft;
            else if ((horizontal < 530) && (vertical > 570))       
                state = upLeft;
            break;

        case left:
            if (((horizontal >= 530) && (horizontal <= 570)) && ((vertical >= 530) && (vertical <= 570)))
                state = noMove;
            else if ((horizontal > 570) && ((vertical >= 530) && (vertical <= 570)))    
                state = right;
            else if ((horizontal < 530) && ((vertical >= 530) && (vertical <= 570)))    
                state = left;
            else if ((vertical > 570) && ((horizontal >= 530) && (horizontal <= 570)))
                state = up;
            else if ((vertical < 530) && ((horizontal >= 530) && (horizontal <= 570)))
                state = down;
            else if ((horizontal > 570) && (vertical < 530))       
                state = downRight;
            else if ((horizontal > 570) && (vertical > 570))       
                state = upRight;
            else if ((horizontal < 530) && (vertical < 530))       
                state = downLeft;
            else if ((horizontal < 530) && (vertical > 570))       
                state = upLeft;
            break;

        case up:
            if (((horizontal >= 530) && (horizontal <= 570)) && ((vertical >= 530) && (vertical <= 570)))
                state = noMove;
            else if ((horizontal > 570) && ((vertical >= 530) && (vertical <= 570)))    
                state = right;
            else if ((horizontal < 530) && ((vertical >= 530) && (vertical <= 570)))    
                state = left;
            else if ((vertical > 570) && ((horizontal >= 530) && (horizontal <= 570)))
                state = up;
            else if ((vertical < 530) && ((horizontal >= 530) && (horizontal <= 570)))
                state = down;
            else if ((horizontal > 570) && (vertical < 530))       
                state = downRight;
            else if ((horizontal > 570) && (vertical > 570))       
                state = upRight;
            else if ((horizontal < 530) && (vertical < 530))       
                state = downLeft;
            else if ((horizontal < 530) && (vertical > 570))       
                state = upLeft;
            break;

        case down:
            if (((horizontal >= 530) && (horizontal <= 570)) && ((vertical >= 530) && (vertical <= 570)))
                state = noMove;
            else if ((horizontal > 570) && ((vertical >= 530) && (vertical <= 570)))    
                state = right;
            else if ((horizontal < 530) && ((vertical >= 530) && (vertical <= 570)))    
                state = left;
            else if ((vertical > 570) && ((horizontal >= 530) && (horizontal <= 570)))
                state = up;
            else if ((vertical < 530) && ((horizontal >= 530) && (horizontal <= 570)))
                state = down;
            else if ((horizontal > 570) && (vertical < 530))       
                state = downRight;
            else if ((horizontal > 570) && (vertical > 570))       
                state = upRight;
            else if ((horizontal < 530) && (vertical < 530))       
                state = downLeft;
            else if ((horizontal < 530) && (vertical > 570))       
                state = upLeft;
            break;

        case downRight:
            if (((horizontal >= 530) && (horizontal <= 570)) && ((vertical >= 530) && (vertical <= 570)))
                state = noMove;
            else if ((horizontal > 570) && ((vertical >= 530) && (vertical <= 570)))    
                state = right;
            else if ((horizontal < 530) && ((vertical >= 530) && (vertical <= 570)))    
                state = left;
            else if ((vertical > 570) && ((horizontal >= 530) && (horizontal <= 570)))
                state = up;
            else if ((vertical < 530) && ((horizontal >= 530) && (horizontal <= 570)))
                state = down;
            else if ((horizontal > 570) && (vertical < 530))       
                state = downRight;
            else if ((horizontal > 570) && (vertical > 570))       
                state = upRight;
            else if ((horizontal < 530) && (vertical < 530))       
                state = downLeft;
            else if ((horizontal < 530) && (vertical > 570))       
                state = upLeft;
            break;

        case upRight:
            if (((horizontal >= 530) && (horizontal <= 570)) && ((vertical >= 530) && (vertical <= 570)))
                state = noMove;
            else if ((horizontal > 570) && ((vertical >= 530) && (vertical <= 570)))    
                state = right;
            else if ((horizontal < 530) && ((vertical >= 530) && (vertical <= 570)))    
                state = left;
            else if ((vertical > 570) && ((horizontal >= 530) && (horizontal <= 570)))
                state = up;
            else if ((vertical < 530) && ((horizontal >= 530) && (horizontal <= 570)))
                state = down;
            else if ((horizontal > 570) && (vertical < 530))       
                state = downRight;
            else if ((horizontal > 570) && (vertical > 570))       
                state = upRight;
            else if ((horizontal < 530) && (vertical < 530))       
                state = downLeft;
            else if ((horizontal < 530) && (vertical > 570))       
                state = upLeft;
            break;   

        case downLeft:
            if (((horizontal >= 530) && (horizontal <= 570)) && ((vertical >= 530) && (vertical <= 570)))
                state = noMove;
            else if ((horizontal > 570) && ((vertical >= 530) && (vertical <= 570)))    
                state = right;
            else if ((horizontal < 530) && ((vertical >= 530) && (vertical <= 570)))    
                state = left;
            else if ((vertical > 570) && ((horizontal >= 530) && (horizontal <= 570)))
                state = up;
            else if ((vertical < 530) && ((horizontal >= 530) && (horizontal <= 570)))
                state = down;
            else if ((horizontal > 570) && (vertical < 530))       
                state = downRight;
            else if ((horizontal > 570) && (vertical > 570))       
                state = upRight;
            else if ((horizontal < 530) && (vertical < 530))       
                state = downLeft;
            else if ((horizontal < 530) && (vertical > 570))       
                state = upLeft;
            break; 

        case upLeft:
            if (((horizontal >= 530) && (horizontal <= 570)) && ((vertical >= 530) && (vertical <= 570)))
                state = noMove;
            else if ((horizontal > 570) && ((vertical >= 530) && (vertical <= 570)))    
                state = right;
            else if ((horizontal < 530) && ((vertical >= 530) && (vertical <= 570)))    
                state = left;
            else if ((vertical > 570) && ((horizontal >= 530) && (horizontal <= 570)))
                state = up;
            else if ((vertical < 530) && ((horizontal >= 530) && (horizontal <= 570)))
                state = down;
            else if ((horizontal > 570) && (vertical < 530))       
                state = downRight;
            else if ((horizontal > 570) && (vertical > 570))       
                state = upRight;
            else if ((horizontal < 530) && (vertical < 530))       
                state = downLeft;
            else if ((horizontal < 530) && (vertical > 570))       
                state = upLeft;
            break;

        default:
            state = noMove;
            break;
    }               // Transitions end

    switch (state)  // State actions begin
    {
        case noMove:               
            break;

        case right:
            if (pattern == 0x01)
                pattern = 0x01;
            else if (pattern > 0x01)
                pattern = (pattern >> 1);
            break;

        case left:
            if (pattern == 0x80)           
                pattern = 0x80;               
            else if (pattern < 0x80)            
                pattern = (pattern << 1);   
            break;

        case up:
            if (row == 0x01)
                row = 0x01;
            else if (row > 0x01)
                row = (row >> 1);
            break;

        case down:
            if (row == 0x10)
                row = 0x10;
            else if (row < 0x10)
                row = (row << 1);
            break;

        case downRight:
            if (pattern == 0x01)
                pattern = 0x01;
            else if (pattern > 0x01)
                pattern = (pattern >> 1);

            if (row == 0x10)
                row = 0x10;
            else if (row < 0x10)
                row = (row << 1);
            break;

        case upRight:
            if (pattern == 0x01)
                pattern = 0x01;
            else if (pattern > 0x01)
                pattern = (pattern >> 1);

            if (row == 0x01)
                row = 0x01;
            else if (row > 0x01)
                row = (row >> 1);
            break;

        case downLeft:
            if (pattern == 0x80)           
                pattern = 0x80;               
            else if (pattern < 0x80)            
                pattern = (pattern << 1); 

            if (row == 0x10)
                row = 0x10;
            else if (row < 0x10)
                row = (row << 1);
            break;

        case upLeft:
             if (pattern == 0x80)           
                pattern = 0x80;               
            else if (pattern < 0x80)            
                pattern = (pattern << 1);  

            if (row == 0x01)
                row = 0x01;
            else if (row > 0x01)
                row = (row >> 1);
            break; 
    }               // State actions end

    return state;
}

enum LEDMatrix {display};
int LEDMatrixSM(int state)
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
            PORTC = pattern;
            PORTD = ~row;
            break;
    }               // State actions end

    return state;
}

int main(void) 
{
    DDRA = 0x00; PORTA = 0xFF;  // Joystick input
    DDRC = 0xFF; PORTC = 0x00;  // LED Matrix pattern
    DDRD = 0x1F; PORTD = 0x00;  // LED Matrix rows

    static task task1, task2, task3;
    task *tasks[] = {&task1, &task2, &task3};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    task1.state = update;
    task1.period = 25;
    task1.elapsedTime = task1.period;
    task1.TickFct = &GetADCSM;

    task2.state = noMove;
    task2.period = 100;
    task2.elapsedTime = task2.period;
    task2.TickFct = &JoystickSM;

    task3.state = display;
    task3.period = 50;
    task3.elapsedTime = task3.period;
    task3.TickFct = &LEDMatrixSM;

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
