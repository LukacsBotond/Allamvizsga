#pragma once

class IVALUES
{

public:
     ~IVALUES() {};
    virtual void setResistance(double res) = 0;
    virtual double getResistance() = 0;
};