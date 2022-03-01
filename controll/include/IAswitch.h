#pragma once
#include <stdio.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "../../Exceptions/include/NoSuchPort.h"

class IASWITCH
{
public:
    virtual void selectOutput(uint8_t port) = 0;
    //get switch port values
    virtual uint8_t getOutput() = 0;
    //get resistor values
    virtual uint getResistor(uint8_t nr) = 0;

    //@return the current port resistance
    virtual double getTotSwitchResistance() = 0;

    //@param mode, from which mode calculate the port resistance
    virtual double getTotSwitchResistanceFromMode(uint8_t mode) = 0;
};