#include <avr/io.h>
#include <util/delay.h>
#include "output/console.hpp"
#include "sensor/hm3301.hpp"
#include "output/display/e-ink-driver.hpp"
#include "utilities/graphics/GUI_Paint.h"
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

    unsigned char image_temp[1280]={0};
    Paint paith(image_temp, 80, 128);

    display.clear();
    _delay_ms(500);
    paith.DrawStringAt(13, 0, "1.02inch", &Font12, 0xff);
    paith.DrawStringAt(22, 13, "epd", &Font12, 0xff);
    paith.DrawStringAt(10, 30, "ABC123", &Font16, 0xff);
    
    paith.DrawLine(10, 50, 60, 100, 0xff );
    paith.DrawLine(10, 100, 60, 50, 0xff);    
    paith.DrawRectangle(10, 50, 60, 100, 0xff);
    paith.DrawFilledCircle(35, 75, 20, 0xff);

    display.display(image_temp);

    // TODO: Write some intro text
}

void measure_and_display()
{
    HM3301::initialize();
    HM3301::Reading reading = HM3301::read();

    // console.write_line((unsigned char)reading.pm_2_5_concentration_cf_atm);

    // TODO: Display reading on the screen
}