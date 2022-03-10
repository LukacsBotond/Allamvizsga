#pragma once
#include "Machine.h"
#include "../../Calculate/include/ICalculate.h"
#include <vector>

class STATE
{
protected:
    static ICALCULATE *icalculate;
    static std::vector<std::string> usedModes;
public:
    STATE(/* args */)
    {
        icalculate = nullptr;
    }
    ~STATE() {}
    virtual bool check() = 0;
    virtual void calculate() = 0;
    std::vector<double> getMeasurement(std::string measurement);
};

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
