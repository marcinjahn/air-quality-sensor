#include <avr/io.h>
#include <util/delay.h>
#include "output/console.hpp"
#include "sensor/hm3301.hpp"
#include "output/display/e-ink-driver.hpp"
#include "utilities/graphics/painter.hpp"
#include "utilities/graphics/fonts.h"

#ifndef F_CPU
#error "F_CPU has to be defined"
#endif

#define MEASUREMENT_INTERVAL_MS 5000

using namespace Output;

void display_intro();
void measure_and_display();

int main()
{
    Console &console = Console::get_instance();

    unsigned char message[] = {"S1"};
    console.write_line(message);

    display_intro();

    unsigned char message1[] = {"end"};
    console.write_line(message1);

    _delay_ms(30000); // HM3301 requires 30 seconds of delay to provide good results

    while(true)
    {
        // measure_and_display();
        _delay_ms(MEASUREMENT_INTERVAL_MS);
    }

    return 0;
}

void display_intro()
{
    EInkDisplay &display = EInkDisplay::get_instance();
    display.clear();

    unsigned char image_buffer[1280]={0};
    Painter painter(image_buffer, 80, 128);

    display.clear();
    _delay_ms(500);
    painter.draw_string(Point(13, 0), "1.02inch", &Font12);
    painter.draw_string(Point(22, 13), "epd", &Font12);
    painter.draw_string(Point(10, 30), "ABC123", &Font16);
    
    painter.draw_line(Point(10, 50), Point(60, 100));
    painter.draw_line(Point(10, 100), Point(60, 50));    
    painter.draw_rectangle(Point(10, 50), Point(60, 100));

    display.display(image_buffer);

    // TODO: Write some intro text
}

void measure_and_display()
{
    HM3301::initialize();
    HM3301::Reading reading = HM3301::read();

    // console.write_line((unsigned char)reading.pm_2_5_concentration_cf_atm);

    // TODO: Display reading on the screen
}