#pragma once
#include "State.h"
#include <vector>

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
    std::vector<double> measurement;
    std::string modes[] = {"250", "450", "205", "405", "025", "045"};
    std::string modesRev[] = {"520", "540", "502", "504", "052", "054"};
    for (int8_t i = 0; i < 6; i++)
    {
        std::cout << "reverseCheck: mode" << modes[i] << " " << modesRev[i] << " " << checkReverse(modes[i], modesRev[i]) << std::endl;
    }

    //! do a proper check
    return false;
}

void RESISTOR::calculate()
{
    // TODO implement
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
        std::cout << (measurementData1.at(1) - measurementData1.at(2)) << " " << (measurementData2.at(1) - measurementData2.at(2)) << std::endl;
        return icalculate->roughlyEqual((measurementData1.at(1) - measurementData1.at(2)), (measurementData2.at(1) - measurementData2.at(2)));
    }
    else
    {                                   // 2. or 3. is not used
        if (measurement1[1] - '0' == 0) // 2. is not used
        {
            std::cout << (measurementData1.at(0) - measurementData1.at(2)) << " " << (measurementData2.at(0) - measurementData2.at(2)) << std::endl;
            return icalculate->roughlyEqual((measurementData1.at(0) - measurementData1.at(2)), (measurementData2.at(0) - measurementData2.at(2)));
        }
        else // 3. is not used
        {
            std::cout << (measurementData1.at(1) - measurementData1.at(0)) << " " << (measurementData2.at(0) - measurementData2.at(1)) << std::endl;
            return icalculate->roughlyEqual((measurementData1.at(0) - measurementData1.at(1)), (measurementData2.at(0) - measurementData2.at(1)));
        }
    }
}
