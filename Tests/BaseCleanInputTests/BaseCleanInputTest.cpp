#include "./include/BaseCleanInputTest.h"

BaseCleanInputTest::BaseCleanInputTest(BASECLEANINPUT *inputTest)
{
    this->inputTest = inputTest;

    AVGVoltage();
    IsAnythingConnected();
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
        errorPrinter("AVGVoltage", avgVolage, 382.75);
    else
        succesPrinter("AVGVoltage", "Basic int calculation");

    avgVolage = inputTest->AVGVoltage(nullptr, 5);
    if (avgVolage != 0)
        errorPrinter("AVGVoltage", avgVolage, 382.75);
    else
        succesPrinter("AVGVoltage", "null pointer check");

    avgVolage = inputTest->AVGVoltage(tmp, 0);
    if (avgVolage != 0)
        errorPrinter("AVGVoltage", avgVolage, 382.75);
    else
        succesPrinter("AVGVoltage", "0 sampleSize added");
}

void BaseCleanInputTest::IsAnythingConnected()
{
    std::cout << "IsAnythingConnected test\n";

    std::cout << "Port on ground 0 \n";
    if (!inputTest->IsAnythingConnected(0.01, 0))
        succesPrinter("IsAnythingConnected", "gnd nothing connected");
    else
        errorPrinter("IsAnythingConnected", "gnd nothing connected");
    if (inputTest->IsAnythingConnected(0.3, 0))
        succesPrinter("IsAnythingConnected", "");
    else
        errorPrinter("IsAnythingConnected", "");
    if (inputTest->IsAnythingConnected(1, 0))
        succesPrinter("IsAnythingConnected", "");
    else
        errorPrinter("IsAnythingConnected", "");
    if (inputTest->IsAnythingConnected(3.1, 0))
        succesPrinter("IsAnythingConnected", "");
    else
        errorPrinter("IsAnythingConnected", "");

    std::cout << "Port on ground 1 \n";
    if (!inputTest->IsAnythingConnected(0.01, 1))
        succesPrinter("IsAnythingConnected", "gnd nothing connected");
    else
        errorPrinter("IsAnythingConnected", "gnd nothing connected");
    if (inputTest->IsAnythingConnected(0.3, 1))
        succesPrinter("IsAnythingConnected", "");
    else
        errorPrinter("IsAnythingConnected", "");
    if (inputTest->IsAnythingConnected(1, 1))
        succesPrinter("IsAnythingConnected", "");
    else
        errorPrinter("IsAnythingConnected", "");
    if (inputTest->IsAnythingConnected(3.1, 1))
        succesPrinter("IsAnythingConnected", "");
    else
        errorPrinter("IsAnythingConnected", "");

    std::cout << "Port on 3.3V high resistor 2 \n";
    if (inputTest->IsAnythingConnected(0.01, 2))
        succesPrinter("IsAnythingConnected", "gnd nothing connected");
    else
        errorPrinter("IsAnythingConnected", "gnd nothing connected");
    if (inputTest->IsAnythingConnected(0.3, 2))
        succesPrinter("IsAnythingConnected", "");
    else
        errorPrinter("IsAnythingConnected", "");
    if (inputTest->IsAnythingConnected(1, 2))
        succesPrinter("IsAnythingConnected", "");
    else
        errorPrinter("IsAnythingConnected", "");
    if (inputTest->IsAnythingConnected(3.09, 2))
        succesPrinter("IsAnythingConnected", "");
    else
        errorPrinter("IsAnythingConnected", "");
    if (!inputTest->IsAnythingConnected(3.3, 2))
        succesPrinter("IsAnythingConnected", "");
    else
        errorPrinter("IsAnythingConnected", "");

    std::cout << "Port on ground 3 \n";
    if (!inputTest->IsAnythingConnected(0.01, 3))
        succesPrinter("IsAnythingConnected", "gnd nothing connected");
    else
        errorPrinter("IsAnythingConnected", "gnd nothing connected");
    if (inputTest->IsAnythingConnected(0.3, 3))
        succesPrinter("IsAnythingConnected", "");
    else
        errorPrinter("IsAnythingConnected", "");
    if (inputTest->IsAnythingConnected(1, 3))
        succesPrinter("IsAnythingConnected", "");
    else
        errorPrinter("IsAnythingConnected", "");
    if (inputTest->IsAnythingConnected(3.1, 3))
        succesPrinter("IsAnythingConnected", "");
    else
        errorPrinter("IsAnythingConnected", "");

    std::cout << "Port on 3.3V low resistor 4 \n";
    if (inputTest->IsAnythingConnected(0.01, 4))
        succesPrinter("IsAnythingConnected", "gnd nothing connected");
    else
        errorPrinter("IsAnythingConnected", "gnd nothing connected");
    if (inputTest->IsAnythingConnected(0.3, 4))
        succesPrinter("IsAnythingConnected", "");
    else
        errorPrinter("IsAnythingConnected", "");
    if (inputTest->IsAnythingConnected(1, 4))
        succesPrinter("IsAnythingConnected", "");
    else
        errorPrinter("IsAnythingConnected", "");
    if (inputTest->IsAnythingConnected(3.09, 4))
        succesPrinter("IsAnythingConnected", "");
    else
        errorPrinter("IsAnythingConnected", "");
    if (!inputTest->IsAnythingConnected(3.3, 4))
        succesPrinter("IsAnythingConnected", "");
    else
        errorPrinter("IsAnythingConnected", "");

    std::cout << "Port on ground 5 \n";
    if (!inputTest->IsAnythingConnected(0.01, 5))
        succesPrinter("IsAnythingConnected", "gnd nothing connected");
    else
        errorPrinter("IsAnythingConnected", "gnd nothing connected");
    if (inputTest->IsAnythingConnected(0.3, 5))
        succesPrinter("IsAnythingConnected", "");
    else
        errorPrinter("IsAnythingConnected", "");
    if (inputTest->IsAnythingConnected(1, 5))
        succesPrinter("IsAnythingConnected", "");
    else
        errorPrinter("IsAnythingConnected", "");
    if (inputTest->IsAnythingConnected(3.1, 5))
        succesPrinter("IsAnythingConnected", "");
    else
        errorPrinter("IsAnythingConnected", "");
}

//* Private functions

void BaseCleanInputTest::errorPrinter(std::string functionName, double calculatedValue, double correctValue)
{
    std::cout << "ERROR! BASECLEANINPUT " << functionName << " error, value supposed to be" << correctValue << " instead it is " << calculatedValue << "! \n";
}

void BaseCleanInputTest::errorPrinter(std::string functionName, std::string description)
{
    std::cout << "ERROR! BASECLEANINPUT " << functionName << " " << description << "! \n";
}

void BaseCleanInputTest::succesPrinter(std::string functionName, std::string description)
{
    std::cout << "SUCCESS! BASECLEANINPUT " << functionName << " " << description << "! \n";
}
