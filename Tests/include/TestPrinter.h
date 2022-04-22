#pragma once
#include <iostream>

class TESTPRINTER
{
private:
    /* data */
public:
    TESTPRINTER(/* args */) {}
    ~TESTPRINTER() {}
    void errorPrinter(const std::string &functionName, const double calculatedValue, const double correctValue) const;
    void errorPrinter(const std::string &functionName, const std::string& description) const;
    void succesPrinter(const std::string &functionName, const std::string& description) const;
};
