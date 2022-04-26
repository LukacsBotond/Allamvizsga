#include "./include/BaseCleanInputTest.h"

BaseCleanInputTest::BaseCleanInputTest(BASECLEANINPUT *inputTest, TESTPRINTER* testprinter)
{
    this->inputTest = inputTest;
    this->testprinter = testprinter;
    AVGVoltage();
    IsAnythingConnected();
}

BaseCleanInputTest::~BaseCleanInputTest()
{
    delete inputTest;
    delete testprinter;
}

void BaseCleanInputTest::AVGVoltage()
{
    std::cout << "AVGVoltage tests" << std::endl;
    uint16_t tmp[]{10, 2, 38, 23, 38, 23, 21, 152, 126, 123, 131, 123, 213, 1215, 2323, 1563};
    uint16_t tmpLenght = 16;
    double avgVolage = inputTest->AVGVoltage(tmp, tmpLenght);
    double reference = (382.75 * 3.3) / 4096.0;
    if (avgVolage <= reference - 0.0001 || avgVolage >= reference + 0.0001)
        testprinter->errorPrinter("AVGVoltage", avgVolage, 382.75);
    else
        testprinter->succesPrinter("AVGVoltage", "Basic int calculation");

    avgVolage = inputTest->AVGVoltage(nullptr, 5);
    if (avgVolage != 0)
        testprinter->errorPrinter("AVGVoltage", avgVolage, 382.75);
    else
        testprinter->succesPrinter("AVGVoltage", "null pointer check");

    avgVolage = inputTest->AVGVoltage(tmp, 0);
    if (avgVolage != 0)
        testprinter->errorPrinter("AVGVoltage", avgVolage, 382.75);
    else
        testprinter->succesPrinter("AVGVoltage", "0 sampleSize added");
}

