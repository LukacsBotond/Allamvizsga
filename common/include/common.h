#pragma once
#include <stdint.h>
#include "pico/stdlib.h"
#include <cmath>

class COMMON
{
public:
    COMMON(/* args */) {}
    uint16_t swap_bytes(const uint16_t color);
    bool roughlyEqual(const double val1,const double val2);
};
