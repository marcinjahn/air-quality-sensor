#include <avr/io.h>
#include <util/delay.h>
#include "lib/console.hpp"

int main() 
{
    Console& console = Console::get_instance();
    
    while(true) 
    {
        unsigned char message[] = {"To jest test"};
        console.write_line(message);
        _delay_ms(1000);
    }
}