#include "./include/TestPrinter.h"

void TESTPRINTER::errorPrinter(const std::string& functionName, double calculatedValue, double correctValue) const
{
    std::cout << "ERROR! BASECLEANINPUT " << functionName << " error, value supposed to be " << correctValue << " instead it is " << calculatedValue << "!\n";
}

void TESTPRINTER::errorPrinter(const std::string& functionName,const std::string& description) const
{
    std::cout << "ERROR! BASECLEANINPUT " << functionName << " " << description << "! \n";
}

void TESTPRINTER::succesPrinter(const std::string& functionName,const std::string& description) const
{
    std::cout << "SUCCESS! BASECLEANINPUT " << functionName << " " << description << "! \n";
}
