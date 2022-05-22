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
public:
    STATE(/* args */)
    {
        icalculate = nullptr;
    }
    ~STATE() {}
    virtual bool check() = 0;
    virtual std::map<std::string , double> calculate() = 0;
    std::vector<double> getMeasurement(std::string measurement);
    bool checkReverse(const std::string &measurementNormal, const std::string &measurementReverse);
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

bool STATE::checkReverse(const std::string &measurementNormal, const std::string &measurementReverse)
{
    std::vector<double> measurementDataNormal = getMeasurement(measurementNormal);
    std::vector<double> measurementDataReverse = getMeasurement(measurementReverse);

    //! DELETE
    std::cout << "measurementDataNormal " << measurementDataNormal[0] << " " << measurementDataNormal[1] << " " << measurementDataNormal[2] << std::endl;
    std::cout << "measurementDataReverse " << measurementDataReverse[0] << " " << measurementDataReverse[1] << " " << measurementDataReverse[2] << std::endl;
    std::cout << "measurementNormal " << measurementNormal << ": " << icalculate->IsAnythingConnected(measurementDataNormal.at(0), measurementNormal[0] - '0') << " " << icalculate->IsAnythingConnected(measurementDataNormal.at(1), measurementNormal[1] - '0') << " " << icalculate->IsAnythingConnected(measurementDataNormal.at(2), measurementNormal[2] - '0') << std::endl;
    std::cout << "measurementReverse " << measurementReverse << ": " << icalculate->IsAnythingConnected(measurementDataReverse.at(0), measurementReverse[0] - '0') << " " << icalculate->IsAnythingConnected(measurementDataReverse.at(1), measurementReverse[1] - '0') << " " << icalculate->IsAnythingConnected(measurementDataReverse.at(2), measurementReverse[2] - '0') << std::endl;
    //! DELETE END

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
            std::cout << " 1-2 " << (measurementDataNormal.at(1) - measurementDataNormal.at(2)) << " " << (measurementDataReverse.at(1) - measurementDataReverse.at(2)) << std::endl;
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
                std::cout << " 0-2 " << (measurementDataNormal.at(0) - measurementDataNormal.at(2)) << " " << (measurementDataReverse.at(0) - measurementDataReverse.at(2)) << std::endl;
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
                std::cout << " 0-1 " << (measurementDataNormal.at(0) - measurementDataNormal.at(1)) << " " << (measurementDataReverse.at(0) - measurementDataReverse.at(1)) << std::endl;
                return commonClass->roughlyEqual((measurementDataNormal.at(0) - measurementDataNormal.at(1)), (measurementDataReverse.at(0) - measurementDataReverse.at(1)));
            }
        }
    }
}
