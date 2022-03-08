#pragma once

#include "../../stateMachine/include/Resistor.h"


class RESISTORTEST
{
private:
    RESISTOR* resistor;



public:
    RESISTORTEST(RESISTOR* resistor);
    ~RESISTORTEST();
};

RESISTORTEST::RESISTORTEST(RESISTOR* resistor)
{
    this->resistor = resistor;
}

RESISTORTEST::~RESISTORTEST()
{
    delete resistor;
}


