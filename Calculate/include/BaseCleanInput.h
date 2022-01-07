#pragma once

#include "./ICleanInput.h"

class BASECLEANINPUT : public ICLEANINPUT
{

public:
    double AVGVoltage(uint16_t *samples, uint16_t samplesSize);
};