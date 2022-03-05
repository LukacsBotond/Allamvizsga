#pragma once
#include "IValues.h"
#include "../../Exceptions/include/NoSuchMeasurement.h"
#include <vector>

class ICALCULATE
{
protected:
public:
    virtual ~ICALCULATE() {}

    //! DELETE LATER
    virtual void calculateResult() = 0;

    virtual void startMeasurements() = 0;
    virtual void SameOut3ChannelRepeat(uint8_t sw1, uint8_t sw2, uint8_t sw3) = 0;
    virtual double calcResistance(std::string measurement) = 0;

    virtual std::vector<double> getMeasurement(std::string measurement) = 0;

    void startSemaphoreRelease();
    void doneSemaphoreAquire();
    bool roughlyEqual(double val1, double val2)
    {
        val1 = abs(val1);
        val2 = abs(val2);
        if (val1 - 0.1 <= val2 && val1 + 0.1 >= val2)
            return true;
        return false;
    }

    // ICLEANINPUT interface
    virtual bool IsAnythingConnected(double avgVoltage, uint8_t portMode) = 0;
};
