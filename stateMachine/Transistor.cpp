#include "./include/Transistor.h"

TRANSISTOR::TRANSISTOR(ICALCULATE *icalculate)
{
    this->icalculate = icalculate;
}

bool TRANSISTOR::check()
{
    for (gatePin = 0; gatePin < 3; gatePin++)
    {
        bool npn, pnp;
        std::cout << "gatePin: " << gatePin << std::endl;
        pnp = transTestStart("5", gatePin);
        npn = transTestStart("6", gatePin);

        std::cout << "npn: " << npn << " pnp " << pnp << " HF_fw" << HFE_fw << " HF_bw " << HFE_bw << std::endl;
        if (npn && pnp)
        {
            return false;
        }

        if (npn || pnp)
        {
            if (HFE_fw > HFE_bw)
            {

                if (gatePin == 0)
                {
                    collectorPin = 1;
                    emmiterPin = 2;
                }
                else
                {
                    collectorPin = 0;
                    emmiterPin = 2;
                }
                setResults(npn, HFE_fw);
            }
            else
            {

                if (gatePin == 0)
                {
                    collectorPin = 2;
                    emmiterPin = 1;
                }
                else
                {
                    collectorPin = 2;
                    emmiterPin = 0;
                }
                setResults(npn, HFE_bw);
            }
            return true;
        }
    }
    return false;
}

void TRANSISTOR::calculate()
{
    check();
}

//* private

void TRANSISTOR::setResults(bool npn, double HFe)
{
    this->results.clear();
    this->usedPins[collectorPin] = "C";
    this->usedPins[gatePin] = "B";
    this->usedPins[emmiterPin] = "E";

    if (npn)
    {
        this->mainResult = "npn transistor";
    }
    else
    { // pnp
        this->mainResult = "pnp transistor";
    }
    results["HFe"] = HFe;
}

bool TRANSISTOR::transTestStart(const std::string &gateMode, int gatePin)
{
    bool fw, bw;

    const std::string basemodesfw[] = {"12", "56"};
    fw = transTest(gateMode, gatePin, basemodesfw, HFE_fw);

    const std::string basemodesbw[] = {"21", "65"};
    bw = transTest(gateMode, gatePin, basemodesbw, HFE_bw);
    return fw && bw;
}

bool TRANSISTOR::transTest(const std::string &gateMode, int gatePin, const std::string basemodes[], double &HFE)
{
    std::string modes[2];
    // icalculate->cleanMesurements();
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
    std::cout << "usedModes: " << modes[0] << " " << modes[1] << std::endl;

    for (int i = 0; i < 2; i++)
    {
        icalculate->SameOut3ChannelRepeat(modes[i][0] - '0', modes[i][1] - '0', modes[i][2] - '0', true);
        icalculate->values->printMeasurements();
        if (!checkIfTransistorIsOn(modes[i], gatePin))
        {
            // icalculate->values->printMeasurements();
            return false;
        }
    }

    HFE = HFECalculation(modes[0], gatePin);
    // icalculate->values->printMeasurements();
    return true;
}

bool TRANSISTOR::checkIfTransistorIsOn(const std::string &mode, int gatePin)
{
    std::vector<double> measurement = icalculate->getMeasurement(mode);
    if (!icalculate->IsAnythingConnected(measurement.at(gatePin), mode[gatePin] - '0'))
    {
        return false;
    }
    if (gatePin == 0)
    {
        return checkIfTransistorIsOnHelper(measurement.at(1), measurement.at(2), mode[1] - '0', mode[2] - '0');
    }
    else
    {
        return checkIfTransistorIsOnHelper(measurement.at(0), measurement.at(2), mode[0] - '0', mode[2] - '0');
    }
}

bool TRANSISTOR::checkIfTransistorIsOnHelper(double volt1, double volt2, int volt1Mode, int volt2Mode)
{
    double dif = volt1 - volt2;
    // std::cout << "volt dif: " << dif << std::endl;
    if (!icalculate->IsAnythingConnected(volt1, volt1Mode) || !icalculate->IsAnythingConnected(volt1, volt2Mode))
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

double TRANSISTOR::HFECalculation(const std::string &mode, int gatePin)
{
    double HFE = 0;
    if (gatePin == 0)
    {
        HFE = HFECalculationHelper(mode, gatePin, 1);
    }
    if (gatePin == 1)
    {
        HFE = HFECalculationHelper(mode, gatePin, 0);
    }
    return HFE;
}

double TRANSISTOR::HFECalculationHelper(const std::string &mode, int gatePin, int collectorPin)
{
    double IbmA = 0;
    double IcmA = 0;
    std::vector<double> measurement = icalculate->getMeasurement(mode);
    IbmA = double((3.3 - measurement.at(gatePin)) / icalculate->controller->getTotResistor(mode[gatePin] - '0')) * 1000.0;
    if ((mode[collectorPin] - '0') % 2 == 0) // pin is in collector mode
    {
        IcmA = double((3.3 - measurement.at(collectorPin)) / icalculate->controller->getTotResistor(mode[collectorPin] - '0')) * 1000.0;
    }
    else // collector is actually the emmiter
    {    // search the true collector pin
        int truecollectorPin = 0;
        for (int i = 0; i < 3; i++)
        {
            if (i != gatePin && i != collectorPin)
            {
                truecollectorPin = i;
            }
        }
        IcmA = double((3.3 - measurement.at(truecollectorPin)) / icalculate->controller->getTotResistor(mode[truecollectorPin] - '0')) * 1000.0;
    }
    return std::abs(IcmA / IbmA);
}
