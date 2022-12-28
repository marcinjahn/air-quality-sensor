#ifndef I2C_H
#define I2C_H

#include <avr/io.h>

namespace I2C
{
    /// @brief Statuses for I2C operations
    enum I2CStatusCode: uint8_t {
        started = 0x08,
        repeated_start = 0x10,
        sla_w_transmitted_with_ack = 0x18,
        sla_w_transmitted_without_ack = 0x20,
        data_byte_transmitted_with_ack = 0x28,
        data_byte_transmitted_without_ack = 0x30,
        arbitration_lost = 0x38,
        sla_r_transmitted_with_ack = 0x40,
        sla_r_transmitted_without_ack = 0x48,
        data_byte_received_with_ack = 0x50,
        data_byte_received_without_ack = 0x58,
        unknown
    };

    /// @brief Sends a single byte via I2C
    /// @param address Recipient address
    /// @param byte A byte to send
    /// @return 
    I2CStatusCode write_byte(uint8_t address, uint8_t byte);

    /// @brief Reads data from external party
    /// @param address An address of the sender
    /// @param receive_buffer An array for the incoming data
    /// @param length The lengs of of the receive_buffer array (number of bytes)
    /// @return 
    I2CStatusCode read(uint8_t address, uint8_t* receive_buffer, uint8_t length);
}

#endif // I2C_H