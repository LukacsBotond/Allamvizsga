#include "./include/BaseCleanInputTest.h"

BaseCleanInputTest::BaseCleanInputTest(BASECLEANINPUT *inputTest)
{
    this->inputTest = inputTest;

    AVGVoltage();
}

BaseCleanInputTest::~BaseCleanInputTest()
{
    delete inputTest;
}

void BaseCleanInputTest::AVGVoltage()
{
    std::cout << "AVGVoltage tests" << std::endl;
    uint16_t tmp[]{10, 2, 38, 23, 38, 23, 21, 152, 126, 123, 131, 123, 213, 1215, 2323, 1563};
    uint16_t tmpLenght = 16;
    double avgVolage = inputTest->AVGVoltage(tmp, tmpLenght);
    double reference = (382.75 * 3.3) / 4096.0;
    if (avgVolage <= reference - 0.0001 || avgVolage >= reference + 0.0001)
    {
        errorPrinter("AVGVoltage", avgVolage, 382.75);
    }
    else
    {
        succesPrinter("AVGVoltage", "Basic int calculation");
    }

    avgVolage = inputTest->AVGVoltage(nullptr, 5);
    if (avgVolage != 0)
    {
        errorPrinter("AVGVoltage", avgVolage, 382.75);
    }
    else
    {
        succesPrinter("AVGVoltage", "null pointer check");
    }

    avgVolage = inputTest->AVGVoltage(tmp, 0);
    if (avgVolage != 0)
    {
        errorPrinter("AVGVoltage", avgVolage, 382.75);
    }
    else
    {
        succesPrinter("AVGVoltage", "0 sampleSize added");
    }
}

void BaseCleanInputTest::IsAnythingConnected()
{
}

//* Private functions

void BaseCleanInputTest::errorPrinter(std::string functionName, double calculatedValue, double correctValue)
{
    std::cout << "ERROR! BASECLEANINPUT " << functionName << " error, value supposed to be" << correctValue << " instead it is " << calculatedValue << "! \n";
}

void BaseCleanInputTest::succesPrinter(std::string functionName, std::string description)
{
    std::cout << "SUCCESS! BASECLEANINPUT " << functionName << " " << description << "! \n";
}
