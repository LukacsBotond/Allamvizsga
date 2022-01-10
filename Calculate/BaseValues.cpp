#include "./include/BaseValues.h"

BASEVALUES::~BASEVALUES() {}
void BASEVALUES::setResistance(double res){
    resistance = res;
}
double BASEVALUES::getResistance(){
    return resistance;
}

bool BASEVALUES::addMeasurement(std::string measurement, std::vector<double> values){
    if(values.size() != 3)
        return false;

    int beforeSize = this->measurements.size();
    this->measurements.insert({measurement,values});

    if(beforeSize == this->measurements.size())
        return false;
    return true;
}

