#pragma once
#include "State.h"
#include <vector>
#include <iostream>
#include <string>

class RESISTOR : public STATE
{
private:
    // bool checkReverse(const std::string &measurementNormal, const std::string &measurementReverse);

public:
    explicit RESISTOR(ICALCULATE *icalculate);
    ~RESISTOR() {}

    //*check order
    /*
        1st pin if used then check 2nd then 3rd
        if not then 2nd and 3rd
        if none of them pass then return false
        stores which pins were in use in static std::vector<std::string> usedModes vector
        *THROWS: POSSIBLYDIODE
        * NOTHINGCONNECTED exceptions
    */
    bool check() override;

    //*prints the resistor value or give error if no resitor is found
    /*
    checks which pins were used be calling the check() function then find the best
    resistor mode for the calculation
    @return double: resistor value
    *THROWS a NOTARESISTOR exception
    */
    void calculate() override;
};

RESISTOR::RESISTOR(ICALCULATE *icalculate)
{
    this->icalculate = icalculate;
}

bool RESISTOR::check()
{
    bool flag = false;
    std::string modes[] = {"210", "430", "650", "201", "401", "605", "021", "043", "065"};
    std::string modesRev[] = {"120", "340", "560", "102", "304", "506", "012", "034", "056"};
    for (int8_t i = 0; i < 9; i++)
    {
        try
        {
            // std::cout << "reverseCheck: mode" << modes[i] << " " << modesRev[i] << " " << std::endl;
            if (!checkReverse(modes[i], modesRev[i]))
            {
                usedModes.push_back(modes[i]);
                usedModes.push_back(modesRev[i]);
                // std::cout << "REVERSE CHECK FAILED, POSSIBLY DIODE OR SIMILAR\n";
                throw POSSIBLYDIODE("reverse check failed with port modes:" + modes[i] + modesRev[i]);
            }
            usedModes.push_back(modes[i]);
            usedModes.push_back(modesRev[i]);
            flag = true;
        }
        catch (NOTHINGCONNECTED &e)
        {
            std::cout << e.what() << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    }

    if (this->twoInverseDiode())
    {
        throw POSSIBLYDIODE("2 inverse diode");
    }
    return flag;
}

void RESISTOR::calculate()
{
    this->results.clear();

    if (check())
    {
        results["resistance"] = icalculate->calcResistance(this->usedModes);
        this->mainResult = "Resistor";
        setUsedPins(usedModes.at(0), 'R');
    }
    else
    {
        // std::cout << "NO resistor found\n";
        throw NOTARESISTOR("NO PIN IS USED");
    }
}