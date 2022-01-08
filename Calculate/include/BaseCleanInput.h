#pragma once

#include "./ICleanInput.h"

class BASECLEANINPUT : public ICLEANINPUT
{

public:
    //return the avagage voltage from multiple measurements
    double AVGVoltage(uint16_t *samples, uint16_t samplesSize) override;

        //true is something is connected to the tester, false if nothing or righ impenade, like diode backwards
    bool IsAnythingConnected(double avgVoltage, uint8_t portMode) override;
};