#include "console.hpp"
#include "../communication/uart.hpp"
#include <avr/io.h>

#ifndef CONSOLE_BAUD
#pragma message "CONSOLE_BAUD was not defined, using 9600"
#define CONSOLE_BAUD 9600
#endif

#ifdef ENABLE_CONSOLE

namespace Output
{
    Console::Console()
    {
        this->initialize();
    }

    void Console::initialize()
    {
        UART::initialize(CONSOLE_BAUD);
    }

    Console &Console::get_instance()
    {
        return instance;
    }

    void Console::write(unsigned char character)
    {
        UART::send(character);
    }

    void Console::write(unsigned char *message)
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

    void Console::write(unsigned int number)
    {
        char buf[10];
        uint8_t i = 0;
        
        if (number < 10)
        {
            write((unsigned char)(number + '0'));
        }
        else
        {
            while (number)
            {
                buf[i++] = number % 10;
                number /= 10;
            }
            while (i)
            {
                write((unsigned char)(buf[--i] + '0'));
            }
        }
    }

    void Console::write_line(unsigned char character)
    {
        this->write(character);
        this->write((unsigned char)'\n');
    }

    void Console::write_line(unsigned char *message)
    {
        this->write(message);
        this->write((unsigned char)'\n');
    }

    void Console::write_line(unsigned char *message, uint16_t length)
    {
        this->write(message, length);
        this->write((unsigned char)'\n');
    }

    void Console::write_line(unsigned int number)
    {
        this->write(number);
        this->write((unsigned char)'\n');
    }

    Console Console::instance;
}

#else

namespace Output
{
    Console::Console()
    {
        this->initialize();
    }

    void Console::initialize()
    {
    }

    Console &Console::get_instance()
    {
        return instance;
    }

    void Console::write(unsigned char character)
    {
    }

    void Console::write(unsigned char *message)
    {
    }

    void Console::write(unsigned char *message, uint16_t length)
    {
    }

    void Console::write_line(unsigned char character)
    {
    }

    void Console::write_line(unsigned char *message)
    {
    }

    void Console::write_line(unsigned char *message, uint16_t length)
    {
    }

    Console Console::instance;
}

#endif