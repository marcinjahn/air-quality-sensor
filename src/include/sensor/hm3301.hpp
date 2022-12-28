#ifndef HM3301_H
#define HM3301_H

#include <avr/io.h>

namespace HM3301
{
    /// @brief The output from the HM3301 Sensor
    struct Reading
    {
        uint16_t pm_1_concentration_cf_1;
        uint16_t pm_1_concentration_cf_atm;
        uint16_t pm_2_5_concentration_cf_1;
        uint16_t pm_2_5_concentration_cf_atm;
        uint16_t pm_10_concentration_cf_1;
        uint16_t pm_10_concentration_cf_atm;
        uint16_t no_particles_0_3_in_1_liter;
        uint16_t no_particles_0_5_in_1_liter;
        uint16_t no_particles_1_in_1_liter;
        uint16_t no_particles_2_5_in_1_liter;
        uint16_t no_particles_5_in_1_liter;
        uint16_t no_particles_10_in_1_liter;
    };

    /// @brief Initalize the sensor
    void initialize();

    /// @brief Read the sensor measurements
    Reading read();
}

#endif // HM3301_H
