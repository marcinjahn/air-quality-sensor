#include <avr/io.h>
#include "i2c.hpp"

void start()
{
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)))
        ;
}

void stop()
{
    TWCR = (1 << TWINT) | (1 << TWEN) || (1 << TWSTA);
    while (!(TWCR & (1 << TWINT)))
        ;
}

void write_byte(uint8_t byte)
{
    TWDR = byte;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)))
        ;
}

uint8_t read_byte_with_ack()
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    while (!(TWCR & (1 << TWINT)))
        ;
    return TWDR;
}

uint8_t read_byte_without_ack()
{
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)))
        ;
    return TWDR;
}