#pragma once

#include <stdio.h>
#include "pico/stdlib.h"

class ICLEANINPUT
{

public:
    //return the avagage voltage from multiple measurements
    virtual double AVGVoltage(uint16_t *samples, uint16_t samplesSize) = 0;

    //true is something is connected to the tester, false if nothing or righ impenade, like diode backwards
    virtual bool IsAnythingConnected(double avgVoltage, uint8_t portMode) = 0;
};
