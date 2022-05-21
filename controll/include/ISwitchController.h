#pragma once
#include "./IAswitch.h"
#include "pico/sem.h"
/*
    Measurement mode table
    mode   swSetting   voltages
    0 ->    0          0V
    1 ->    1          0V
    2 ->    1          3.3V
    3 ->    2          0V
    4 ->    2          3.3V
    5 ->    3          0V
    6 ->    3          3.3V
*/
class ISWITCHCONTROLLER
{

public:
    /*
    With the switch mode as the parameter use the translation map to get the switch setting and
    the supply voltage for that channel
    @param sw1:const uint8_t swith mode for switch 1
    @param sw2:const uint8_t swith mode for switch 2
    @param sw3:const uint8_t swith mode for switch 3
    */
    virtual void setSwithcSetting(const uint8_t sw1, const uint8_t sw2, const uint8_t sw3) = 0;

    //@param swNum which switch we are intrested in
    //@return that switch operating mode
    //* if incorrect value was given it van generate a NOTSUPPOSEDTOREACHTHIS exception
    virtual uint8_t getSwithcSetting(const uint8_t swNum) const = 0;

    virtual uint getTotResistor(uint8_t usedMode) = 0;

    virtual void setVoltage(const uint16_t voltage, const uint8_t command) = 0;
};