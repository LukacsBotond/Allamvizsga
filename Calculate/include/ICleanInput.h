#pragma once

#include <stdio.h>
#include "pico/stdlib.h"

#include <iostream>
#include <numeric> // std::accumulate

class ICLEANINPUT
{

public:
    //return the avagage voltage from multiple measurements
    double AVGVoltage(uint16_t *samples,const uint16_t samplesSize);

    //true is something is connected to the tester, false if nothing or high impenadance, like diode backwards
    virtual bool IsAnythingConnected(const double avgVoltage,const uint8_t portMode) const = 0;
};
