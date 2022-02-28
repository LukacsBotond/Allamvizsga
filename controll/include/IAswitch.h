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
    virtual double getTotSwitchResistance(uint8_t swNr) = 0;
};