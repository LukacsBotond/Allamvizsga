#pragma once

#include "./IValues.h"

class BASEVALUES : public IVALUES
{   private:
    double resistance = 0;
    public:
        void setResistance(double res) override;
        double getResistance() override;
};
