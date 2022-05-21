#pragma once

#include "./ICleanInput.h"
#include "../../Exceptions/include/NoSuchMeasurement.h"

class ACLEANINPUT : public ICLEANINPUT
{

public:
    // return the avagage voltage from multiple measurements

    // true is something is connected to the tester, false if nothing or righ impenade, like diode backwards
    bool IsAnythingConnected(const double avgVoltage, const uint8_t portMode) const override;
};