#ifndef CONSOLE_H
#define CONSOLE_H

class Console
{
private:
    Console();
    static Console instance;
    void initialize();
    void write_char(unsigned char character);

public:
    static Console &get_instance();
    void write(unsigned char *message);
    void write_line(unsigned char *message);
};

#endif // CONSOLE_H
