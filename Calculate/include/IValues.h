#pragma once
#include <iostream>
#include <map>
#include <vector>
#include "../../Exceptions/include/NoSuchMeasurement.h"

class IVALUES
{

public:
    ~IVALUES(){};
    virtual void setResistance(const double res) = 0;
    virtual double getResistance() const = 0;

    /*
     @param measurement, which output mode was used during the measurement, for example 
     "405"
     @param values, the avaraged out voltage of each pin 
     @return true if there weren no measurement with the same name
     false, if there is no 3 value in the values vector or failed to insert
    */
    virtual bool addMeasurement(const std::string &measurement, std::vector<double> values) = 0;
    
    
    /*
    *Get Measurement data by name 
    @param measurement,(const std::string &measurement), which output mode was used during the measurement, for example 
     "405"
     @return (std::vector<double>) with 3 value, avaraged voltage of that setting if exist
     *THROW NOSUCHMEASUREMENT if there is no such saved measurement
    */
    virtual std::vector<double> getMeasurement(const std::string &measurement) const = 0;
    
    /*
        deletes the stored values
    */
    virtual void cleanMeasurements() = 0;

    //! debug
    /*
        prints stored measurement values through the serial port
    */
    virtual void printMeasurements() = 0;
};