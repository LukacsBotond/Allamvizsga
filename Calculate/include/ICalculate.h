#pragma once
#include "IValues.h"

class ICALCULATE
{
private:
    // TODO state machine
    //calculate the results of the current measurement
    virtual void calculateResult() = 0;

public:
    virtual ~ICALCULATE() {}

    virtual void startMeasurements() = 0;
    virtual void SameOut3ChannelRepeat(uint8_t sw1, uint8_t sw2, uint8_t sw3) = 0;

    void startSemaphoreRelease();
    void doneSemaphoreAquire();
};
