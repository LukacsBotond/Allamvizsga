#pragma once
#include "pico/multicore.h"
#include "hardware/pll.h"
#include "./ICalculate.h"
#include "../../Exceptions/include/NotAResistor.h"

class BASECALCULATE : public ICALCULATE
{
private:
    // void calculateResult() override;

public:
    BASECALCULATE(IVALUES *values, ICLEANINPUT *cleanup, ISWITCHCONTROLLER *controller, IADCORRECTER *adccorrecter);
    ~BASECALCULATE();
    void SameOut3ChannelRepeat(const uint8_t sw1,const uint8_t sw2,const uint8_t sw3) override;

    //* Throw a NOTARESISTOR exception if it is not a resistor
    double calcResistance(std::vector<std::string> &measurements) override;

    std::vector<double> getMeasurement(const std::string measurement)const override;
    bool setMeasurement(const std::string measurement, std::vector<double> valuesVector) override;
    void cleanMesurements() override;
    // ICLEANINPUT interface
    bool IsAnythingConnected(const double avgVoltage,const uint8_t portMode) override;
};
