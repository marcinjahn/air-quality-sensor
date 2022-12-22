#include <avr/io.h>
#include "i2c.hpp"
#include "console.hpp"

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~(1 << (bit)))
#endif

#define SLA_W 0
#define SLA_R 1

using namespace Output;

namespace I2C
{
    I2CStatusCode start();
    void stop();
    I2CStatusCode write_byte_core(uint8_t byte);
    I2CStatusCode read_core(uint8_t* received_byte);
    I2CStatusCode get_status();

    I2CStatusCode write_byte(uint8_t address, uint8_t byte)
    {
        Console& console = Console::get_instance();

        I2CStatusCode status;

        status = start();

        unsigned char message[] = {"START sent"};
        console.write_line(message);
        unsigned char message1[] = { (unsigned char)status, '\0' };
        console.write_line(message1);

        if (status != started)
        {
            return status;
        }

        status = write_byte_core(address | SLA_W);

        unsigned char message3[] = {"SLA-W sent"};
        console.write_line(message3);
        unsigned char message4[] = { (unsigned char)status, '\0' };
        console.write_line(message4);

        if (status != sla_w_transmitted_with_ack)
        {
            return status;
        }

        status = write_byte_core(byte);

        unsigned char message5[] = {"CMD sent"};
        console.write_line(message5);
        unsigned char message6[] = { (unsigned char)status, '\0' };
        console.write_line(message6);

        stop();

        unsigned char message7[] = {"STOP sent"};
        console.write_line(message7);

        return status;
    }

    I2CStatusCode read(uint8_t address, uint8_t* receive_buffer, uint8_t length)
    {
        Console& console = Console::get_instance();

        I2CStatusCode status;

        status = start();

        unsigned char message[] = {"START sent"};
        console.write_line(message);
        unsigned char message1[] = { (unsigned char)status, '\0' };
        console.write_line(message1);

        if (status != started)
        {
            return status;
        }

        status = write_byte_core(address | SLA_R);

        unsigned char message2[] = {"SLA-R sent"};
        console.write_line(message2);
        unsigned char message3[] = { (unsigned char)status, '\0' };
        console.write_line(message3);

        if (status != sla_r_transmitted_with_ack)
        {
            return status;
        }

        for (uint8_t i = 0; i < length; i++)
        {
            status = read_core(receive_buffer + i);

            unsigned char message4[] = {"BYTE RECEIVED"};
            console.write_line(message4);
            unsigned char message5[] = { *(receive_buffer + i) };
            console.write_line(message5, 1);

            if (status != data_byte_received_with_ack)
            {
                return status;
            }
        }

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

    I2CStatusCode read_core(uint8_t* received_byte)
    {
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
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

    // not needed, the default settings are alright
    void init(long i2c_frequency = 100000)
    {
        cbi(TWSR, TWPS0);
        cbi(TWSR, TWPS1);
        TWBR = ((F_CPU / i2c_frequency) - 16) / 2;
        TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA);
    }
}

