#include <avr/io.h>

#ifndef SPI_H
#define SPI_H

namespace SPI
{
    /// @brief SPI initialization (for a master), it has some configuration hard-coded
    void initialize_master();

    /// @brief Sends a single byte via SPI
    /// @param byte A byte to send
    void send(uint8_t byte);
}

#endif // SPI_H