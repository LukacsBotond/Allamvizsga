#pragma once
#include "./IValues.h"
#include "../../Exceptions/include/NoSuchMeasurement.h"
#include "../../ADC/include/ADCCorrecter.h"
#include "./ICleanInput.h"
#include "../../ADC/include/IADC.h"
#include "../../controll/include/IAswitch.h"
#include "../../controll/include/BaseSwithcController.h"
#include <vector>

class IADCORRECTER;
class ICALCULATE
{
protected:
public:
    static IVALUES *values;
    static ICLEANINPUT *cleanup;
    static ISWITCHCONTROLLER *controller;
    static IADCORRECTER *adccorrecter;
    virtual ~ICALCULATE() {}

    virtual void SameOut3ChannelRepeat(uint8_t sw1, uint8_t sw2, uint8_t sw3) = 0;
    virtual double calcResistance(std::vector<std::string> &measurements) = 0;
    virtual std::vector<double> getMeasurement(std::string measurement) = 0;

    void startSemaphoreRelease();
    void doneSemaphoreAquire();
    //! Technically deleted
    /*
    bool roughlyEqual(double val1, double val2)
    {
        val1 = abs(val1);
        val2 = abs(val2);
        if (val1 - 0.1 <= val2 && val1 + 0.1 >= val2)
            return true;
        return false;
    }
    */

    // ICLEANINPUT interface
    virtual bool IsAnythingConnected(double avgVoltage, uint8_t portMode) = 0;
};
