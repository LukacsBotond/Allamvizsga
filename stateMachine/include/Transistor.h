#pragma once
#include "State.h"
#include <vector>
#include <iostream>
#include <string>
#include "../../Exceptions/include/NotATransistor.h"
#include "../../controll/include/IDAC.h"

class TRANSISTOR : public STATE
{
private:
    double HFE_fw = 0, HFE_bw = 0;
    int gatePin = 0, collectorPin = 0, emmiterPin = 0;

    /*
        saves results to usedPins and results maps
        !the collector/base/emmiter needs to be identified before calling this
        @param npn:bool true if npn was detected, false if pnp
    */
    void setResults(bool npn, double HFe);

    bool transTestStart(const std::string &gateMode, int gatePin);
    bool transTest(const std::string &gateMode, int gatePin, const std::string basemodes[], double &HFE);
    bool checkIfTransistorIsOn(const std::string &mode, int gatePin);
    bool checkIfTransistorIsOnHelper(double volt1, double volt2, int volt1Mode, int volt2Mode);

    double HFECalculation(const std::string &basemodes, int gatePin, bool npn);
    double HFECalculationHelper(const std::string &mode, int gatePin, int collectorPin);
    std::string mergeMode(int gatePin, const std::string &mode, const std::string &gateMode);
    void inputResistance();
public:
    explicit TRANSISTOR(ICALCULATE *icalculate);
    ~TRANSISTOR() {}

    bool check() override;

    void calculate() override;
};
