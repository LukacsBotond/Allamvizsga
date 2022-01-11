#include "./include/BaseValues.h"

BASEVALUES::~BASEVALUES() {}
void BASEVALUES::setResistance(double res)
{
    resistance = res;
}
double BASEVALUES::getResistance()
{
    return resistance;
}

bool BASEVALUES::addMeasurement(std::string measurement, std::vector<double> values)
{
    if (values.size() != 3)
        return false;

    int beforeSize = this->measurements.size();
    this->measurements.insert({measurement, values});

    if (beforeSize == this->measurements.size())
        return false;
    return true;
}

//* DEBUG

void BASEVALUES::printMeasurements()
{
    for (auto it = measurements.cbegin(); it != measurements.cend(); ++it)
    {
        std::cout << it->first << " " << it->second.at(0) << " " << it->second.at(1) << " " << it->second.at(2) << "\n";
    }
}
