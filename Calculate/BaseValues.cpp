#include "./include/BaseValues.h"

BASEVALUES::~BASEVALUES() {}
void BASEVALUES::setResistance(const double res)
{
    resistance = res;
}

double BASEVALUES::getResistance() const
{
    return resistance;
}

bool BASEVALUES::addMeasurement(const std::string &measurement, const std::vector<double> values)
{

    // std::cout << "addMeasurementValues: " << measurement << std::endl;
    if (values.size() != 3)
        return false;

    int beforeSize = this->measurements.size();
    this->measurements[measurement] = values;

    if (beforeSize == this->measurements.size())
        return false;
    return true;
}

std::vector<double> BASEVALUES::getMeasurement(const std::string &measurement) const
{
    // std::cout << "measurementValues: "
    //           << " val: " << measurement << "LENGHT:" << measurement.size() << std::endl;
    if (measurement.size() != 3)
        throw NOSUCHMEASUREMENT("BASEVALUES the seach string is not 3 length, string: " + measurement + "|\n");
    auto pos = this->measurements.find(measurement);
    if (pos == this->measurements.end())
    {
        throw NOSUCHMEASUREMENT("BASEVALUES no such saved measurement:" + measurement + "|\n");
    }
    return pos->second;
}

void BASEVALUES::cleanMeasurements()
{
    for (auto i : this->measurements)
    {
        i.second.clear();
    }
    this->measurements.clear();
}

//* DEBUG

void BASEVALUES::printMeasurements()
{
    for (auto it : measurements)
    {
        std::cout << it.first << " " << it.second.at(0) << " " << it.second.at(1) << " " << it.second.at(2) << std::endl;
    }
}
