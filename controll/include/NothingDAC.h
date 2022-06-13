#pragma once

#include "./IDAC.h"

//! THIS ADC not even exist and does nothing
class NothingDAC : public IDAC
{
private:
    /* data */
public:
    NothingDAC(/* args */){}
    ~NothingDAC(){}
    void setVoltageOnChannel(uint16_t voltage, uint8_t command) override{}
    void reset(bool resetLvl) override{}
};