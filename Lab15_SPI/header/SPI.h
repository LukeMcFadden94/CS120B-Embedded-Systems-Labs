#ifndef SPI_H
#define SPI_H

#include <avr/interrupt.h>

unsigned char receivedData = 0x00;

void SPI_MasterInit(void)
{
    DDRB = 0xB0;

    SPCR |= 0x51;
    SREG |= 0x80;
}

void SPI_SlaveInit(void)
{
    DDRB = 0x40;

    SPCR |= 0xC0;
    SREG |= 0x80;
}

void SPI_MasterTransmit(char cData)
{
    SPDR = cData;

    PORTB = 0xA0;
    while(!(SPSR & (1<<SPIF)))
        ;

    PORTB = 0xB0;
}

ISR(SPI_STC_vect)   
{   // this enable in with the SPCR register's "SPI interrupt Enable"
    // SPDR contains the received data, e.g. unsigned char receivedData = SPDR;
    receivedData = SPDR;
}

// void SPI_MasterInit(void)
// {
//     /* Set MOSI and SCK output, all others input */
//     DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK);
//     /* Enable SPI, Master, set clock rate fck/16 */
//     SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
//     sei();
// }

// void SPI_MasterTransmit(char cData)
// {
//     /* Start transmission */
//     SPDR = cData;
//     /* Wait for transmission complete */
//     while(!(SPSR & (1<<SPIF)))
//     ;
// }

// void SPI_SlaveInit(void)
//     {
//     /* Set MISO output, all others input */
//     DDR_SPI = (1<<DD_MISO);
//     /* Enable SPI */
//     SPCR = (1<<SPE);

//     sei();
// }

// char SPI_SlaveReceive(void)
// {
//     /* Wait for reception complete */
//     while(!(SPSR & (1<<SPIF)))
//     ;
//     /* Return Data Register */
//     return SPDR;
// }

#endif //SPI_H