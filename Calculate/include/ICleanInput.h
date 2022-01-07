#pragma once

#include <stdio.h>
#include "pico/stdlib.h"

class ICLEANINPUT
{

public:
    virtual double AVGVoltage(uint16_t *samples, uint16_t samplesSize) = 0;
};
