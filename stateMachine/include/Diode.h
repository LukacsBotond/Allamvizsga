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
    DIODE(ICALCULATE *icalculate);
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
    void calculate() override;
};

DIODE::DIODE(/* args */)
{
}

DIODE::~DIODE(ICALCULATE *icalculate)
{
}
