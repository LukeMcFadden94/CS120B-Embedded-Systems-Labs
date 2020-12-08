/*  Author: Luke McFadden
 *  Partner(s) Name: 
 *  Lab Section: 023
 *  Assignment: Lab 12  Exercise 5
 *  Exercise Description: 
 *
 *  I acknowledge all content contained herein, excluding template or example
 *  code, is my own original work.
 *
 * Demo Link: https://youtu.be/8yPD0Ia2j04
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

#define A7 (~PINA & 0x40)   // Move up
#define A6 (~PINA & 0x80)   // Move down
#define A5 (~PINA & 0x10)   // Move left
#define A4 (~PINA & 0x20)   // Move right

enum MatrixShift {display} state;
enum Buttons {standby} bState;

static unsigned int patternArr[3] = {0x3C, 0x24, 0x3C};       
static unsigned int rowArr[3] = {0x02, 0x04, 0x08}; 
static unsigned int secondRowArr[3] = {0x02, 0x04, 0x08};
static unsigned int thirdRowArr[3] = {0x02, 0x04, 0x08};      
unsigned int pattern;               // 0 - off, 1 - on
unsigned int row;                   // 0 - displayed, 1 - not displayed
unsigned int pos = 0;
unsigned int horizontal = 2;        // Goes 0 through 4, adjusts pattern
unsigned int vertical = 1;          // Goes 0 through 2, adjusts row
unsigned int tempC;
unsigned int tempD;

int displayFunc (int state)
{
    switch (state)  // Transitions begin
    {
        case display:
            state = display;
            break;

        default:
            state = display;
    }               // Transitions end

    switch (state)  // State actions begin
    {
        case display:
            if (pos < 2)
                pos++;          
            else if (pos == 2)           
                pos = 0;
            

            pattern = patternArr[pos];
        
            // How this ended up displaying 3 entire rows too low before modifications, idk
            if (vertical == 0 && rowArr[2] < 0x80)
            {
                for (int i = 0; i < 3; i++)
                    rowArr[i] = rowArr[i] << 1;
            }
            else if (vertical == 1)             
            {
                for (int i = 0; i < 3; i++)
                    rowArr[i] = secondRowArr[i] << 3;
            }
            else if (vertical == 2 && rowArr[0] > 0x04)             
            {
                for (int i = 0; i < 3; i++)
                    rowArr[i] = thirdRowArr[i] << 2;
            }

            row = ~rowArr[pos];

            if (horizontal == 0)
                pattern = (pattern >> 2);
            else if (horizontal == 1)
                pattern = (pattern >> 1);   
            else if (horizontal == 3)  
                pattern = (pattern << 1);   
            else if (horizontal == 4)  
                pattern = (pattern << 2);    
            else 
                pattern = pattern;  

            break;
    }               // State actions end

    tempC = pattern;
    tempD = row;
    return state;
}

// A7 - Move up
// A6 - Move down
// A5 - Move left
// A4 - Move right

int buttonFunc(int bState)
{
    switch (bState)
    {
        case standby:
            bState = standby;
            break;

        default:
            bState = standby;
    }

    switch (bState)
    {
        case standby:
            if (A7)         // up
            {
                if (vertical < 2)
                    vertical += 1;
            }
            else if (A6)    // down  
            {
                if (vertical > 0)
                    vertical -= 1;
            }
            else if (A5)    // left
            {
                if (horizontal < 4)
                    horizontal += 1;
            }
            else if (A4)    // right
            {
                if (horizontal > 0)
                    horizontal -= 1;
            }
            else
            {
                horizontal = horizontal;
                vertical = vertical;
            }
            break;
    }

    return bState;
}

void bitRegPortC(unsigned char data) 
{
    int i;
    for (i = 0; i < 8 ; ++i) 
    {
        PORTC = 0x08;
        PORTC |= ((data >> i) & 0x01);
        PORTC |= 0x02;  
    }
    PORTC |= 0x04;
    PORTC = 0x00;
}

void bitRegPortD(unsigned char data) 
{
    int i;
    for (i = 0; i < 8 ; ++i) 
    {
        PORTD = 0x08;
        PORTD |= ((data >> i) & 0x01);
        PORTD |= 0x02;  
    }

    PORTD |= 0x04;
    PORTD = 0x00;
}

int main(void) 
{
    int timer = 0;

    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;

    TimerSet(1);
    TimerOn();

    while (1) 
    {
        if (timer < 115)
            timer++;
        else
        {
            timer = 0;
            buttonFunc(bState);
        }

        displayFunc(state);
        bitRegPortC(tempC);
        bitRegPortD(tempD);

        while (!TimerFlag);
        TimerFlag = 0;
    }

    return 1;
}
