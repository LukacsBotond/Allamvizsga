#pragma once
#include "pico/multicore.h"
#include "hardware/pll.h"

#include "./ICalculate.h"
#include "./ICleanInput.h"
#include "../../ADC/include/IADC.h"
#include "../../controll/include/IAswitch.h"
#include "../../controll/include/BaseSwithcController.h"

class BASECALCULATE : public ICALCULATE
{
private:
    IVALUES *values;
    ICLEANINPUT *cleanup;
    ISWITCHCONTROLLER *controller;

/*
    //resistors values are the same for all switch, so I take the 1st one
    int GetOutResitance(uint8_t OutPort);
*/
    void calculateResult() override;

public:
    BASECALCULATE(IVALUES *values, ICLEANINPUT *cleanup, ISWITCHCONTROLLER *controller);
    ~BASECALCULATE();
    void startMeasurements() override;
    void SameOut3ChannelRepeat(uint8_t sw1, uint8_t sw2, uint8_t sw3);
};
