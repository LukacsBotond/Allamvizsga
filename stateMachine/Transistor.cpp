#include "./include/Transistor.h"

TRANSISTOR::TRANSISTOR(ICALCULATE *icalculate)
{
    this->icalculate = icalculate;
}

bool TRANSISTOR::check()
{
    for (gatePin = 0; gatePin < 3; gatePin++)
    {
        bool pnp = transTestStart("5", gatePin);
        bool npn = transTestStart("6", gatePin);
        if (npn && pnp)
        {
            return false;
        }

        if (npn || pnp)
        {
            HFE_fw = HFECalculation("12", gatePin, npn);
            HFE_bw = HFECalculation("21", gatePin, npn);
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
            inputResistance();
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
    for (int i = 0; i < 2; i++)
    {
        modes[i] = mergeMode(gatePin, basemodes[i], gateMode);
    }
    for (int i = 0; i < 2; i++)
    {
        icalculate->SameOut3ChannelRepeat(modes[i][0] - '0', modes[i][1] - '0', modes[i][2] - '0', true);
        if (!checkIfTransistorIsOn(modes[i], gatePin))
        {
            return false;
        }
    }
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
    if (!icalculate->IsAnythingConnected(volt1, volt1Mode) || !icalculate->IsAnythingConnected(volt1, volt2Mode))
        return false;
    if (dif < 0)
        dif *= -1;
    if (dif > 3)
        return false;
    return true;
}

double TRANSISTOR::HFECalculation(const std::string &basemodes, int gatePin, bool npn)
{
    double HFE = 0;
    // const std::string basemodes = "12";
    std::string gateMode;
    if (npn)
    {
        gateMode = "6";
    }
    else
    {
        gateMode = "5";
    }
    std::string mode = mergeMode(gatePin, basemodes, gateMode);

    if (gatePin == 0)
        HFE = HFECalculationHelper(mode, gatePin, 1);
    if (gatePin == 1)
        HFE = HFECalculationHelper(mode, gatePin, 0);
    return HFE;
}

double TRANSISTOR::HFECalculationHelper(const std::string &mode, int gatePin, int collectorPin)
{
    double IbmA = 0;
    double IcmA = 0;
    int EmitterPin = 0;
    uint16_t baseVoltage = 0xffff;
    uint8_t baseCommand;
    for (int i = 0; i < 3; i++)
    {
        if (i != gatePin && i != collectorPin)
            EmitterPin = i;
    }
    switch (gatePin)
    {
    case 0:
        baseCommand = DAC_COMM_WRITE_BUFF_LOAD_ALL_B;
        break;
    case 1:
        baseCommand = DAC_COMM_WRITE_BUFF_LOAD_ALL_C;
        break;
    case 2:
        baseCommand = DAC_COMM_WRITE_BUFF_LOAD_ALL_D;
        break;
    default:
        break;
    }

    // double refCurrent;
    std::vector<double> measurement = icalculate->SameOut3ChannelRepeat(mode[0] - '0', mode[1] - '0', mode[2] - '0');
    icalculate->controller->prepareSwitchSetting(mode[0] - '0', mode[1] - '0', mode[2] - '0');
    icalculate->controller->setSwithcSetting(mode[0] - '0', mode[1] - '0', mode[2] - '0');

    while (std::abs(measurement[collectorPin] - measurement[EmitterPin]) < 2)
    {
        // std::cout << "voltDif: " << std::abs(measurement[collectorPin] - measurement[EmitterPin]) << " collector " << measurement[collectorPin] << " emitter: " << measurement[EmitterPin] << " baseVolt:" << baseVoltage << std::endl;
        if ((mode[gatePin] - '0') % 2 == 0)
            baseVoltage -= 200;
        else
            baseVoltage += 200;

        this->icalculate->controller->setVoltage(baseVoltage, baseCommand);
        measurement = icalculate->SameOut3ChannelRepeat();
    }

    double baseV = (double)((double)3.3 * (double)((double)baseVoltage / (double)UINT16_MAX));
    IbmA = double((baseV - measurement.at(gatePin)) / icalculate->controller->getTotResistor(mode[gatePin] - '0')) * 1000.0;

    if ((mode[collectorPin] - '0') % 2 == 0) // pin is in collector mode
        IcmA = double((3.3 - measurement.at(collectorPin)) / icalculate->controller->getTotResistor(mode[collectorPin] - '0')) * 1000.0;
    else // collector is actually the emmiter
    {    // search the true collector pin
        int truecollectorPin = 0;
        for (int i = 0; i < 3; i++)
        {
            if (i != gatePin && i != collectorPin)
                truecollectorPin = i;
        }
        IcmA = double((3.3 - measurement.at(truecollectorPin)) / icalculate->controller->getTotResistor(mode[truecollectorPin] - '0')) * 1000.0;
    }
    return std::abs(IcmA / IbmA);
}

std::string TRANSISTOR::mergeMode(int gatePin, const std::string &mode, const std::string &gateMode)
{
    int baseIndex = 0;
    std::string tmp = "";
    for (int j = 0; j < 3; j++)
    {
        if (gatePin == j)
        {
            tmp += gateMode;
        }
        else
        {
            tmp += mode[baseIndex];
            baseIndex++;
        }
    }
    return tmp;
}

void TRANSISTOR::inputResistance()
{
    std::string mode = "000";
    double Rin;
    if (this->mainResult == "npn transistor")
    {
        mode[collectorPin] = '2';
        mode[emmiterPin] = '1';
        mode[gatePin] = '6';
    }
    else
    {
        mode[collectorPin] = '1';
        mode[emmiterPin] = '2';
        mode[gatePin] = '5';
    }
    std::vector<double> measurement = icalculate->SameOut3ChannelRepeat(mode[0] - '0', mode[1] - '0', mode[2] - '0');
    double Ib = double((double)(3.3 - measurement.at(gatePin)) / (double)RESISTOR_HIGH) * 1000.0;
    if (this->mainResult == "npn transistor")
    {
        Rin = std::abs((measurement.at(gatePin) - measurement.at(emmiterPin)) / Ib);
    }
    else
    {
        Rin = std::abs((measurement.at(gatePin) - measurement.at(collectorPin)) / Ib);
    }
    results["Rin"] = Rin;
}
