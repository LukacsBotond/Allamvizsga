#pragma once
#include <map>
#include <vector>

class IVALUES
{

public:
    ~IVALUES(){};
    virtual void setResistance(double res) = 0;
    virtual double getResistance() = 0;

    /*
     @param measurement, which output mode was used during the measurement, for example 
     "405"
     @param values, the avaraged out voltage of each pin 
     return true if there weren no measurement with the same name
     false, if there is no 3 value in the values vector or failed to insert
    */
    virtual bool addMeasurement(std::string measurement, std::vector<double> values) = 0;
};