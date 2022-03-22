#pragma once
#include <iostream>

class TESTPRINTER
{
private:
    /* data */
public:
    TESTPRINTER(/* args */) {}
    ~TESTPRINTER() {}
    void errorPrinter(std::string functionName, double calculatedValue, double correctValue);
    void errorPrinter(std::string functionName, std::string description);
    void succesPrinter(std::string functionName, std::string description);
};
