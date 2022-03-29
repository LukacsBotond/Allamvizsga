#pragma once

#include "../../stateMachine/include/Resistor.h"
#include "./TestPrinter.h"

class RESISTORTEST
{
private:
    RESISTOR* resistor;
    TESTPRINTER *testprinter;
public:
    RESISTORTEST(RESISTOR* resistor, TESTPRINTER* testprinter);
    ~RESISTORTEST();
    void testingCheck();
    void testingCalculate();
};

RESISTORTEST::RESISTORTEST(RESISTOR* resistor, TESTPRINTER* testprinter)
{
    this->resistor = resistor;
    this->testprinter = testprinter;
}

RESISTORTEST::~RESISTORTEST()
{
    delete resistor;
}

void RESISTORTEST::testingCheck(){
    
}






