#pragma once
#include "IValues.h"

class ICALCULATE{

    public:
        virtual ~ICALCULATE(){}
        virtual IVALUES* calculateRes() = 0;
};


