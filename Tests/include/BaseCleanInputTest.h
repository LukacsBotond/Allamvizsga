#pragma once

#include "../../../Calculate/include/BaseCleanInput.h"
#include "./TestPrinter.h"

class BaseCleanInputTest
{
private:
    BASECLEANINPUT *inputTest;
    TESTPRINTER* testprinter;
public:
    BaseCleanInputTest(BASECLEANINPUT *inputTest, TESTPRINTER* testprinter);
    ~BaseCleanInputTest();

    void AVGVoltage();
    void IsAnythingConnected();
};
