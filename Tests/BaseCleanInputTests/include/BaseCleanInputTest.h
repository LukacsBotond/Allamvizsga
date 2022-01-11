#pragma once

#include "../../../Calculate/include/BaseCleanInput.h"
#include <iostream>

class BaseCleanInputTest
{
private:
    BASECLEANINPUT *inputTest;
    void errorPrinter(std::string functionName, double calculatedValue, double correctValue);
    void errorPrinter(std::string functionName, std::string description);
    void succesPrinter(std::string functionName, std::string description = "");

public:
    BaseCleanInputTest(BASECLEANINPUT *inputTest);
    ~BaseCleanInputTest();

    void AVGVoltage();
    void IsAnythingConnected();
};
