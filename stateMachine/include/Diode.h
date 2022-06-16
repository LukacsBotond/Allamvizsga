#pragma once
#include "State.h"
#include <vector>
#include <iostream>
#include <string>

class DIODE : public STATE
{
private:
    void setUsedPins(const std::string &usedMode);
    void saveRes(int index);

public:
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
    void calculate() override;
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

void DIODE::calculate()
{
    if (twoInverseDiode())
    {
        saveRes(0);
        return;
    }
    for (uint i = 0; i < usedModes.size(); i += 2)
    {
        if (!checkReverse(usedModes[i], usedModes[i + 1]))
        {
            saveRes(i);
            return;
        }
    }
}

//* private
void DIODE::setUsedPins(const std::string &usedMode)
{

    for (int i = 0; i < 3; i++)
    {
        int tmpMode = usedMode[i] - '0';
        // current flows from this direction so anode
        if (tmpMode % 2 == 0 && tmpMode > 0)
        {
            usedPins[i] += 'A';
        }
        // cathode
        if (tmpMode % 2 == 1)
        {
            usedPins[i] += 'C';
        }
    }
}

void DIODE::saveRes(int index)
{
    this->results.clear();
    double val = icalculate->diodeThreshold(usedModes[index]);
    this->mainResult = "Diode";
    if (val < 3)
    {
        results["fw"] = val;
        setUsedPins(usedModes[index]);
    }
    val = icalculate->diodeThreshold(usedModes[index + 1]);
    if (val < 3)
    {
        results["bw"] = val;
        setUsedPins(usedModes[index + 1]);
    }
}
