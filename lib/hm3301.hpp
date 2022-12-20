#ifndef HM3301_H
#define HM3301_H

#include <avr/io.h>

class HM3301
{
private:
    HM3301();
    static HM3301 instance;
    void initialize();

public:
    static HM3301 &get_instance();
    uint8_t get_pm_25();
};

#endif // HM3301_H
