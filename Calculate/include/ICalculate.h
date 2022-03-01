#pragma once
#include "IValues.h"
#include "../../Exceptions/include/NoSuchMeasurement.h"
class ICALCULATE
{
protected:
    bool roughlyEqual(double val1, double val2){
        if(val1 - 0.1 <= val2 && val1 + 0.1 >= val2)
            return true;
        return false;
    }

public:
    virtual ~ICALCULATE() {}

    virtual void calculateResult() = 0;

    virtual void startMeasurements() = 0;
    virtual void SameOut3ChannelRepeat(uint8_t sw1, uint8_t sw2, uint8_t sw3) = 0;
    virtual double calcResistance(std::string measurement) = 0;


    void startSemaphoreRelease();
    void doneSemaphoreAquire();
};
