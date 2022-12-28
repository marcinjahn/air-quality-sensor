#include <avr/io.h>

#ifndef UART_H
#define UART_H

namespace UART
{
    /// @brief An UART initializer. It has hardcoded transmission parameters (8bit, with 2 stop bits).
    /// Only the baud rate is adjustable.
    /// @param baud_rate Transmission baud rate
    void initialize(uint16_t baud_rate);

    /// @brief Sends a single byte using the UART communication
    /// @param byte A byte to be sent
    void send(unsigned char byte);
}

#endif