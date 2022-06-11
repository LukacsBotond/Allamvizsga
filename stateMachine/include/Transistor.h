#pragma once
#include "State.h"
#include <vector>
#include <iostream>
#include <string>
#include "../../Exceptions/include/NotATransistor.h"
class TRANSISTOR : public STATE
{
private:
    void setResults(bool npn);

    bool transTestStart(const std::string &gateMode, int gatePin);
    bool transTest(const std::string &gateMode, int gatePin, const std::string basemodes[]);
    bool checkIfTransistorIsOn(const std::string &mode, int gatePin);
    bool checkIfTransistorIsOnHelper(double volt1, double volt2);

public:
    explicit TRANSISTOR(ICALCULATE *icalculate);
    ~TRANSISTOR() {}

    bool check() override;

    void calculate() override;
};

TRANSISTOR::TRANSISTOR(ICALCULATE *icalculate)
{
    this->icalculate = icalculate;
}

bool TRANSISTOR::check()
{
    bool npn, pnp;
    /*
        if gate is powered with voltage then it is a npn transistor so gate is an anode
        else it is pnp so gate is a cathode
    */

    for (int i = 0; i < 3; i++)
    {
        pnp = transTestStart("1", i);
        npn = transTestStart("2", i);
        if (pnp)
        {
            std::cout << "pnp\n";
            setResults(false);
        }
        if (npn)
        {
            std::cout << "npn\n";
            setResults(true);
        }
    }
    return npn || pnp;
}

void TRANSISTOR::calculate()
{
    check();
}

//* private

void TRANSISTOR::setResults(bool npn)
{
    std::vector<int> gate;
    std::cout << "printResults \n";
    if (npn)
    {
        gate = this->usedPinsFindByValue("");
        std::cout << "collector:" << gate.at(0) << std::endl;
        this->usedPins[gate.at(0)] = "C";
        gate = this->usedPinsFindByValue("A");
        this->usedPins[gate.at(0)] = "B";
        gate = this->usedPinsFindByValue("C");
        this->usedPins[gate.at(0)] = "E";
    }
    else
    { // pnp
        gate = this->usedPinsFindByValue("");
        std::cout << "collector:" << gate.at(0) << std::endl;
        this->usedPins[gate.at(0)] = "C";
        gate = this->usedPinsFindByValue("C");
        this->usedPins[gate.at(0)] = "B";
        gate = this->usedPinsFindByValue("A");
        this->usedPins[gate.at(0)] = "E";
    }
}

bool TRANSISTOR::transTestStart(const std::string &gateMode, int gatePin)
{
    bool fw, bw;
    const std::string basemodesfw[] = {"12", "56"};
    fw = transTest(gateMode, gatePin, basemodesfw);
    const std::string basemodesbw[] = {"21", "65"};
    bw = transTest(gateMode, gatePin, basemodesbw);

    if (fw)
    {
        std::cout << "fw passed\n";
    }
    if (bw)
    {
        std::cout << "bw passed\n";
    }

    return fw && bw;
}

bool TRANSISTOR::transTest(const std::string &gateMode, int gatePin, const std::string basemodes[])
{

    std::string modes[2];
    icalculate->cleanMesurements();
    std::cout << "gatePin: " << gatePin << std::endl;
    for (int i = 0; i < 2; i++)
    {
        int baseIndex = 0;
        std::string tmp;
        for (int j = 0; j < 3; j++)
        {
            if (gatePin == j)
            {
                tmp += gateMode;
            }
            else
            {
                tmp += basemodes[i][baseIndex];
                baseIndex++;
            }
        }
        modes[i] = tmp;
    }
    for (int i = 0; i < 2; i++)
    {
        icalculate->SameOut3ChannelRepeat(modes[i][0] - '0', modes[i][1] - '0', modes[i][2] - '0', true);
        if (!checkIfTransistorIsOn(modes[i], gatePin))
        {
            icalculate->values->printMeasurements();
            return false;
        }
    }
    icalculate->values->printMeasurements();
    return true;
}

bool TRANSISTOR::checkIfTransistorIsOn(const std::string &mode, int gatePin)
{
    std::vector<double> measurement = icalculate->getMeasurement(mode);
    if (gatePin == 0)
    {
        return checkIfTransistorIsOnHelper(measurement.at(1), measurement.at(2));
    }
    else
    {
        return checkIfTransistorIsOnHelper(measurement.at(0), measurement.at(2));
    }
}

bool TRANSISTOR::checkIfTransistorIsOnHelper(double volt1, double volt2)
{
    double dif = volt1 - volt2;
    // std::cout << "volt dif: " << dif << std::endl;
    if (!icalculate->IsAnythingConnected(volt1, 6) || !icalculate->IsAnythingConnected(volt1, 6))
    {
        return false;
    }
    if (dif < 0)
    {
        dif *= -1;
    }
    if (dif > 3)
    {
        return false;
    }
    return true;
}