#include "output/console.hpp"
#include "communication/uart.hpp"
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

    void Console::write(char character)
    {
        UART::send(character);
    }

    void Console::write(const char *message)
    {
        uint16_t i = 0;

        while (message[i])
        {
            this->write(message[i++]);
        }
    }

    void Console::write(char *message, uint16_t length)
    {
        for (uint16_t i = 0; i < length; i++)
        {
            this->write(message[i]);
        }
    }

    void Console::write(int number)
    {
        char buf[10];
        uint8_t i = 0;
        
        if (number < 10 && number >= 0)
        {
            write((char)(number + '0'));
            return;
        }

        if (number < 0)
        {
            write((char)'-');
            number *= -1;
        }

        while (number)
        {
            buf[i++] = number % 10;
            number /= 10;
        }
        while (i)
        {
            write((char)(buf[--i] + '0'));
        }
    }

    void Console::write(long number)
    {
        char buf[10];
        uint8_t i = 0;
        
        if (number < 10 && number >= 0)
        {
            write((char)(number + '0'));
            return;
        }

        if (number < 0)
        {
            write((char)'-');
            number *= -1;
        }

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

    void Console::write_line(char character)
    {
        this->write(character);
        this->write((char)'\n');
    }

    void Console::write_line(const char *message)
    {
        this->write(message);
        this->write((char)'\n');
    }

    void Console::write_line(char *message, uint16_t length)
    {
        this->write(message, length);
        this->write((char)'\n');
    }

    void Console::write_line(int number)
    {
        this->write(number);
        this->write((char)'\n');
    }

    void Console::write_line(long number)
    {
        this->write(number);
        this->write((char)'\n');
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

    void Console::write(char character)
    {
    }

    void Console::write(const char *message)
    {
    }

    void Console::write(char *message, uint16_t length)
    {
    }

    void Console::write_line(char character)
    {
    }

    void Console::write_line(const char *message)
    {
    }

    void Console::write_line(char *message, uint16_t length)
    {
    }

    Console Console::instance;
}

#endif