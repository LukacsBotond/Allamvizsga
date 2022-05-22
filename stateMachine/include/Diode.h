#pragma once
#include "State.h"
#include <vector>
#include <iostream>
#include <string>

class DIODE : public STATE
{
private:
    /* data */
public:
    DIODE() {}
    explicit DIODE(ICALCULATE *icalculate);
    ~DIODE();
    /*
    1st pin if used then check 2nd then 3rd
    if not then 2nd and 3rd
    if none of them pass then return false
    stores which pins were in use in static std::vector<std::string> usedModes vector
*/
    bool check() override;

    //*prints the diode threshold voltage value or give error if no diode is found
    /*
    checks which pins were used be calling the check() function then find the best
    diode mode for the calculation
    */
    std::map<std::string, double> calculate() override;
};

DIODE::DIODE(ICALCULATE *icalculate)
{
    this->icalculate = icalculate;
}

bool DIODE::check()
{
    for (uint i = 0; i < usedModes.size(); i += 2)
    {
        if (!checkReverse(usedModes[i], usedModes[i + 1]))
        {
            return true;
        }
    }
    return false;
}

std::map<std::string, double> DIODE::calculate()
{
    std::map<std::string, double> ret;
    for (uint i = 0; i < usedModes.size(); i += 2)
    {
        if (!checkReverse(usedModes[i], usedModes[i + 1]))
        {
            ret["fw"] = icalculate->diodeThreshold(usedModes[i]);
            ret["bw"] = icalculate->diodeThreshold(usedModes[i + 1]);
            return ret;
        }
    }
    return ret;
}