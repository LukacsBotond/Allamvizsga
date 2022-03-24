#pragma once
#include "State.h"
#include "../../Exceptions/include/NothingConnected.h"
#include "../../Exceptions/include/NotAResistor.h"
#include "../../Exceptions/include/PossiblyDiode.h"
#include <vector>
#include <iostream>
#include <string>

class RESISTOR : public STATE
{
private:
    bool checkReverse(std::string measurementNormal, std::string measurementReverse);

public:
    RESISTOR(ICALCULATE *icalculate);
    ~RESISTOR();

    //*check order
    /*
        1st pin if used then check 2nd then 3rd
        if not then 2nd and 3rd
        if none of them pass then return false
        stores which pins were in use in static std::vector<std::string> usedModes vector
    */
    bool check() override;

    //*prints the resistor value or give error if no resitor is found
    /*
    checks which pins were used be calling the check() function then find the best
    resistor mode for the calculation
    */
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
    // std::string modes[] = {"205","405"};
    // std::string modesRev[] = {"502", "504"};
    for (int8_t i = 0; i < 6; i++)
    {
        try
        {
            std::cout << "reverseCheck: mode" << modes[i] << " " << modesRev[i] << " " << std::endl;
            if (checkReverse(modes[i], modesRev[i]))
            {
                usedModes.push_back(modes[i]);
                usedModes.push_back(modesRev[i]);
            }
            else
            {
                std::cout << "REVERSE CHECK FAILED, POSSIBLY DIODE OR SIMILAR\n";
                throw POSSIBLYDIODE("reverse check failed with port modes:" + modes[i] + modesRev[i]);
            }
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
bool RESISTOR::checkReverse(std::string measurementNormal, std::string measurementReverse)
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




