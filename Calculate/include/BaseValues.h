#pragma once

#include "./IValues.h"
#include <map>
#include <vector>

class BASEVALUES : public IVALUES
{
private:
    double resistance = 0;
    std::map<std::string, std::vector<double>> measurements;

public:
    virtual ~BASEVALUES();
    void setResistance(double res) override;
    double getResistance() override;

    bool addMeasurement(std::string measurement, std::vector<double> values) override;
};
