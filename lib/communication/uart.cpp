#include "uart.hpp"

namespace UART
{
    void initialize(uint16_t baud_rate)
    {
        /* Set baud rate */
        unsigned char ubrr = (F_CPU + baud_rate * 8UL) / (baud_rate * 16UL) - 1UL;
        UBRRH = (unsigned char)(ubrr >> 8);
        UBRRL = (unsigned char)ubrr;
        /* Enable transmitter */
        UCSRB = (1 << TXEN);
        /* Set frame format: 8data, 2stop bit */
        UCSRC = (1 << URSEL) | (1 << USBS) | (3 << UCSZ0);
    }

    void send(unsigned char byte)
    {
        while (!(UCSRA & (1 << UDRE)))
            ;

        /* Put data into buffer, sends the data */
        UDR = byte;
    }
}