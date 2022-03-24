#pragma once
#include <stdint.h>
#include "pico/stdlib.h"
#include <cmath>

class COMMON
{
public:
    COMMON(/* args */) {}
    uint16_t swap_bytes(uint16_t color);
    bool roughlyEqual(double val1, double val2);
};
