#pragma once
#include "./IAswitch.h"

class ISWITCHCONTROLLER
{

public:
    //repeats the same output, 3 times for each ADC channel
    //at the end of measurement sinks the current is it had capacitance
    virtual void SameOut3ChannelRepeat(uint8_t sw1P, uint8_t sw2P, uint8_t sw3P) = 0;
};