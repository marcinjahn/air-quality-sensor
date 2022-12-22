#include <avr/io.h>
#include <util/delay.h>
#include "lib/console.hpp"
#include "lib/hm3301.hpp"

#ifndef F_CPU
#error "F_CPU has to be defined"
#endif

using namespace Output;

int main() 
{
    Console& console = Console::get_instance();

    unsigned char message[] = {"Sensor starts"};
    console.write_line(message);

    _delay_ms(5000); // TODO: 30 seconds

    HM3301::initialize();
    HM3301::Reading reading = HM3301::read();

    console.write_line((unsigned char)reading.pm_2_5_concentration_cf_atm);
}