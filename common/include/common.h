#pragma once
#include <stdint.h>
#include "pico/stdlib.h"

class COMMON
{
public:
    COMMON(/* args */){}
    uint16_t swap_bytes(uint16_t color);
};
