#pragma once
#include "pico/multicore.h"
#include "hardware/pll.h"
#include "./ICalculate.h"
#include "../../Exceptions/include/NotAResistor.h"
#include "../../Exceptions/include/CapacitorSlowCharge.h"

class ACALCULATE : public ICALCULATE
{
private:
    // void calculateResult() override;

public:
    ACALCULATE(IVALUES *values, ISWITCHCONTROLLER *controller, IADCORRECTER *adccorrecter);
    ~ACALCULATE();

    uint16_t *ChannelMeasure(const uint8_t sw1, const uint8_t sw2, const uint8_t sw3, uint32_t channelId, bool saveMeasurement = true) override;
    std::vector<double> SameOut3ChannelRepeat(const uint8_t sw1, const uint8_t sw2, const uint8_t sw3, bool saveMeasurement = true) override;

    std::vector<double> SameOut3ChannelRepeat() override;

    //* Throw a NOTARESISTOR exception if it is not a resistor
    double calcResistance(std::vector<std::string> &measurements) override;

    double diodeThreshold(std::string &measurement) override;

    double CalcCapacitance_nF(uint16_t *samples, uint16_t CAPTURE_DEPTH, double sapleRate, const uint8_t swMode) override;

    std::vector<double> getMeasurement(const std::string &measurement) const override;
    bool setMeasurement(const std::string &measurement, const std::vector<double> &valuesVector) override;
    void cleanMesurements() override;
    // ICLEANINPUT interface
    bool IsAnythingConnected(const double avgVoltage, const uint8_t portMode) override;
};
