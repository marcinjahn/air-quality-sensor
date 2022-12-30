#include "output/display/e-ink-driver.hpp"
#include "output/display/config.hpp"
#include <util/delay.h>
#include "communication/spi_master.hpp"
#include "utilities/pins.hpp"
#include "output/console.hpp"

namespace Output
{
    EInkDisplay::EInkDisplay()
    {
        initialize();
    }

    void EInkDisplay::initialize()
    {
        initialize_pins();
        SPI::initialize_master();

        reset();

        send_command(0xD2);
        send_data(0x3F);

        send_command(0x00);
        send_data(0x6F); // from outside

        send_command(0x01); // power setting
        send_data(0x03);
        send_data(0x00);
        send_data(0x2b);
        send_data(0x2b);

        send_command(0x06); // Configuring the charge pump
        send_data(0x3f);

        send_command(0x2A); // Setting XON and the options of LUT
        send_data(0x00);
        send_data(0x00);

        send_command(0x30); // Set the clock frequency
        send_data(0x13);    // 50Hz

        send_command(0x50); // Set VCOM and data output interval
        send_data(0x57);

        send_command(0x60); // Set The non-overlapping period of Gate and Source.
        send_data(0x22);

        send_command(0x61); // resolution setting
        send_data(0x50);    // source 128
        send_data(0x80);

        send_command(0x82); // sets VCOM_DC value
        send_data(0x12);    // -1v

        send_command(0xe3); // Set POWER SAVING
        send_data(0x33);
        set_lut();
        send_command(0x04); // Set POWER SAVING

        wait_for_idle();
    }

    void EInkDisplay::initialize_pins()
    {
        CS_DDR |= (1 << CS_PIN);
        RST_DDR |= (1 << RST_PIN);
        DC_DDR |= (1 << DC_PIN);
        BUSY_DDR &= ~(1 << BUSY_PIN);

        set_register_bit_high(CS_PORT, CS_PIN);
    }

    void EInkDisplay::set_lut(void)
    {
        unsigned int count;

        send_command(0x23);

        for (count = 0; count < 42; count++)
        {
            send_data(lut_w1[count]);
        }

        send_command(0x24);

        for (count = 0; count < 42; count++)
        {
            send_data(lut_b1[count]);
        }
    }

    void EInkDisplay::reset()
    {
        set_register_bit_low(RST_PORT, RST_PIN);

        _delay_ms(20);

        set_register_bit_high(RST_PORT, RST_PIN);

        _delay_ms(20);
    }

    void EInkDisplay::send_command(uint8_t command)
    {
        set_register_bit_low(CS_PORT, CS_PIN);
        set_register_bit_low(DC_PORT, DC_PIN); // command

        SPI::send(command);
        
        set_register_bit_high(CS_PORT, CS_PIN);
    }

    void EInkDisplay::send_data(uint8_t data)
    {
        set_register_bit_low(CS_PORT, CS_PIN);
        set_register_bit_high(DC_PORT, DC_PIN); // data

        SPI::send(data);

        set_register_bit_high(CS_PORT, CS_PIN);
    }

    void EInkDisplay::wait_for_idle()
    {
        bool busy = true;
        while(busy)
        {
            send_command(0x71);
            _delay_ms(50);
            busy = !get_register_bit(BUSY_STATE, BUSY_PIN);
        }

        _delay_ms(200);
    }

    void EInkDisplay::turn_on()
    {
        Console &console = Console::get_instance();

        send_command(0x04); // power on
        wait_for_idle();

        console.write_line("c3");

        send_command(0x12); // Start refreshing the screen

        console.write_line("c4");

        wait_for_idle();

        console.write_line("c5");

        send_command(0x02); // power off

        console.write_line("c6");

        wait_for_idle();
    }

    void EInkDisplay::clear()
    {
        unsigned int i;

        send_command(0x10);

        for (i = 0; i < 1280; i++)
        {
            send_data(0X00);
        }

        send_command(0x13); // Transfer new data

        for (i = 0; i < 1280; i++)
        {
            send_data(0xff);
        }

        turn_on();
    }

    void EInkDisplay::display(uint8_t *image_buffer)
    {
        Console &console = Console::get_instance();

        console.write_line("display");

        uint16_t width_bytes;
        width_bytes = (SCREEN_WIDTH % 8 == 0) ? (SCREEN_WIDTH / 8) : (SCREEN_WIDTH / 8 + 1);

        send_command(0x10);

        console.write_line("c1");

        for (uint16_t j = 0; j < SCREEN_HEIGHT; j++)
        {
            for (uint16_t i = 0; i < width_bytes; i++)
            {
                send_data(0xff);
            }
        }

        console.write_line("d1");

        send_command(0x13);

        console.write_line("c2");

        for (uint16_t j = 0; j < SCREEN_HEIGHT; j++)
        {
            for (uint16_t i = 0; i < width_bytes; i++)
            {
                send_data(~(image_buffer[i + j * width_bytes]));
            }
        }

        console.write_line("d2");
        
        turn_on();

        console.write_line("turned on");
    }

    void EInkDisplay::sleep(void)
    {
        send_command(0X02); // power off
        
        wait_for_idle();

        send_command(0X07); // deep sleep
        send_data(0xA5);

        set_register_bit_low(RST_PORT, RST_PIN);
    }

    EInkDisplay &EInkDisplay::get_instance()
    {
        return instance;
    }

    EInkDisplay EInkDisplay::instance;
}