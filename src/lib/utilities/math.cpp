#include "utilities/math.hpp"

namespace Math
{
    uint16_t min(uint16_t a, uint16_t b)
    {
        return a > b ? b : a;
    }

    uint16_t max(uint16_t a, uint16_t b)
    {
        return a > b ? a : b;
    }
}