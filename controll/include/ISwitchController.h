#pragma once
#include "./IAswitch.h"
#include "pico/sem.h"

class ISWITCHCONTROLLER
{

public:

    //TODO implement this in CALCUATE class
    //repeats the same output, 3 times for each ADC channel
    //at the end of measurement sinks the current is it had capacitance
    //virtual void SameOut3ChannelRepeat(uint8_t sw1P, uint8_t sw2P, uint8_t sw3P) = 0;

    // @param swNum which switch we want to modify
    // @param mode to which mode
    //* if incorrect value was given it van generate a NOTSUPPOSEDTOREACHTHIS exception
    virtual void setSwithcSetting(uint8_t swNum, uint8_t mode) = 0;
    //@param swNum which switch we are intrested in
    //@return that switch operating mode
    //* if incorrect value was given it van generate a NOTSUPPOSEDTOREACHTHIS exception
    virtual uint8_t getSwithcSetting(uint8_t swNum) = 0;
    //@param swNum from which switch we require the resistor value
    //@param resistorNr which resistor 0-3
    //return the selecter resistor value in Ohm
    //* if incorrect value was given it van generate a NOTSUPPOSEDTOREACHTHIS exception
    virtual uint getResistorSetting(uint8_t swNum, uint8_t resistorNr) = 0;
};