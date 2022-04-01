#pragma once

#include "../../Calculate/include/BaseCalculate.h"
#include "./TestPrinter.h"

class BaseCalculateTest
{
private:
    ICALCULATE *basecalculate;
    TESTPRINTER *testprinter;

public:
    BaseCalculateTest(ICALCULATE *basecalculate, TESTPRINTER *testprinter);
    ~BaseCalculateTest();
    void calcResistanceTest();

    double voltageDropBasedOnCurrent(const double resistor, const double mAmper);
};

BaseCalculateTest::BaseCalculateTest(ICALCULATE *basecalculate, TESTPRINTER *testprinter)
{
    this->basecalculate = basecalculate;
    this->testprinter = testprinter;
    calcResistanceTest();
}

BaseCalculateTest::~BaseCalculateTest()
{
    delete basecalculate;
}

void BaseCalculateTest::calcResistanceTest()
{
    std::vector<double> measurement;
    std::vector<std::string> strmeasurements;
    measurement.push_back(0);
    measurement.push_back(0);
    measurement.push_back(0);
    std::cout << "Test4\n";
    // resets in the beginning
    basecalculate->cleanMesurements();
    // store the values
    measurement.at(0) = 3.3;
    measurement.at(1) = 3.3;
    measurement.at(2) = 3.3;
    std::cout << "Test5\n";
    if (!basecalculate->setMeasurement("405", measurement))
        std::cout << "setMeasurements fail\n";
    strmeasurements.push_back("405");
    // basecalculate->setMeasurement("504", measurement);
    // strmeasurements.push_back("504");
    std::cout << "Test6\n";
    try
    {
        std::cout << "resistance: " << basecalculate->calcResistance(strmeasurements) << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    std::cout << "Test7\n";
}

//* --------------------- Private functions -------------
double BaseCalculateTest::voltageDropBasedOnCurrent(const double resistor, const double mAmper)
{
    return 3.3 - (mAmper * resistor);
}
