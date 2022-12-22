#include <avr/io.h>
#include "hm3301.hpp"
#include "i2c.hpp"
#include "console.hpp"

#define SENSOR_WRITE_ADDRESS 0x80
#define SENSOR_READ_ADDRESS 0x81
#define INIT_COMMAND 0x88

#define COMBINE_BYTES(msb, lsb) (msb << 8) + lsb

using namespace I2C;
using namespace Output;

namespace HM3301
{
    void initialize()
    {
        Console& console = Console::get_instance();
        
        I2CStatusCode status = write_byte(SENSOR_WRITE_ADDRESS, INIT_COMMAND);
    }

    Reading read()
    {
        uint8_t data[29];
        I2CStatusCode status = I2C::read(SENSOR_READ_ADDRESS, data, 29);

        Console& console = Console::get_instance();
        unsigned char message[] = {"HM3301 DATA RECEIVED"};
        console.write_line(message);

        // unsigned char message1[29];
        // for (uint8_t i = 0; i < 29; i++)
        // {
        //     message1[i] = (unsigned char)data[i];
        // }
        // console.write_line(message1, 29);

        Reading result;
        result.pm_1_concentration_cf_1 = COMBINE_BYTES(data[4], data[5]);
        result.pm_2_5_concentration_cf_1 = COMBINE_BYTES(data[6], data[7]);
        result.pm_10_concentration_cf_1 = COMBINE_BYTES(data[8], data[9]);
        result.pm_1_concentration_cf_atm = COMBINE_BYTES(data[10], data[11]);
        result.pm_2_5_concentration_cf_atm = COMBINE_BYTES(data[12], data[13]);
        result.pm_10_concentration_cf_atm = COMBINE_BYTES(data[14], data[15]);
        result.no_particles_0_3_in_1_liter = COMBINE_BYTES(data[16], data[17]);
        result.no_particles_0_5_in_1_liter = COMBINE_BYTES(data[18], data[19]);
        result.no_particles_1_in_1_liter = COMBINE_BYTES(data[20], data[21]);
        result.no_particles_2_5_in_1_liter = COMBINE_BYTES(data[22], data[23]);
        result.no_particles_5_in_1_liter = COMBINE_BYTES(data[24], data[25]);
        result.no_particles_10_in_1_liter = COMBINE_BYTES(data[26], data[27]);
        return result;
    }
}