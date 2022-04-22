#pragma once

#include "../../Calculate/include/BaseCalculate.h"
#include "./TestPrinter.h"

class BaseCalculateTest
{
private:
    ICALCULATE *basecalculate;
    TESTPRINTER *testprinter;
    std::vector<double> measurement;
    std::vector<std::string> strmeasurements;

public:
    BaseCalculateTest(ICALCULATE *basecalculate, TESTPRINTER *testprinter);
    ~BaseCalculateTest();
    void calcResistanceTest();

    double voltageDropBasedOnCurrent(const double resistor, const double mAmper, const double startVoltage);
    double mAmperCalculation(const double portSumResistance, const double measuredResistor);
    void measurementAutomate(std::string measurementMode, double res1, double res2, double res3);
    void testcase(double unknownresistor);
};

BaseCalculateTest::BaseCalculateTest(ICALCULATE *basecalculate, TESTPRINTER *testprinter)
{
    this->basecalculate = basecalculate;
    this->testprinter = testprinter;
    measurement.push_back(0);
    measurement.push_back(0);
    measurement.push_back(0);
    calcResistanceTest();
}

BaseCalculateTest::~BaseCalculateTest()
{
    delete basecalculate;
}

void BaseCalculateTest::calcResistanceTest()
{
    testcase(50);
    testcase(220);
    testcase(550);
    testcase(1000);
    testcase(3300);
    testcase(5000);
    testcase(10000);
    testcase(50000);
}

//* --------------------- Private functions -------------
double BaseCalculateTest::voltageDropBasedOnCurrent(const double resistor, const double mAmper, double startVoltage)
{
    return (startVoltage - ((mAmper / 1000) * resistor));
}

double BaseCalculateTest::mAmperCalculation(const double portSumResistance, const double measuredResistor)
{
    return (3.3 / (portSumResistance + measuredResistor) * 1000);
}

void BaseCalculateTest::measurementAutomate(std::string measurementMode, double res1, double res2, double res3)
{
    double portRes = res1 + res3;
    double mAmper = mAmperCalculation(portRes, res2);
    if (measurementMode[0] == '0')
    {
        measurement.at(0) = 0.0;
        if ((measurementMode.at(1) - '0') % 2 == 0) // powered pin
        {
            measurement.at(1) = voltageDropBasedOnCurrent(res1, mAmper, 3.3);
            measurement.at(2) = voltageDropBasedOnCurrent(res2, mAmper, measurement.at(1));
        }
        else
        {
            measurement.at(2) = voltageDropBasedOnCurrent(res2, mAmper, 3.3);
            measurement.at(1) = voltageDropBasedOnCurrent(res1, mAmper, measurement.at(1));
        }
    }
    else
    {                                  // 2. or 3. is not used
        if (measurementMode[1] == '0') // 2. is not used
        {
            measurement.at(1) = 0.0;
            if ((measurementMode.at(0) - '0') % 2 == 0) // powered pin
            {
                measurement.at(0) = voltageDropBasedOnCurrent(res1, mAmper, 3.3);
                measurement.at(2) = voltageDropBasedOnCurrent(res2, mAmper, measurement.at(0));
            }
            else
            {
                measurement.at(2) = voltageDropBasedOnCurrent(res2, mAmper, 3.3);
                measurement.at(0) = voltageDropBasedOnCurrent(res1, mAmper, measurement.at(2));
            }
        }
        else
        { // 3. is not used
            measurement.at(3) = 0.0;
            if ((measurementMode.at(0) - '0') % 2 == 0) // powered pin
            {
                measurement.at(0) = voltageDropBasedOnCurrent(res1, mAmper, 3.3);
                measurement.at(1) = voltageDropBasedOnCurrent(res2, mAmper, measurement.at(0));
            }
            else
            {

                measurement.at(1) = voltageDropBasedOnCurrent(res2, mAmper, 3.3);
                measurement.at(0) = voltageDropBasedOnCurrent(res1, mAmper, measurement.at(1));
            }
        }
    }
    if (!basecalculate->setMeasurement(measurementMode, measurement))
        std::cout << "setMeasurements fail\n";
    strmeasurements.push_back(measurementMode);
}

void BaseCalculateTest::testcase(double unknownResistor)
{
    basecalculate->cleanMesurements();
    strmeasurements.clear();
    // store the values
    measurementAutomate("405", 330, unknownResistor, 306);
    measurementAutomate("504", 306, unknownResistor, 330);
    measurementAutomate("205", 4700, unknownResistor, 306);
    measurementAutomate("502", 306, unknownResistor, 4700);
    try
    {
        double res = basecalculate->calcResistance(strmeasurements);
        if (commonClass->roughlyEqual(res, unknownResistor))
        {
            testprinter->succesPrinter("BaseCalculateTest calcResistance", "accurate enough");
        }
        else
        {
            testprinter->errorPrinter("BaseCalculateTest calcResistance", res, unknownResistor);
        }
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
}