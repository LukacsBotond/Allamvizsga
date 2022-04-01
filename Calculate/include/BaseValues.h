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
    void setResistance(const double res) override;
    double getResistance() const override;

    bool addMeasurement(const std::string measurement, std::vector<double> values) override;
    std::vector<double> getMeasurement(const std::string measurement) const override;
    void cleanMeasurements() override;

    void printMeasurements() override;
};
