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
        void write(char character);
        void write(const char *message);
        void write(char *message, uint16_t length);
        void write(int number);
        void write(long number);
        void write_line(char character);
        void write_line(const char *message);
        void write_line(char *message, uint16_t length);
        void write_line(int number);
        void write_line(long number);
    };
}

#endif // CONSOLE_H
