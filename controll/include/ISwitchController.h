#pragma once
#include "./IAswitch.h"
#include "pico/sem.h"

class ISWITCHCONTROLLER
{

public:
    /*
    uses bit masking to set the swithc pins, value is set by
    0 | (sw1 << 16) | (sw2 << 18) | (sw3 << 20) where the sws can be between 0 and 3
    @param value: const uint32_t the calculated value with the above function
    */
    virtual void setSwithcSetting(const uint8_t sw1, const uint8_t sw2, const uint8_t sw3) = 0;

    //@param swNum which switch we are intrested in
    //@return that switch operating mode
    //* if incorrect value was given it van generate a NOTSUPPOSEDTOREACHTHIS exception
    virtual uint8_t getSwithcSetting(const uint8_t swNum) const = 0;

    virtual void setVoltage(const uint16_t voltage,const  uint8_t command) = 0;
};