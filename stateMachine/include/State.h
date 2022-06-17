#pragma once
#include "Machine.h"
#include "../../Calculate/include/ICalculate.h"
#include "../../Exceptions/include/NothingConnected.h"
#include "../../Exceptions/include/NotAResistor.h"
#include "../../Exceptions/include/PossiblyDiode.h"
#include <vector>
#include <map>

class STATE
{
public:
    static std::map<int, std::string> usedPins;
    static std::map<std::string, double> results;
    static std::string mainResult;

protected:
    static ICALCULATE *icalculate;
    static std::vector<std::string> usedModes;
    void setUsedPins(const std::string &usedMode, char mode);

public:
    STATE();
    ~STATE() {}
    virtual bool check() = 0;
    virtual void calculate() = 0;

    std::map<std::string, double> getResults();
    std::vector<double> getMeasurement(std::string measurement);
    bool checkReverse(const std::string &measurementNormal, const std::string &measurementReverse);
    /*if there is a similar voltage drop with a smaller and a larger resistor then
        it is a diode
    */
    bool twoInverseDiode();
    std::vector<int> usedPinsFindByValue(const std::string &value);
};
