#include "communication/uart.hpp"
#include <avr/io.h>

namespace UART
{
    /// @brief Initializes UART TX communication on ATMega328P
    /// @param baud_rate The desired baud rate
    void initialize(uint16_t baud_rate = 9600)
    {
        /* Set baud rate */
        unsigned char ubrr = (F_CPU + baud_rate * 8UL) / (baud_rate * 16UL) - 1UL;

        UBRR0H = (unsigned char)(ubrr >> 8);
        UBRR0L = (unsigned char)ubrr;
        // Enable transmitter
        UCSR0B = (1 << TXEN0);
        // Set frame format: 8data, 2stop bit
        UCSR0C = (1 << USBS0) | (3 << UCSZ00);
    }

    /// @brief Sends a byte via UART
    /// @param byte A byte to send
    void send(unsigned char byte)
    {
        // Wait for empty transmit buffer
        while (!(UCSR0A & (1 << UDRE0)))
            ;
        // Put data into buffer, sends the data
        UDR0 = byte;
    }
}