#ifndef E_INK_DRIVER_H
#define E_INK_DRIVER_H

#include <avr/io.h>

namespace Output
{
    class EInkDisplay
    {
    private:
        EInkDisplay();
        static EInkDisplay instance;
        void initialize();
        void initialize_pins();
        void reset();
        void send_command(uint8_t command);
        void send_data(uint8_t data);
        void wait_for_idle();
        void set_lut(void);
        void turn_on();

    public:
        static EInkDisplay &get_instance();
        void display(uint8_t *image_buffer);
        void clear();
        void sleep();
    };
}

#endif // E_INK_DRIVER_H