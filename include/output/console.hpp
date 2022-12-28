#ifndef CONSOLE_H
#define CONSOLE_H

#include <avr/io.h>

namespace Output
{
    class Console
    {
    private:
        Console();
        static Console instance;
        void initialize();

    public:
        static Console &get_instance();
        void write(unsigned char character);
        void write(unsigned char *message);
        void write(unsigned char *message, uint16_t length);
        void write(unsigned int number);
        void write_line(unsigned char character);
        void write_line(unsigned char *message);
        void write_line(unsigned char *message, uint16_t length);
        void write_line(unsigned int number);
    };
}

#endif // CONSOLE_H
