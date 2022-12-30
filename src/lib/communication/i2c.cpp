#include <avr/io.h>
#include "communication/i2c.hpp"
#include "output/console.hpp"
#include "utilities/pins.hpp"

#define SLA_W 0
#define SLA_R 1

namespace I2C
{
    I2CStatusCode start();
    void stop();
    I2CStatusCode write_byte_core(uint8_t byte);
    I2CStatusCode read_core(uint8_t* received_byte, bool is_last_byte = false);
    I2CStatusCode get_status();
    
    I2CStatusCode write_byte(uint8_t address, uint8_t byte)
    {
        I2CStatusCode status;
        Output::Console &console = Output::Console::get_instance();

        status = start();
        console.write_line((char)status);

        if (status != started)
        {
            stop();
            return status;
        }

        status = write_byte_core(address | SLA_W);
        console.write_line((char)status);

        if (status != sla_w_transmitted_with_ack)
        {
            stop();
            return status;
        }

        status = write_byte_core(byte);
        console.write_line((char)status);

        stop();

        console.write_line("STARTED");

        return status;
    }

    I2CStatusCode read(uint8_t address, uint8_t* receive_buffer, uint8_t length)
    {
        Output::Console &console = Output::Console::get_instance();
        
        I2CStatusCode status;

        console.write_line("READING");

        status = start();
        console.write_line((char)status);

        if (status != started)
        {
            stop();
            return status;
        }

        status = write_byte_core(address | SLA_R);
        console.write_line((char)status);

        if (status != sla_r_transmitted_with_ack)
        {
            stop();
            return status;
        }

        for (uint8_t i = 0; i < length; i++)
        {
            status = read_core(receive_buffer + i, i == length - 1);
            console.write_line((char)status);

            if (status != data_byte_received_with_ack && status != data_byte_received_without_ack)
            {
                stop();
                return status;
            }
        }

        stop();

        console.write_line("READ");

        return status;
    }

    I2CStatusCode start()
    {
        TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
        while (!(TWCR & (1 << TWINT)))
            ;

        return get_status();
    }

    void stop()
    {
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
        while (!(TWCR & (1 << TWSTO)))
            ;
    }

    I2CStatusCode write_byte_core(uint8_t byte)
    {
        TWDR = byte;
        TWCR = (1 << TWINT) | (1 << TWEN);
        while (!(TWCR & (1 << TWINT)))
            ;

        return get_status();
    }

    I2CStatusCode read_core(uint8_t* received_byte, bool is_last_byte)
    {
        TWCR = (1 << TWINT) | (1 << TWEN) | ((is_last_byte ? 0 : 1) << TWEA);
        while (!(TWCR & (1 << TWINT)))
            ;

        *received_byte = TWDR;

        return get_status();
    }

    I2CStatusCode get_status()
    {
        int8_t status = (TWSR & 0xF8);

        switch(status)
        {
            case 0x08:
                return started;
            case 0x10:
                return repeated_start;
            case 0x18:
                return sla_w_transmitted_with_ack;
            case 0x20:
                return sla_w_transmitted_without_ack;
            case 0x28:
                return data_byte_transmitted_with_ack;
            case 0x30:
                return data_byte_transmitted_without_ack;
            case 0x38:
                return arbitration_lost;
            case 0x40:
                return sla_r_transmitted_with_ack;
            case 0x48:
                return sla_r_transmitted_without_ack;
            case 0x50:
                return data_byte_received_with_ack;
            case 0x58:
                return data_byte_received_without_ack;
            default:
                return unknown;
        }
    }

    void initialize(long i2c_frequency)
    {
        // Prescaler = 16
        set_register_bit_high(TWSR, TWPS1);

        TWBR = ((F_CPU / i2c_frequency) - 16) / 32; // 32 = 2 * Prescaler
    }
}

