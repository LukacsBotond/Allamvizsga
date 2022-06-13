#pragma once
#include <stdio.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "../../Exceptions/include/NoSuchPort.h"

class IASWITCH
{
public:
    virtual void selectOutput(const uint8_t port) = 0;
    //get switch port values
    virtual uint8_t getOutput() const = 0;
    //get resistor values
    virtual uint getResistor(const uint8_t nr) const = 0;

    //@return the current port resistance
    virtual double getTotSwitchResistance() const = 0;

    //@param mode, from which mode calculate the port resistance
    virtual double getTotSwitchResistance(uint8_t mode) const = 0;
};