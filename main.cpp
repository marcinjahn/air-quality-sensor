#include <avr/io.h>
#include <util/delay.h>
#include "lib/output/console.hpp"
#include "lib/sensor/hm3301.hpp"
#include "lib/output/display/e-ink-driver.hpp"
#include "lib/utilities/graphics/GUI_Paint.h"
#include "lib/utilities/graphics/fonts.h"

#ifndef F_CPU
#error "F_CPU has to be defined"
#endif

#define MEASUREMENT_INTERVAL_MS 5000

using namespace Output;

int main()
{
    Console &console = Console::get_instance();

    unsigned char message[] = {"S1"};
    console.write_line(message);

    display_intro();

    _delay_ms(30000); // HM3301 requires 30 seconds of delay to provide good results

    while(true)
    {
        measure_and_display();
        _delay_ms(MEASUREMENT_INTERVAL_MS);
    }

    return 0;
}

void display_intro()
{
    EInkDisplay &display = EInkDisplay::get_instance();
    display.clear();

    unsigned char image_temp[2] = {0};
    Paint paith(image_temp, 80, 128);

    // TODO: Write some intro text
}

void measure_and_display()
{
    HM3301::initialize();
    HM3301::Reading reading = HM3301::read();

    // console.write_line((unsigned char)reading.pm_2_5_concentration_cf_atm);

    // TODO: Display reading on the screen
}