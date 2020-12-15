/*  Author: Luke McFadden
 *  Partner(s) Name: 
 *  Lab Section:
 *  Assignment: Lab 14  Exercise 1
 *  Exercise Description: 
 *
 *  I acknowledge all content contained herein, excluding template or example
 *  code, is my own original work.
 *
 * Demo Link: https://youtu.be/1TNWl7KznqI
 */

//********************************
//
//  Program for Leader ATmega - USART 0
//
//********************************

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include "usart_ATmega1284.h"
#endif


enum LED {off, on} LEDState;
enum Transmission {ReadyCheck, SendData, SentCheck, DeliveredCheck, Receive} TransState;

unsigned char tempA;
unsigned char tempVar;

void ToggleLED()
{
    switch (LEDState)  // Transitions begin
    {
        case off:
            LEDState = on;
            break;

        case on:
            LEDState = off;
            break;

        default:    
            LEDState = off;
    }               // Transitions end

    switch (LEDState)  // State actions begin
    {
        case off:
            tempA = 0x00;
            break;

        case on:
            tempA = 0x01;
            break;
    }               // State actions end

    PORTA = tempA;
}


void SendPacket()
{
    switch (TransState)  // Transitions begin
    {
        case ReadyCheck:
            if (USART_IsSendReady(0) != 0)
                TransState = SendData;
            else
                TransState = ReadyCheck;
            break;

        case SendData:
            TransState = SentCheck;
            break;

        case SentCheck:
            if (USART_HasTransmitted(0) != 0) 
                TransState = DeliveredCheck;
            else
                TransState = SentCheck;
            break;

        case DeliveredCheck:
            if (USART_HasReceived(1) != 0)
                TransState = Receive;
            else
                TransState = DeliveredCheck;
            break;

        case Receive:
            TransState = ReadyCheck;
            break;

        default: 
            TransState = ReadyCheck;
    }               // Transitions end

    switch (TransState)  // State actions begin
    {
        case ReadyCheck:
            break;

        case SendData:
            (USART_Send(tempA, 0));
            break;

        case SentCheck:
            break;

        case DeliveredCheck:
            break;

        case Receive:
            //tempVar = USART_Receive(1);
            break;
    }               // State actions end
}
    

int main(void) 
{

    DDRA = 0xFF; PORTA = 0x00;

    TimerSet(1000);
    TimerOn();
    LEDState = off;
    TransState = ReadyCheck;

    initUSART(0); // initializes USART0

    while (1) 
    {
        ToggleLED();
        SendPacket();

        while (!TimerFlag);
        TimerFlag = 0;
    }

    return 1;
}