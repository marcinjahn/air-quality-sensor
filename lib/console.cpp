#include "console.hpp"
#include <avr/io.h>

#ifndef F_CPU
#error "F_CPU has to be defined"
#endif

#ifndef CONSOLE_BAUD
#pragma message "CONSOLE_BAUD was not defined, using 9600"
#define CONSOLE_BAUD 9600
#endif

Console::Console()
{
    this->initialize();
}

void Console::initialize()
{
    /* Set baud rate */
    unsigned char ubrr = (F_CPU + CONSOLE_BAUD * 8UL) / (CONSOLE_BAUD * 16UL) - 1UL;
    UBRRH = (unsigned char)(ubrr >> 8);
    UBRRL = (unsigned char)ubrr;
    /* Enable transmitter */
    UCSRB = (1 << TXEN);
    /* Set frame format: 8data, 2stop bit */
    UCSRC = (1 << URSEL) | (1 << USBS) | (3 << UCSZ0);
}

Console &Console::get_instance()
{
    return instance;
}

void Console::write_char(unsigned char character)
{
    while (!(UCSRA & (1 << UDRE)))
            ;

    /* Put data into buffer, sends the data */
    UDR = character;
}

void Console::write(unsigned char* message)
{
    uint16_t i = 0;
    while (message[i]) {
        this->write_char(message[i++]);
    }
}

void Console::write_line(unsigned char* message)
{
    this->write(message);
    this->write_char('\n');
}

Console Console::instance;