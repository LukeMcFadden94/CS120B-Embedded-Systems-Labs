/*	Author: Luke McFadden
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 14  Exercise 1
 *	Exercise Description: 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * Demo Link: https://youtu.be/1TNWl7KznqI
 */

//********************************
//
//  Program for Follower ATmega - USART 1
//
//********************************

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include "usart_ATmega1284.h"
#endif

enum LED {Display} LEDState;
enum Transmission {Flush, Received, Assign} TransState;

unsigned char tempA;
unsigned char tempVar;

void ToggleLED()
{
    switch (LEDState)  // Transitions begin
    {
        case Display:
            LEDState = Display;
            break;

        default:    
            LEDState = Display;
    }               // Transitions end

    switch (LEDState)  // State actions begin
    {
        case Display:
            tempA = tempVar;
            break;

    }               // State actions end

    PORTA = tempA;
}


void ReceivePacket()
{
    switch (TransState)  // Transitions begin
    {
        case Flush:
            TransState = Received;
            break;

        case Received:
            if (USART_HasReceived(1) != 0) 
                TransState = Assign;
            else
                TransState = Received;
            break;

        case Assign:    
            TransState = Flush;
            break;

        default: 
            TransState = Flush;
    }               // Transitions end

    switch (TransState)  // State actions begin
    {
        case Flush:
            USART_Flush(1);
            break;

        case Received:
            break;

        case Assign:
            tempVar = USART_Receive(1);
            break;
    }               // State actions end
}
    

int main(void) 
{

    DDRA = 0xFF; PORTA = 0x00;
    DDRD = 0xFF; PORTD = 0x00;

    TimerSet(1001);
    TimerOn();
    LEDState = Display;
    TransState = Flush;

    initUSART(1); // initializes USART1

    while (1) 
    {
        ToggleLED();
        ReceivePacket();

        while (!TimerFlag);
        TimerFlag = 0;
    }

    return 1;
}