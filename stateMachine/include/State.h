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
public:
    static std::map<int, std::string> usedPins;
    static std::map<std::string, double> results;
    static std::string mainResult;

protected:
    static ICALCULATE *icalculate;
    static std::vector<std::string> usedModes;
    void setUsedPins(const std::string &usedMode, char mode);
    std::vector<int> usedPinsFindByValue(const std::string &value);

public:
    STATE();
    ~STATE() {}
    virtual bool check() = 0;
    virtual void calculate() = 0;

    std::map<std::string, double> getResults();
    std::vector<double> getMeasurement(std::string measurement);
    bool checkReverse(const std::string &measurementNormal, const std::string &measurementReverse);
    /*if there is a similar voltage drop with a smaller and a larger resistor then
        it is a diode
    */
    bool twoInverseDiode();
};

STATE::STATE(/* args */)
{
    icalculate = nullptr;
}

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

bool STATE::twoInverseDiode()
{
    if (usedModes.size() < 4)
    {
        return false;
    }
    double threshold1 = icalculate->diodeThreshold(usedModes.at(0));
    double threshold2 = icalculate->diodeThreshold(usedModes.at(2));
    if (std::abs(threshold1 - threshold2) < 0.5)
    {
        return true;
    }
    threshold1 = icalculate->diodeThreshold(usedModes.at(1));
    threshold2 = icalculate->diodeThreshold(usedModes.at(3));
    if (std::abs(threshold1 - threshold2) < 0.5)
    {
        return true;
    }
    std::cout << std::endl;

    return false;
}

//*protected
void STATE::setUsedPins(const std::string &usedMode, char mode)
{
    if (usedMode[0] != '0')
    {
        usedPins[0] = mode;
    }
    if (usedMode[1] != '0')
    {
        usedPins[1] = mode;
    }
    if (usedMode[2] != '0')
    {
        usedPins[2] = mode;
    }
}

std::vector<int> STATE::usedPinsFindByValue(const std::string &value)
{

    for (auto it : STATE::usedPins)
    {
        std::cout << it.first << " " << it.second << std::endl;
    }

    std::vector<int> ret;
    for (auto &it : usedPins)
    {
        if (it.second == value)
        {
            ret.push_back(it.first);
        }
    }
    return ret;
}
