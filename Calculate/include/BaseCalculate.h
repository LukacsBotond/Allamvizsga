#pragma once

#include "./ICalculate.h"
#include "./ICleanInput.h"
#include "../../ADC/include/IADC.h"
#include "../../controll/include/IAswitch.h"

class BASECALCULATE : public ICALCULATE
{
private:
    IVALUES *values;
    IADC *adc;
    IASWITCH *sw1;
    IASWITCH *sw2;
    IASWITCH *sw3;
    ICLEANINPUT* cleanup;

    //resistors values are the same for all switch, so I take the 1st one
    int GetOutResitance(uint8_t OutPort);

public:
    BASECALCULATE(IVALUES *values, IADC *adc, ICLEANINPUT* cleanup, IASWITCH *sw1, IASWITCH *sw2, IASWITCH *sw3);
    ~BASECALCULATE();
    IVALUES *calculateRes();
};
