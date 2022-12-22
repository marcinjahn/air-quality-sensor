#include "console.hpp"
#include <avr/io.h>

#ifndef CONSOLE_BAUD
#pragma message "CONSOLE_BAUD was not defined, using 9600"
#define CONSOLE_BAUD 9600
#endif

namespace Output
{
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

    void Console::write(unsigned char character)
    {
        while (!(UCSRA & (1 << UDRE)))
                ;

        /* Put data into buffer, sends the data */
        UDR = character;
    }

    void Console::write(unsigned char* message)
    {
        uint16_t i = 0;

        while (message[i])
        {
            this->write(message[i++]);
        }
    }

    void Console::write(unsigned char *message, uint16_t length)
    {  
        for (uint16_t i = 0; i < length; i++)
        {
            this->write(message[i]);
        }
    }

    void Console::write_line(unsigned char character)
    {
        this->write(character);
        this->write('\n');
    }

    void Console::write_line(unsigned char* message)
    {
        this->write(message);
        this->write('\n');
    }

    void Console::write_line(unsigned char *message, uint16_t length)
    {
        this->write(message, length);
        this->write('\n');
    }

    Console Console::instance;
}
