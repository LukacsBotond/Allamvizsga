#pragma once

#include "./ICalculate.h"
#include "IValues.h"

class BASECALCULATE : pulbic ICALCULATE
{
    private:
        IValues* values;

    public:
        BASECALCULATE(IValues* values);

};
