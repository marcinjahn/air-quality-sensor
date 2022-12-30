#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "output/console.hpp"
#include "sensor/hm3301.hpp"
#include "output/display/e-ink-driver.hpp"
#include "utilities/graphics/painter.hpp"
#include "utilities/graphics/fonts.h"

#ifndef F_CPU
#error "F_CPU has to be defined"
#endif

#define MEASUREMENT_INTERVAL_MS 1000 // e-ink refreshes already take long enough
#define LOGO_TIME 5000

using namespace Output;

void display_intro(Painter painter, uint8_t min_seconds);
void measure_and_display(Painter painter);
void ask_for_restart(Painter painter, char* message);
void get_value_to_display(char* buffer);
void display(Painter painter, char* value);

int main()
{
    Console &console = Console::get_instance();

    console.write_line("START");

    unsigned char image_buffer[1280]={0};
    Painter painter(image_buffer, 1280, 128, 80);

    display_intro(painter, HM3301_INIT_TIME_S);

    if (!HM3301::initialize())
    {
        ask_for_restart(painter, "HM3301 init failed");
        return 1;
    }

    while(true)
    {
        measure_and_display(painter);
        _delay_ms(MEASUREMENT_INTERVAL_MS);
    }

    return 0;
}

void display_intro(Painter painter, uint8_t min_seconds)
{
    EInkDisplay &display = EInkDisplay::get_instance();
    display.clear();

    painter.draw_string(Point(22, 13), "Air Quality", &Font12);
    painter.draw_string(Point(22, 25), "Sensor", &Font12);
    painter.draw_string(Point(22, 42), "Designed by", &Font12);
    painter.draw_string(Point(22, 54), "Marcin Jahn", &Font12);

    display.display(painter.get_image_buffer());

    _delay_ms(LOGO_TIME);

    painter.clear();

    painter.draw_string(Point(10, 35), "Initializing...", &Font12);

    display.display(painter.get_image_buffer());
    
    if (min_seconds - LOGO_TIME > 0)
    {
        _delay_ms((min_seconds - LOGO_TIME) * 1000); // TODO: MS
    }
}

void measure_and_display(Painter painter)
{
    Console &console = Console::get_instance();



    char value[4];
    get_value_to_display(value);

    display(painter, value);

    // console.write_line((int)reading.pm_2_5_concentration_cf_atm);
}

void display(Painter painter, char* value)
{
    EInkDisplay &display = EInkDisplay::get_instance();

    painter.clear();

    painter.draw_character(Point(8, 13), value[0], &Font40);
    painter.draw_character(Point(48, 13), value[1], &Font40);
    painter.draw_character(Point(88, 13), value[2], &Font40);

    painter.draw_rectangle(Point(0, 68), Point(127, 79), true);
    painter.draw_string(Point(30, 69), "PM 2.5 (ug/m3)", &Font12, Mode::Erase);

    display.display(painter.get_image_buffer());
}

void ask_for_restart(Painter painter, char* message)
{
    Console::get_instance().write_line(message);
    painter.draw_string(Point(3, 25), message, &Font12);
    painter.draw_string(Point(3, 37), "Please restart...", &Font12);
    EInkDisplay::get_instance().display(painter.get_image_buffer());

    while(true);
}

void get_value_to_display(char* buffer)
{
    Console &console = Console::get_instance();

    HM3301::Reading reading = HM3301::read();

    if (!reading.is_ok)
    {
        buffer[0] = ':';
        buffer[1] = '(';
        buffer[2] = '\0';
        return;
    }

    auto number = reading.pm_2_5_concentration_cf_atm;

    if (number < 10)
    {
        buffer[0] = '0';
        buffer[1] = '0';
        itoa(number, buffer + 2, 10);
    }
    else if (number < 100)
    {
        buffer[0] = '0';
        itoa(number, buffer + 1, 10);
    }
    else
    {
        itoa(reading.pm_2_5_concentration_cf_atm, buffer, 10);
    }
}