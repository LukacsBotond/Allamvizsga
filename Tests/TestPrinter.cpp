#include "./include/TestPrinter.h"

void TESTPRINTER::errorPrinter(std::string functionName, double calculatedValue, double correctValue) const
{
    std::cout << "ERROR! BASECLEANINPUT " << functionName << " error, value supposed to be " << correctValue << " instead it is " << calculatedValue << "!\n";
}

void TESTPRINTER::errorPrinter(std::string functionName, std::string description) const
{
    std::cout << "ERROR! BASECLEANINPUT " << functionName << " " << description << "! \n";
}

void TESTPRINTER::succesPrinter(std::string functionName, std::string description) const
{
    std::cout << "SUCCESS! BASECLEANINPUT " << functionName << " " << description << "! \n";
}
