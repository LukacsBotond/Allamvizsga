#pragma once
#include "State.h"
#include "../../Exceptions/include/NothingConnected.h"
#include <vector>
#include <iostream>
#include <string>

class RESISTOR : public STATE
{
private:
    bool checkReverse(std::string measurement1, std::string measurement2);

public:
    RESISTOR(ICALCULATE *icalculate);
    ~RESISTOR();

    //*check order
    /*
        1st pin if used then check 2nd then 3rd
        if not then 2nd and 3rd
        if none of them pass then return false
    */
    bool check() override;
    void calculate() override;
};

RESISTOR::RESISTOR(ICALCULATE *icalculate)
{
    this->icalculate = icalculate;
}

RESISTOR::~RESISTOR()
{
    delete icalculate;
}

bool RESISTOR::check()
{
    bool flag = false;
    std::string modes[] = {"250", "450", "205", "405", "025", "045"};
    std::string modesRev[] = {"520", "540", "502", "504", "052", "054"};
    for (int8_t i = 0; i < 6; i++)
    {
        try
        {
            std::cout << "reverseCheck: mode" << modes[i] << " " << modesRev[i] << " " << checkReverse(modes[i], modesRev[i]) << std::endl;
            usedModes.push_back(modes[i]);
            usedModes.push_back(modesRev[i]);
            flag = true;
        }
        catch (NOTHINGCONNECTED &e)
        {
            std::cout << e.what() << std::endl;
        }
    }

    return flag;
}

void RESISTOR::calculate()
{
    std::cout << "RESISTOR CALCULATE\n";
    // TODO implement
    if (check())
    {
        std::cout << "RESISTANCE: " << icalculate->calcResistance(this->usedModes) << std::endl;
    }
    else
    {
        std::cout << "NO resistor found\n";
        throw NOTARESISTOR("NO PIN IS USED");
    }
    return;
}

//* --------------------- Private functions -------------
bool RESISTOR::checkReverse(std::string measurement1, std::string measurement2)
{
    std::vector<double> measurementData1 = getMeasurement(measurement1);
    std::vector<double> measurementData2 = getMeasurement(measurement2);
    // first port is not used
    if (measurement1[0] - '0' == 0)
    {
        if (!icalculate->IsAnythingConnected(measurementData1.at(1), measurement1[1] - '0') ||
            !icalculate->IsAnythingConnected(measurementData1.at(2), measurement1[2] - '0') ||
            !icalculate->IsAnythingConnected(measurementData2.at(1), measurement2[1] - '0') ||
            !icalculate->IsAnythingConnected(measurementData2.at(2), measurement2[2] - '0'))
        {
            throw NOTHINGCONNECTED("1-2 is not used");
        }
        std::cout << " 2-3 " << (measurementData1.at(1) - measurementData1.at(2)) << " " << (measurementData2.at(1) - measurementData2.at(2)) << std::endl;
        return icalculate->roughlyEqual((measurementData1.at(1) - measurementData1.at(2)), (measurementData2.at(1) - measurementData2.at(2)));
    }
    else
    {
        if (!icalculate->IsAnythingConnected(measurementData1.at(0), measurement1[0] - '0') ||
            !icalculate->IsAnythingConnected(measurementData2.at(0), measurement2[0] - '0'))
        {
            throw NOTHINGCONNECTED("0 is not used");
        }                               // 2. or 3. is not used
        if (measurement1[1] - '0' == 0) // 2. is not used
        {
            if (!icalculate->IsAnythingConnected(measurementData1.at(2), measurement1[2] - '0') ||
                !icalculate->IsAnythingConnected(measurementData2.at(2), measurement2[2] - '0'))
            {
                throw NOTHINGCONNECTED("0-2 is not used");
            }
            std::cout << " 1-3 " << (measurementData1.at(0) - measurementData1.at(2)) << " " << (measurementData2.at(0) - measurementData2.at(2)) << std::endl;
            return icalculate->roughlyEqual((measurementData1.at(0) - measurementData1.at(2)), (measurementData2.at(0) - measurementData2.at(2)));
        }
        else // 3. is not used
        {
            if (!icalculate->IsAnythingConnected(measurementData1.at(1), measurement1[1] - '0') ||
                !icalculate->IsAnythingConnected(measurementData2.at(1), measurement2[1] - '0'))
            {
                throw NOTHINGCONNECTED("0-1 is not used");
            }
            std::cout << " 1-2 " << (measurementData1.at(0) - measurementData1.at(1)) << " " << (measurementData2.at(0) - measurementData2.at(1)) << std::endl;
            return icalculate->roughlyEqual((measurementData1.at(0) - measurementData1.at(1)), (measurementData2.at(0) - measurementData2.at(1)));
        }
    }
}
