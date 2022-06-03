#pragma once
#include "Machine.h"
#include "../../Calculate/include/ICalculate.h"
#include "../../Exceptions/include/NothingConnected.h"
#include "../../Exceptions/include/NotAResistor.h"
#include "../../Exceptions/include/PossiblyDiode.h"
#include <vector>
#include <map>

class STATE
{
protected:
    static ICALCULATE *icalculate;
    static std::vector<std::string> usedModes;
    static std::map<std::string, double> results;

public:
    STATE(/* args */)
    {
        icalculate = nullptr;
    }
    ~STATE() {}
    virtual bool check() = 0;
    virtual void calculate() = 0;

    std::map<std::string, double> getResults();
    std::vector<double> getMeasurement(std::string measurement);
    bool checkReverse(const std::string &measurementNormal, const std::string &measurementReverse);
};

std::map<std::string, double> STATE::getResults()
{
    return results;
}

std::vector<double> STATE::getMeasurement(std::string measurement)
{
    std::vector<double> measurementData;
    try
    {
        measurementData = icalculate->getMeasurement(measurement);
    }
    catch (NOSUCHMEASUREMENT &e) // No saved measurement
    {
        // make measurement
        icalculate->SameOut3ChannelRepeat(measurement[0] - '0', measurement[1] - '0', measurement[2] - '0');
        // now that there is measurement now it should succeed
        measurementData = icalculate->getMeasurement(measurement);
    }
    return measurementData;
}

bool STATE::checkReverse(const std::string &measurementNormal, const std::string &measurementReverse)
{
    std::vector<double> measurementDataNormal = getMeasurement(measurementNormal);
    std::vector<double> measurementDataReverse = getMeasurement(measurementReverse);

    // first port is not used
    if (measurementNormal[0] - '0' == 0)
    {
        if ( // need to pass in one direction or in the other direction
            !(icalculate->IsAnythingConnected(measurementDataNormal.at(1), measurementNormal[1] - '0') && icalculate->IsAnythingConnected(measurementDataNormal.at(2), measurementNormal[2] - '0')) &&
            !(icalculate->IsAnythingConnected(measurementDataReverse.at(1), measurementReverse[1] - '0') && icalculate->IsAnythingConnected(measurementDataReverse.at(2), measurementReverse[2] - '0')))
        {
            throw NOTHINGCONNECTED("1-2 is not used");
        }
        else
        {
            return commonClass->roughlyEqual((measurementDataNormal.at(1) - measurementDataNormal.at(2)), (measurementDataReverse.at(1) - measurementDataReverse.at(2)));
        }
    }
    else
    {                                        // 2. or 3. is not used
        if (measurementNormal[1] - '0' == 0) // 2. is not used
        {
            if ( // need to pass in one direction or in the other direction
                !(icalculate->IsAnythingConnected(measurementDataNormal.at(0), measurementNormal[0] - '0') &&
                  icalculate->IsAnythingConnected(measurementDataNormal.at(2), measurementNormal[2] - '0')) &&
                !(icalculate->IsAnythingConnected(measurementDataReverse.at(0), measurementReverse[0] - '0') &&
                  icalculate->IsAnythingConnected(measurementDataReverse.at(2), measurementReverse[2] - '0')))
            {
                throw NOTHINGCONNECTED("0-2 is not used");
            }
            else
            {
                return commonClass->roughlyEqual((measurementDataNormal.at(0) - measurementDataNormal.at(2)), (measurementDataReverse.at(0) - measurementDataReverse.at(2)));
            }
        }
        else // 3. is not used
        {
            if ( // need to pass in one direction or in the other direction
                !(icalculate->IsAnythingConnected(measurementDataNormal.at(0), measurementNormal[0] - '0') &&
                  icalculate->IsAnythingConnected(measurementDataNormal.at(1), measurementNormal[1] - '0')) &&
                !(icalculate->IsAnythingConnected(measurementDataReverse.at(0), measurementReverse[0] - '0') &&
                  icalculate->IsAnythingConnected(measurementDataReverse.at(1), measurementReverse[1] - '0')))
            {
                throw NOTHINGCONNECTED("0-1 is not used");
            }
            else
            {
                return commonClass->roughlyEqual((measurementDataNormal.at(0) - measurementDataNormal.at(1)), (measurementDataReverse.at(0) - measurementDataReverse.at(1)));
            }
        }
    }
}