void BaseCleanInputTest::IsAnythingConnected()
{
    std::cout << "IsAnythingConnected test\n";

    std::cout << "Port on ground 0 \n";
    if (!inputTest->IsAnythingConnected(0.01, 0))
        testprinter->succesPrinter("IsAnythingConnected", "gnd nothing connected");
    else
        testprinter->errorPrinter("IsAnythingConnected", "gnd nothing connected");
    if (inputTest->IsAnythingConnected(0.3, 0))
        testprinter->succesPrinter("IsAnythingConnected", "");
    else
        testprinter->errorPrinter("IsAnythingConnected", "");
    if (inputTest->IsAnythingConnected(1, 0))
        testprinter->succesPrinter("IsAnythingConnected", "");
    else
        testprinter->errorPrinter("IsAnythingConnected", "");
    if (inputTest->IsAnythingConnected(3.1, 0))
        testprinter->succesPrinter("IsAnythingConnected", "");
    else
        testprinter->errorPrinter("IsAnythingConnected", "");

    std::cout << "Port on ground 1 \n";
    if (!inputTest->IsAnythingConnected(0.01, 1))
        testprinter->succesPrinter("IsAnythingConnected", "gnd nothing connected");
    else
        testprinter->errorPrinter("IsAnythingConnected", "gnd nothing connected");
    if (inputTest->IsAnythingConnected(0.3, 1))
        testprinter->succesPrinter("IsAnythingConnected", "");
    else
        testprinter->errorPrinter("IsAnythingConnected", "");
    if (inputTest->IsAnythingConnected(1, 1))
        testprinter->succesPrinter("IsAnythingConnected", "");
    else
        testprinter->errorPrinter("IsAnythingConnected", "");
    if (inputTest->IsAnythingConnected(3.1, 1))
        testprinter->succesPrinter("IsAnythingConnected", "");
    else
        testprinter->errorPrinter("IsAnythingConnected", "");

    std::cout << "Port on 3.3V high resistor 2 \n";
    if (inputTest->IsAnythingConnected(0.01, 2))
        testprinter->succesPrinter("IsAnythingConnected", "gnd nothing connected");
    else
        testprinter->errorPrinter("IsAnythingConnected", "gnd nothing connected");
    if (inputTest->IsAnythingConnected(0.3, 2))
        testprinter->succesPrinter("IsAnythingConnected", "");
    else
        testprinter->errorPrinter("IsAnythingConnected", "");
    if (inputTest->IsAnythingConnected(1, 2))
        testprinter->succesPrinter("IsAnythingConnected", "");
    else
        testprinter->errorPrinter("IsAnythingConnected", "");
    if (inputTest->IsAnythingConnected(3.09, 2))
        testprinter->succesPrinter("IsAnythingConnected", "");
    else
        testprinter->errorPrinter("IsAnythingConnected", "");
    if (!inputTest->IsAnythingConnected(3.3, 2))
        testprinter->succesPrinter("IsAnythingConnected", "");
    else
        testprinter->errorPrinter("IsAnythingConnected", "");

    std::cout << "Port on ground 3 \n";
    if (!inputTest->IsAnythingConnected(0.01, 3))
        testprinter->succesPrinter("IsAnythingConnected", "gnd nothing connected");
    else
        testprinter->errorPrinter("IsAnythingConnected", "gnd nothing connected");
    if (inputTest->IsAnythingConnected(0.3, 3))
        testprinter->succesPrinter("IsAnythingConnected", "");
    else
        testprinter->errorPrinter("IsAnythingConnected", "");
    if (inputTest->IsAnythingConnected(1, 3))
        testprinter->succesPrinter("IsAnythingConnected", "");
    else
        testprinter->errorPrinter("IsAnythingConnected", "");
    if (inputTest->IsAnythingConnected(3.1, 3))
        testprinter->succesPrinter("IsAnythingConnected", "");
    else
        testprinter->errorPrinter("IsAnythingConnected", "");

    std::cout << "Port on 3.3V low resistor 4 \n";
    if (inputTest->IsAnythingConnected(0.01, 4))
        testprinter->succesPrinter("IsAnythingConnected", "gnd nothing connected");
    else
        testprinter->errorPrinter("IsAnythingConnected", "gnd nothing connected");
    if (inputTest->IsAnythingConnected(0.3, 4))
        testprinter->succesPrinter("IsAnythingConnected", "");
    else
        testprinter->errorPrinter("IsAnythingConnected", "");
    if (inputTest->IsAnythingConnected(1, 4))
        testprinter->succesPrinter("IsAnythingConnected", "");
    else
        testprinter->errorPrinter("IsAnythingConnected", "");
    if (inputTest->IsAnythingConnected(3.09, 4))
        testprinter->succesPrinter("IsAnythingConnected", "");
    else
        testprinter->errorPrinter("IsAnythingConnected", "");
    if (!inputTest->IsAnythingConnected(3.3, 4))
        testprinter->succesPrinter("IsAnythingConnected", "");
    else
        testprinter->errorPrinter("IsAnythingConnected", "");

    std::cout << "Port on ground 5 \n";
    if (!inputTest->IsAnythingConnected(0.01, 5))
        testprinter->succesPrinter("IsAnythingConnected", "gnd nothing connected");
    else
        testprinter->errorPrinter("IsAnythingConnected", "gnd nothing connected");
    if (inputTest->IsAnythingConnected(0.3, 5))
        testprinter->succesPrinter("IsAnythingConnected", "");
    else
        testprinter->errorPrinter("IsAnythingConnected", "");
    if (inputTest->IsAnythingConnected(1, 5))
        testprinter->succesPrinter("IsAnythingConnected", "");
    else
        testprinter->errorPrinter("IsAnythingConnected", "");
    if (inputTest->IsAnythingConnected(3.1, 5))
        testprinter->succesPrinter("IsAnythingConnected", "");
    else
        testprinter->errorPrinter("IsAnythingConnected", "");
}