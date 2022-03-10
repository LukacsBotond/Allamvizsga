#pragma once
#include "pico/stdlib.h"
#include <vector>
#include <iostream>
#include <string>
#include "../../Global.h"
#include "../../Calculate/include/ICalculate.h"
#include "../../Exceptions/include/NotSupposedToReachThis.h"
#include "../../Exceptions/include/Cell.h"
#include "../../Exceptions/include/PossiblyCapacitor.h"

class ICALCULATE;

class IADCORRECTER
{
protected:
    static ICALCULATE *icalculate;

public:
    virtual uint16_t *offsetCorrection(uint16_t *samples, uint16_t samplesSize) = 0;
    virtual void init() = 0;
    void loadIcalculate(ICALCULATE *icalculate)
    {
        this->icalculate = icalculate;
    }
};
