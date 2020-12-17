/*	Author: Luke McFadden
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 13  Exercise 5
 *	Exercise Description: 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * Demo Link: No demo, wasn't able to get it working as intended
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

unsigned short horizontal;
unsigned short vertical;
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
// 530 and 570 - attempting to make the range any tigher allows for the LED
// to move unexpectedly, even when joystick is at rest.

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

enum Speed {stop, rateRight, rateLeft, rateUp, rateDown, rateDownRight, rateUpRight, rateDownLeft, rateUpLeft};
int SpeedSM(int state)
{
    switch (state)  // Transitions begin
    {
        // Left is 0 through <530
        // Right is 570 - 1024
        case stop:                         
            if (((horizontal >= 530) && (horizontal <= 570)) && ((vertical >= 530) && (vertical <= 570)))
                state = stop;
            else if ((horizontal > 570) && ((vertical >= 530) && (vertical <= 570)))    
                state = rateRight;
            else if ((horizontal < 530) && ((vertical >= 530) && (vertical <= 570)))    
                state = rateLeft;
            else if ((vertical > 570) && ((horizontal >= 530) && (horizontal <= 570)))
                state = rateUp;
            else if ((vertical < 530) && ((horizontal >= 530) && (horizontal <= 570)))
                state = rateDown;
            else if ((horizontal > 570) && (vertical < 530))       
                state = rateDownRight;
            else if ((horizontal > 570) && (vertical > 570))       
                state = rateUpRight;
            else if ((horizontal < 530) && (vertical < 530))       
                state = rateDownLeft;
            else if ((horizontal < 530) && (vertical > 570))       
                state = rateUpLeft;
            break;

        case rateLeft:
            if (((horizontal >= 530) && (horizontal <= 570)) && ((vertical >= 530) && (vertical <= 570)))
                state = stop;
            else if ((horizontal > 570) && ((vertical >= 530) && (vertical <= 570)))    
                state = rateRight;
            else if ((horizontal < 530) && ((vertical >= 530) && (vertical <= 570)))    
                state = rateLeft;
            else if ((vertical > 570) && ((horizontal >= 530) && (horizontal <= 570)))
                state = rateUp;
            else if ((vertical < 530) && ((horizontal >= 530) && (horizontal <= 570)))
                state = rateDown;
            else if ((horizontal > 570) && (vertical < 530))       
                state = rateDownRight;
            else if ((horizontal > 570) && (vertical > 570))       
                state = rateUpRight;
            else if ((horizontal < 530) && (vertical < 530))       
                state = rateDownLeft;
            else if ((horizontal < 530) && (vertical > 570))       
                state = rateUpLeft;
            break;

        case rateRight:
            if (((horizontal >= 530) && (horizontal <= 570)) && ((vertical >= 530) && (vertical <= 570)))
                state = stop;
            else if ((horizontal > 570) && ((vertical >= 530) && (vertical <= 570)))    
                state = rateRight;
            else if ((horizontal < 530) && ((vertical >= 530) && (vertical <= 570)))    
                state = rateLeft;
            else if ((vertical > 570) && ((horizontal >= 530) && (horizontal <= 570)))
                state = rateUp;
            else if ((vertical < 530) && ((horizontal >= 530) && (horizontal <= 570)))
                state = rateDown;
            else if ((horizontal > 570) && (vertical < 530))       
                state = rateDownRight;
            else if ((horizontal > 570) && (vertical > 570))       
                state = rateUpRight;
            else if ((horizontal < 530) && (vertical < 530))       
                state = rateDownLeft;
            else if ((horizontal < 530) && (vertical > 570))       
                state = rateUpLeft;
            break;

        case rateUp:
            if (((horizontal >= 530) && (horizontal <= 570)) && ((vertical >= 530) && (vertical <= 570)))
                state = stop;
            else if ((horizontal > 570) && ((vertical >= 530) && (vertical <= 570)))    
                state = rateRight;
            else if ((horizontal < 530) && ((vertical >= 530) && (vertical <= 570)))    
                state = rateLeft;
            else if ((vertical > 570) && ((horizontal >= 530) && (horizontal <= 570)))
                state = rateUp;
            else if ((vertical < 530) && ((horizontal >= 530) && (horizontal <= 570)))
                state = rateDown;
            else if ((horizontal > 570) && (vertical < 530))       
                state = rateDownRight;
            else if ((horizontal > 570) && (vertical > 570))       
                state = rateUpRight;
            else if ((horizontal < 530) && (vertical < 530))       
                state = rateDownLeft;
            else if ((horizontal < 530) && (vertical > 570))       
                state = rateUpLeft;
            break;

        case rateDown:
            if (((horizontal >= 530) && (horizontal <= 570)) && ((vertical >= 530) && (vertical <= 570)))
                state = stop;
            else if ((horizontal > 570) && ((vertical >= 530) && (vertical <= 570)))    
                state = rateRight;
            else if ((horizontal < 530) && ((vertical >= 530) && (vertical <= 570)))    
                state = rateLeft;
            else if ((vertical > 570) && ((horizontal >= 530) && (horizontal <= 570)))
                state = rateUp;
            else if ((vertical < 530) && ((horizontal >= 530) && (horizontal <= 570)))
                state = rateDown;
            else if ((horizontal > 570) && (vertical < 530))       
                state = rateDownRight;
            else if ((horizontal > 570) && (vertical > 570))       
                state = rateUpRight;
            else if ((horizontal < 530) && (vertical < 530))       
                state = rateDownLeft;
            else if ((horizontal < 530) && (vertical > 570))       
                state = rateUpLeft;
            break;

        case rateDownRight:
            if (((horizontal >= 530) && (horizontal <= 570)) && ((vertical >= 530) && (vertical <= 570)))
                state = stop;
            else if ((horizontal > 570) && ((vertical >= 530) && (vertical <= 570)))    
                state = rateRight;
            else if ((horizontal < 530) && ((vertical >= 530) && (vertical <= 570)))    
                state = rateLeft;
            else if ((vertical > 570) && ((horizontal >= 530) && (horizontal <= 570)))
                state = rateUp;
            else if ((vertical < 530) && ((horizontal >= 530) && (horizontal <= 570)))
                state = rateDown;
            else if ((horizontal > 570) && (vertical < 530))       
                state = rateDownRight;
            else if ((horizontal > 570) && (vertical > 570))       
                state = rateUpRight;
            else if ((horizontal < 530) && (vertical < 530))       
                state = rateDownLeft;
            else if ((horizontal < 530) && (vertical > 570))       
                state = rateUpLeft;
            break;

        case rateUpRight:
            if (((horizontal >= 530) && (horizontal <= 570)) && ((vertical >= 530) && (vertical <= 570)))
                state = stop;
            else if ((horizontal > 570) && ((vertical >= 530) && (vertical <= 570)))    
                state = rateRight;
            else if ((horizontal < 530) && ((vertical >= 530) && (vertical <= 570)))    
                state = rateLeft;
            else if ((vertical > 570) && ((horizontal >= 530) && (horizontal <= 570)))
                state = rateUp;
            else if ((vertical < 530) && ((horizontal >= 530) && (horizontal <= 570)))
                state = rateDown;
            else if ((horizontal > 570) && (vertical < 530))       
                state = rateDownRight;
            else if ((horizontal > 570) && (vertical > 570))       
                state = rateUpRight;
            else if ((horizontal < 530) && (vertical < 530))       
                state = rateDownLeft;
            else if ((horizontal < 530) && (vertical > 570))       
                state = rateUpLeft;
            break;

        case rateDownLeft:
            if (((horizontal >= 530) && (horizontal <= 570)) && ((vertical >= 530) && (vertical <= 570)))
                state = stop;
            else if ((horizontal > 570) && ((vertical >= 530) && (vertical <= 570)))    
                state = rateRight;
            else if ((horizontal < 530) && ((vertical >= 530) && (vertical <= 570)))    
                state = rateLeft;
            else if ((vertical > 570) && ((horizontal >= 530) && (horizontal <= 570)))
                state = rateUp;
            else if ((vertical < 530) && ((horizontal >= 530) && (horizontal <= 570)))
                state = rateDown;
            else if ((horizontal > 570) && (vertical < 530))       
                state = rateDownRight;
            else if ((horizontal > 570) && (vertical > 570))       
                state = rateUpRight;
            else if ((horizontal < 530) && (vertical < 530))       
                state = rateDownLeft;
            else if ((horizontal < 530) && (vertical > 570))       
                state = rateUpLeft;
            break;

        case rateUpLeft:
            if (((horizontal >= 530) && (horizontal <= 570)) && ((vertical >= 530) && (vertical <= 570)))
                state = stop;
            else if ((horizontal > 570) && ((vertical >= 530) && (vertical <= 570)))    
                state = rateRight;
            else if ((horizontal < 530) && ((vertical >= 530) && (vertical <= 570)))    
                state = rateLeft;
            else if ((vertical > 570) && ((horizontal >= 530) && (horizontal <= 570)))
                state = rateUp;
            else if ((vertical < 530) && ((horizontal >= 530) && (horizontal <= 570)))
                state = rateDown;
            else if ((horizontal > 570) && (vertical < 530))       
                state = rateDownRight;
            else if ((horizontal > 570) && (vertical > 570))       
                state = rateUpRight;
            else if ((horizontal < 530) && (vertical < 530))       
                state = rateDownLeft;
            else if ((horizontal < 530) && (vertical > 570))       
                state = rateUpLeft;
            break;

        default:
            state = stop;
            break;
    }               // Transitions end

    switch (state)  // State actions begin
    {
        case stop: 
            TimerSet(100);
            break;

        case rateRight:                                        
            // 570 - <672 == 1000
            // 672 - <800 == 500
            // 800 - <946 == 250
            // 946 - 1024 == 100
            if ((horizontal >= 570) && (horizontal < 672))
                TimerSet(1000);
            else if ((horizontal >= 672) && (horizontal < 800))
                TimerSet(500);
            else if ((horizontal >= 800) && (horizontal < 946))
                TimerSet(250);
            else if (horizontal >= 946)
                TimerSet(100);
            break;

        case rateLeft:                                          
            // 0 - <90 == 100
            // 90 - <260 == 250
            // 260 - <410 == 500
            // 410 - <530 == 1000
            if (horizontal < 90)
                TimerSet(100);
            else if ((horizontal >= 90) && (horizontal < 260))
                TimerSet(250);
            else if ((horizontal >= 260) && (horizontal < 410))
                TimerSet(500);
            else if ((horizontal >= 410) && (horizontal < 530))
                TimerSet(1000);
            break;

        case rateUp:                                        
            // 570 - <672 == 1000
            // 672 - <800 == 500
            // 800 - <946 == 250
            // 946 - 1024 == 100
            if ((vertical >= 570) && (vertical < 672))
                TimerSet(1000);
            else if ((vertical >= 672) && (vertical < 800))
                TimerSet(500);
            else if ((vertical >= 800) && (vertical < 946))
                TimerSet(250);
            else if (vertical >= 946)
                TimerSet(100);
            break;

        case rateDown:                                        
            // 0 - <90 == 100
            // 90 - <260 == 250
            // 260 - <410 == 500
            // 410 - <530 == 1000
            if (vertical < 90)
                TimerSet(100);
            else if ((vertical >= 90) && (vertical < 260))
                TimerSet(250);
            else if ((vertical >= 260) && (vertical < 410))
                TimerSet(500);
            else if ((vertical >= 410) && (vertical < 530))
                TimerSet(1000);
            break;

        case rateDownRight: 
            if ((horizontal >= 570) && (horizontal < 672) && (vertical >= 410) && (vertical < 530))
                TimerSet(1000);
            else if ((horizontal >= 672) && (horizontal < 800) && (vertical >= 260) && (vertical < 410))
                TimerSet(500);
            else if ((horizontal >= 800) && (horizontal < 946) && (vertical >= 90) && (vertical < 260))
                TimerSet(250);
            else if ((horizontal >= 946) && (vertical < 90))
                TimerSet(100);
            break;

        case rateUpRight: 
            if ((horizontal >= 570) && (horizontal < 672) && (vertical >= 570) && (vertical < 672))
                TimerSet(1000);
            else if ((horizontal >= 672) && (horizontal < 800) && (vertical >= 672) && (vertical < 800))
                TimerSet(500);
            else if ((horizontal >= 800) && (horizontal < 946) && (vertical >= 800) && (vertical < 946))
                TimerSet(250);
            else if ((horizontal >= 946) && (vertical >= 946))
                TimerSet(100);
            break;

        case rateDownLeft: 
            if ((horizontal >= 410) && (horizontal < 530) && (vertical >= 410) && (vertical < 530))
                TimerSet(1000);
            else if ((horizontal >= 260) && (horizontal < 410) && (vertical >= 260) && (vertical < 410))
                TimerSet(500);
            else if ((horizontal >= 90) && (horizontal < 260) && (vertical >= 90) && (vertical < 260))
                TimerSet(250);
            else if ((horizontal < 90) && (vertical < 90))
                TimerSet(100);
            break;

        case rateUpLeft: 
            if ((horizontal >= 410) && (horizontal < 530) && (vertical >= 570) && (vertical < 672))
                TimerSet(1000);
            else if ((horizontal >= 260) && (horizontal < 410) && (vertical >= 672) && (vertical < 800))
                TimerSet(500);
            else if ((horizontal >= 90) && (horizontal < 260) && (vertical >= 800) && (vertical < 946))
                TimerSet(250);
            else if ((horizontal < 90) && (vertical >= 946))
                TimerSet(100);
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

    static task task1, task2, task3, task4;
    task *tasks[] = {&task1, &task2, &task3, &task4};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    task1.state = update;
    task1.period = 25;
    task1.elapsedTime = task1.period;
    task1.TickFct = &GetADCSM;

    // task2.state = stop;
    // task2.period = 25;
    // task2.elapsedTime = task2.period;
    // task2.TickFct = &SpeedSM;

    task2.state = noMove;
    task2.period = 100;
    task2.elapsedTime = task2.period;
    task2.TickFct = &JoystickSM;

    task3.state = display;
    task3.period = 50;
    task3.elapsedTime = task3.period;
    task3.TickFct = &LEDMatrixSM;

    task4.state = stop;
    task4.period = 10;
    task4.elapsedTime = task4.period;
    task4.TickFct = &SpeedSM;

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
