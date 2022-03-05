#pragma once
#include "pico/multicore.h"
#include "hardware/pll.h"

#include "./ICalculate.h"
#include "./ICleanInput.h"
#include "../../ADC/include/IADC.h"
#include "../../controll/include/IAswitch.h"
#include "../../controll/include/BaseSwithcController.h"
#include "../../Exceptions/include/NotAResistor.h"

class BASECALCULATE : public ICALCULATE
{
private:
    IVALUES *values;
    ICLEANINPUT *cleanup;
    ISWITCHCONTROLLER *controller;
    void calculateResult() override;

public:
    BASECALCULATE(IVALUES *values, ICLEANINPUT *cleanup, ISWITCHCONTROLLER *controller);
    ~BASECALCULATE();
    void startMeasurements() override;
    void SameOut3ChannelRepeat(uint8_t sw1, uint8_t sw2, uint8_t sw3) override;

    //* Throw a NOTARESISTOR exception if it is not a resistor
    double calcResistance(std::string measurement) override;

    std::vector<double> getMeasurement(std::string measurement) override;

    // ICLEANINPUT interface
    bool IsAnythingConnected(double avgVoltage, uint8_t portMode) override;
};
